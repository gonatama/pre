#include "sound.h"

// おまじない
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* c_soundFilename[] = {
	_T("data/BGM/bgm_game.wav"),
	_T("data/SE/explosion.wav"),
	_T("data/SE/hit.wav"),
	_T("data/SE/fire_start.wav"),
	_T("data/SE/enter.wav"),
};

// グローバル変数
IDirectSound8 *g_pDirectSound = NULL;	// サウンドインターフェース

// サウンドの初期化
// hWnd:ウィンドウハンドル
HRESULT InitSound(HWND hWnd)
{
	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;

	// 協調レベル設定 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	return S_OK;
}

// 後片付け
void UninitSound()
{
	if (g_pDirectSound != NULL)
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}

// サウンドのロード
// no:サウンドナンバー（ヘッダに定義された列挙型定数）
LPDIRECTSOUNDBUFFER8 LoadSound(int no)
{
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;	// 曲データのバッファ
	DSBUFFERDESC buff;						// バッファ設定構造体

	HMMIO hMmio = NULL;						// MMIOハンドル
	MMIOINFO mmioInfo;						// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID pBlock;				// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;				// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if (!hMmio)
		return NULL;

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// キーワードをもとに検索

	if (mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索

	if (mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// 検索情報をもとに読み込み

	if (size != formatChunk.cksize) 		// 正しく読み込めなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索

	if (mmRes != MMSYSERR_NOERROR) 		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み
	char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む

	if (size != dataChunk.cksize)  		// 正しく読み込めなかったら異常終了
	{
		delete[] pData;
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);			// そこから各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// サウンドバッファを取り出す
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// 取り出したら総合バッファを捨てる
	pBaseBuffer->Release();

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// データ転送
	delete[] pData;					// 元の読み込み領域を消す

	// バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return pBuffer;
}

// 音を鳴らす
// pBuffer:音を鳴らしたいサウンドデータのセカンダリバッファ
// flag   :1(E_DS8_FLAG_LOOP)ならループ再生
void PlaySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag/*=0*/)
{	// 続きから鳴らすので、最初から鳴らしたい場合はSetCurrentPosition(0)をすること
	pBuffer->Play(0, 0, flag);
}

// 音を止める
void StopSound(LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)	// 鳴っていたら
	{
		pBuffer->Stop();	// 意味的にはPauseになる。
	}
}

// 再生中かどうか調べる
// pBuffer:音を鳴らしたいサウンドデータのセカンダリバッファ
bool IsPlaying(LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)
	{
		return true;
	}
	return false;
}

