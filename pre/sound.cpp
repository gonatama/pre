#include "sound.h"

// ���܂��Ȃ�
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

// �T�E���h�t�@�C���̃p�X�ih�̒ʂ��i���o�[�ƍ��킹�邱�Ɓj
const TCHAR* c_soundFilename[] = {
	_T("data/BGM/bgm_game.wav"),
	_T("data/SE/explosion.wav"),
	_T("data/SE/hit.wav"),
	_T("data/SE/fire_start.wav"),
	_T("data/SE/enter.wav"),
};

// �O���[�o���ϐ�
IDirectSound8 *g_pDirectSound = NULL;	// �T�E���h�C���^�[�t�F�[�X

// �T�E���h�̏�����
// hWnd:�E�B���h�E�n���h��
HRESULT InitSound(HWND hWnd)
{
	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;

	// �������x���ݒ� 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	return S_OK;
}

// ��Еt��
void UninitSound()
{
	if (g_pDirectSound != NULL)
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}

// �T�E���h�̃��[�h
// no:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
LPDIRECTSOUNDBUFFER8 LoadSound(int no)
{
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;	// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;						// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;						// MMIO�n���h��
	MMIOINFO mmioInfo;						// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID pBlock;				// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;				// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if (!hMmio)
		return NULL;

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// �L�[���[�h�����ƂɌ���

	if (mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���

	if (mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// �����������Ƃɓǂݍ���

	if (size != formatChunk.cksize) 		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.�t�@�C����͇B �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���

	if (mmRes != MMSYSERR_NOERROR) 		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���
	char *pData = new char[dataChunk.cksize];					// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// �f�[�^��ǂݍ���

	if (size != dataChunk.cksize)  		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		delete[] pData;
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);			// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// ���o�����瑍���o�b�t�@���̂Ă�
	pBaseBuffer->Release();

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
	delete[] pData;					// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return pBuffer;
}

// ����炷
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
// flag   :1(E_DS8_FLAG_LOOP)�Ȃ烋�[�v�Đ�
void PlaySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag/*=0*/)
{	// ��������炷�̂ŁA�ŏ�����炵�����ꍇ��SetCurrentPosition(0)�����邱��
	pBuffer->Play(0, 0, flag);
}

// �����~�߂�
void StopSound(LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)	// ���Ă�����
	{
		pBuffer->Stop();	// �Ӗ��I�ɂ�Pause�ɂȂ�B
	}
}

