//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		"data/ENEMY/enemy_bar.x"		// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/o_shi_ma_i.png"	// 読み込むテクスチャファイル名

#define	RESULT_LOGO_POS_X	(240)		// リザルトロゴの位置(X座標)
#define	RESULT_LOGO_POS_Y	(320)		// リザルトロゴの位置(Y座標)

#define	RESULT_LOGO_WIDTH	(800)		// リザルトロゴの幅
#define	RESULT_LOGO_HEIGHT	(240)		// リザルトロゴの高さ

#define	COUNT_APPERA_RESULT	(60)		// リザルトロゴ出現までの待ち時間	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// 待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult_score(LPDIRECT3DDEVICE9 pDevice);
void SetColorResult_scoreLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureResult_result = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResult_result = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureResult_resultLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffResult_resultLogo = NULL;	// 頂点バッファインターフェースへのポインタ
int						CountAppearResult = 0;		// 出現までの待ち時間
float					AlphaResult = 0.0f;			// リザルトロゴのα値
int						CountWaitResult = 0;			// 待ち時間

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult_result(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CountAppearResult = 0;
	AlphaResult = 0.0f;
	CountWaitResult = 0;

	// 頂点情報の作成
	MakeVertexResult_score(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT,				// ファイルの名前
		&D3DTextureResult_result);		// 読み込むメモリー

// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT_LOGO,		// ファイルの名前
		&D3DTextureResult_resultLogo);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult_result(void)
{
	if (D3DTextureResult_result != NULL)
	{// テクスチャの開放
		D3DTextureResult_result->Release();
		D3DTextureResult_result = NULL;
	}

	if (D3DVtxBuffResult_result != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResult_result->Release();
		D3DVtxBuffResult_result = NULL;
	}

	if (D3DTextureResult_resultLogo != NULL)
	{// テクスチャの開放
		D3DTextureResult_resultLogo->Release();
		D3DTextureResult_resultLogo = NULL;
	}

	if (D3DVtxBuffResult_resultLogo != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffResult_resultLogo->Release();
		D3DVtxBuffResult_resultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult_result(void)
{

	if ((GetKeyboardTrigger(DIK_RETURN)) || (IsButtonPressed(0, BUTTON_START)))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		SetFade(FADE_OUT);
	}

	CountWaitResult++;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult_result(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult_score(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResultLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogo_score(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}
}