////=============================================================================
////
//// サウンド処理 [sound.cpp]
//// Author : AKIRA TANAKA
////
////=============================================================================
//#include "sound.h"
//
////*****************************************************************************
//// パラメータ構造体定義
////*****************************************************************************
//typedef struct
//{
//	char *pFilename;	// ファイル名
//	bool bLoop;			// ループするかどうか
//} PARAM;
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
//HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
//IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
//IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
//BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
//DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
//
//// 各音素材のパラメータ
//PARAM g_aParam[SOUND_LABEL_MAX] =
//{
//	{"data/BGM/bgm_title.ogg", true},		// BGM0
//	{"data/BGM/bgm001.wav", true},			// BGM1
//	{"data/SE/shot000.wav", false},			// 弾発射音
//	{"data/SE/explosion000.wav", false},	// 爆発音
//	{"data/SE/coin000.wav", false},			// コイン音
//};
//
////=============================================================================
//// 初期化
////=============================================================================
//HRESULT InitSound(HWND hWnd)
//{
//	HRESULT hr;
//
//	// COMライブラリの初期化
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	// XAudio2オブジェクトの作成
//	hr = XAudio2Create(&g_pXAudio2, 0);
//	if(FAILED(hr))
//	{
//		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);
//
//		// COMライブラリの終了処理
//		CoUninitialize();
//		return E_FAIL;
//	}
//	
//	// マスターボイスの生成
//	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
//	if(FAILED(hr))
//	{
//		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);
//
//		if(g_pXAudio2)
//		{
//			// XAudio2オブジェクトの開放
//			g_pXAudio2->Release();
//			g_pXAudio2 = NULL;
//		}
//
//		// COMライブラリの終了処理
//		CoUninitialize();
//
//		return E_FAIL;
//	}
//
//	// サウンドデータの初期化
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		HANDLE hFile;
//		DWORD dwChunkSize = 0;
//		DWORD dwChunkPosition = 0;
//		DWORD dwFiletype;
//		WAVEFORMATEXTENSIBLE wfx;
//		XAUDIO2_BUFFER buffer;
//
//		// バッファのクリア
//		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
//		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
//
//		// サウンドデータファイルの生成
//		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
//		if(hFile == INVALID_HANDLE_VALUE)
//		{
//			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
//			return HRESULT_FROM_WIN32(GetLastError());
//		}
//		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//		{// ファイルポインタを先頭に移動
//			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
//			return HRESULT_FROM_WIN32(GetLastError());
//		}
//	
//		// WAVEファイルのチェック
//		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		if(dwFiletype != 'EVAW')
//		{
//			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//	
//		// フォーマットチェック
//		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//
//		// オーディオデータ読み込み
//		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
//		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//	
//		// ソースボイスの生成
//		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
//			return S_FALSE;
//		}
//
//		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
//		buffer.AudioBytes = g_aSizeAudio[nCntSound];
//		buffer.pAudioData = g_apDataAudio[nCntSound];
//		buffer.Flags      = XAUDIO2_END_OF_STREAM;
//		buffer.LoopCount  = 0;
//
//		// オーディオバッファの登録
//		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitSound(void)
//{
//	// 一時停止
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		if(g_apSourceVoice[nCntSound])
//		{
//			// 一時停止
//			g_apSourceVoice[nCntSound]->Stop(0);
//	
//			// ソースボイスの破棄
//			g_apSourceVoice[nCntSound]->DestroyVoice();
//			g_apSourceVoice[nCntSound] = NULL;
//	
//			// オーディオデータの開放
//			free(g_apDataAudio[nCntSound]);
//			g_apDataAudio[nCntSound] = NULL;
//		}
//	}
//	
//	// マスターボイスの破棄
//	g_pMasteringVoice->DestroyVoice();
//	g_pMasteringVoice = NULL;
//	
//	if(g_pXAudio2)
//	{
//		// XAudio2オブジェクトの開放
//		g_pXAudio2->Release();
//		g_pXAudio2 = NULL;
//	}
//	
//	// COMライブラリの終了処理
//	CoUninitialize();
//}
//
////=============================================================================
//// セグメント再生(停止)
////=============================================================================
//HRESULT PlaySound(SOUND_LABEL label)
//{
//	XAUDIO2_VOICE_STATE xa2state;
//	XAUDIO2_BUFFER buffer;
//
//	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
//	buffer.AudioBytes = g_aSizeAudio[label];
//	buffer.pAudioData = g_apDataAudio[label];
//	buffer.Flags      = XAUDIO2_END_OF_STREAM;
//	buffer.LoopCount  = 0;
//
//	// 状態取得
//	g_apSourceVoice[label]->GetState(&xa2state);
//	if(xa2state.BuffersQueued != 0)
//	{// 再生中
//		// 一時停止
//		g_apSourceVoice[label]->Stop(0);
//
//		// オーディオバッファの削除
//		g_apSourceVoice[label]->FlushSourceBuffers();
//	}
//
//	// オーディオバッファの登録
//	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);
//
//	// 再生
//	g_apSourceVoice[label]->Start(0);
//
//	return S_OK;
//}
//
////=============================================================================
//// セグメント停止
////=============================================================================
//void StopSound(SOUND_LABEL label)
//{
//	XAUDIO2_VOICE_STATE xa2state;
//
//	// 状態取得
//	g_apSourceVoice[label]->GetState(&xa2state);
//	if(xa2state.BuffersQueued != 0)
//	{// 再生中
//		// 一時停止
//		g_apSourceVoice[label]->Stop(0);
//
//		// オーディオバッファの削除
//		g_apSourceVoice[label]->FlushSourceBuffers();
//	}
//}
//
////=============================================================================
//// セグメント停止
////=============================================================================
//void StopSound(void)
//{
//	// 一時停止
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		if(g_apSourceVoice[nCntSound])
//		{
//			// 一時停止
//			g_apSourceVoice[nCntSound]->Stop(0);
//		}
//	}
//}
//
////=============================================================================
//// チャンクのチェック
////=============================================================================
//HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
//{
//	HRESULT hr = S_OK;
//	DWORD dwRead;
//	DWORD dwChunkType;
//	DWORD dwChunkDataSize;
//	DWORD dwRIFFDataSize = 0;
//	DWORD dwFileType;
//	DWORD dwBytesRead = 0;
//	DWORD dwOffset = 0;
//	
//	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//	{// ファイルポインタを先頭に移動
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//	
//	while(hr == S_OK)
//	{
//		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
//		{// チャンクの読み込み
//			hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//
//		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
//		{// チャンクデータの読み込み
//			hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//
//		switch(dwChunkType)
//		{
//		case 'FFIR':
//			dwRIFFDataSize  = dwChunkDataSize;
//			dwChunkDataSize = 4;
//			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
//			{// ファイルタイプの読み込み
//				hr = HRESULT_FROM_WIN32(GetLastError());
//			}
//			break;
//
//		default:
//			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
//			{// ファイルポインタをチャンクデータ分移動
//				return HRESULT_FROM_WIN32(GetLastError());
//			}
//		}
//
//		dwOffset += sizeof(DWORD) * 2;
//		if(dwChunkType == format)
//		{
//			*pChunkSize         = dwChunkDataSize;
//			*pChunkDataPosition = dwOffset;
//
//			return S_OK;
//		}
//
//		dwOffset += dwChunkDataSize;
//		if(dwBytesRead >= dwRIFFDataSize)
//		{
//			return S_FALSE;
//		}
//	}
//	
//	return S_OK;
//}
//
////=============================================================================
//// チャンクデータの読み込み
////=============================================================================
//HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
//{
//	DWORD dwRead;
//	
//	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//	{// ファイルポインタを指定位置まで移動
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//
//	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
//	{// データの読み込み
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//	
//	return S_OK;
//}
//