// �Đ������ǂ������ׂ�
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
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
//// �T�E���h���� [sound.cpp]
//// Author : AKIRA TANAKA
////
////=============================================================================
//#include "sound.h"
//
////*****************************************************************************
//// �p�����[�^�\���̒�`
////*****************************************************************************
//typedef struct
//{
//	char *pFilename;	// �t�@�C����
//	bool bLoop;			// ���[�v���邩�ǂ���
//} PARAM;
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
//HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
//IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
//IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
//BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
//DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
//
//// �e���f�ނ̃p�����[�^
//PARAM g_aParam[SOUND_LABEL_MAX] =
//{
//	{"data/BGM/bgm_title.ogg", true},		// BGM0
//	{"data/BGM/bgm001.wav", true},			// BGM1
//	{"data/SE/shot000.wav", false},			// �e���ˉ�
//	{"data/SE/explosion000.wav", false},	// ������
//	{"data/SE/coin000.wav", false},			// �R�C����
//};
//
////=============================================================================
//// ������
////=============================================================================
//HRESULT InitSound(HWND hWnd)
//{
//	HRESULT hr;
//
//	// COM���C�u�����̏�����
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	// XAudio2�I�u�W�F�N�g�̍쐬
//	hr = XAudio2Create(&g_pXAudio2, 0);
//	if(FAILED(hr))
//	{
//		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);
//
//		// COM���C�u�����̏I������
//		CoUninitialize();
//		return E_FAIL;
//	}
//	
//	// �}�X�^�[�{�C�X�̐���
//	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
//	if(FAILED(hr))
//	{
//		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
//
//		if(g_pXAudio2)
//		{
//			// XAudio2�I�u�W�F�N�g�̊J��
//			g_pXAudio2->Release();
//			g_pXAudio2 = NULL;
//		}
//
//		// COM���C�u�����̏I������
//		CoUninitialize();
//
//		return E_FAIL;
//	}
//
//	// �T�E���h�f�[�^�̏�����
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		HANDLE hFile;
//		DWORD dwChunkSize = 0;
//		DWORD dwChunkPosition = 0;
//		DWORD dwFiletype;
//		WAVEFORMATEXTENSIBLE wfx;
//		XAUDIO2_BUFFER buffer;
//
//		// �o�b�t�@�̃N���A
//		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
//		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
//
//		// �T�E���h�f�[�^�t�@�C���̐���
//		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
//		if(hFile == INVALID_HANDLE_VALUE)
//		{
//			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
//			return HRESULT_FROM_WIN32(GetLastError());
//		}
//		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//		{// �t�@�C���|�C���^��擪�Ɉړ�
//			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
//			return HRESULT_FROM_WIN32(GetLastError());
//		}
//	
//		// WAVE�t�@�C���̃`�F�b�N
//		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		if(dwFiletype != 'EVAW')
//		{
//			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//	
//		// �t�H�[�}�b�g�`�F�b�N
//		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//
//		// �I�[�f�B�I�f�[�^�ǂݍ���
//		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
//		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//	
//		// �\�[�X�{�C�X�̐���
//		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
//		if(FAILED(hr))
//		{
//			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
//			return S_FALSE;
//		}
//
//		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
//		buffer.AudioBytes = g_aSizeAudio[nCntSound];
//		buffer.pAudioData = g_apDataAudio[nCntSound];
//		buffer.Flags      = XAUDIO2_END_OF_STREAM;
//		buffer.LoopCount  = 0;
//
//		// �I�[�f�B�I�o�b�t�@�̓o�^
//		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void UninitSound(void)
//{
//	// �ꎞ��~
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		if(g_apSourceVoice[nCntSound])
//		{
//			// �ꎞ��~
//			g_apSourceVoice[nCntSound]->Stop(0);
//	
//			// �\�[�X�{�C�X�̔j��
//			g_apSourceVoice[nCntSound]->DestroyVoice();
//			g_apSourceVoice[nCntSound] = NULL;
//	
//			// �I�[�f�B�I�f�[�^�̊J��
//			free(g_apDataAudio[nCntSound]);
//			g_apDataAudio[nCntSound] = NULL;
//		}
//	}
//	
//	// �}�X�^�[�{�C�X�̔j��
//	g_pMasteringVoice->DestroyVoice();
//	g_pMasteringVoice = NULL;
//	
//	if(g_pXAudio2)
//	{
//		// XAudio2�I�u�W�F�N�g�̊J��
//		g_pXAudio2->Release();
//		g_pXAudio2 = NULL;
//	}
//	
//	// COM���C�u�����̏I������
//	CoUninitialize();
//}
//
////=============================================================================
//// �Z�O�����g�Đ�(��~)
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
//	// ��Ԏ擾
//	g_apSourceVoice[label]->GetState(&xa2state);
//	if(xa2state.BuffersQueued != 0)
//	{// �Đ���
//		// �ꎞ��~
//		g_apSourceVoice[label]->Stop(0);
//
//		// �I�[�f�B�I�o�b�t�@�̍폜
//		g_apSourceVoice[label]->FlushSourceBuffers();
//	}
//
//	// �I�[�f�B�I�o�b�t�@�̓o�^
//	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);
//
//	// �Đ�
//	g_apSourceVoice[label]->Start(0);
//
//	return S_OK;
//}
//
////=============================================================================
//// �Z�O�����g��~
////=============================================================================
//void StopSound(SOUND_LABEL label)
//{
//	XAUDIO2_VOICE_STATE xa2state;
//
//	// ��Ԏ擾
//	g_apSourceVoice[label]->GetState(&xa2state);
//	if(xa2state.BuffersQueued != 0)
//	{// �Đ���
//		// �ꎞ��~
//		g_apSourceVoice[label]->Stop(0);
//
//		// �I�[�f�B�I�o�b�t�@�̍폜
//		g_apSourceVoice[label]->FlushSourceBuffers();
//	}
//}
//
////=============================================================================
//// �Z�O�����g��~
////=============================================================================
//void StopSound(void)
//{
//	// �ꎞ��~
//	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
//	{
//		if(g_apSourceVoice[nCntSound])
//		{
//			// �ꎞ��~
//			g_apSourceVoice[nCntSound]->Stop(0);
//		}
//	}
//}
//
////=============================================================================
//// �`�����N�̃`�F�b�N
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
//	{// �t�@�C���|�C���^��擪�Ɉړ�
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//	
//	while(hr == S_OK)
//	{
//		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
//		{// �`�����N�̓ǂݍ���
//			hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//
//		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
//		{// �`�����N�f�[�^�̓ǂݍ���
//			hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//
//		switch(dwChunkType)
//		{
//		case 'FFIR':
//			dwRIFFDataSize  = dwChunkDataSize;
//			dwChunkDataSize = 4;
//			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
//			{// �t�@�C���^�C�v�̓ǂݍ���
//				hr = HRESULT_FROM_WIN32(GetLastError());
//			}
//			break;
//
//		default:
//			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
//			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
//// �`�����N�f�[�^�̓ǂݍ���
////=============================================================================
//HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
//{
//	DWORD dwRead;
//	
//	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//
//	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
//	{// �f�[�^�̓ǂݍ���
//		return HRESULT_FROM_WIN32(GetLastError());
//	}
//	
//	return S_OK;
//}
//
