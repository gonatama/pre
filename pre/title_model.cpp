//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "title_model.h"
#include "input.h"
#include "camera.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_PLAYER		"data/PLAYER/Panjandrum.x"	

#define SPEED					(1.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************

LPDIRECT3DTEXTURE9	pD3DTextureTitle_Model;		// テクスチャへのポインタ
LPD3DXMESH			pD3DXMeshTitle_Model;		// メッシュ情報へのポインタ
LPD3DXBUFFER		pD3DXBuffMatTitle_Model;	// マテリアル情報へのポインタ
DWORD				nNumMatTitle_Model;			// マテリアル情報の数

D3DXVECTOR3			Title_model_pos;				// モデルの位置
D3DXVECTOR3			Title_model_rot;				// モデルの向き(回転)
D3DXVECTOR3			Title_model_scl;				// モデルの大きさ(スケール)
D3DXVECTOR3			Title_model_move;				// 移動量
D3DXVECTOR3			Title_model_rotDest;				// 目的の向き


D3DXMATRIX			Title_mtxWorld;				// ワールドマトリックス
float				title_speed = 0.1f;
bool				enter;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle_Model(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	Title_model_pos = D3DXVECTOR3(0.0f, 20.0f, 1000.0f);		// モデルの位置
	Title_model_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの向き(回転)
	Title_model_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// モデルの大きさ(スケール)
	Title_model_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	Title_model_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き

		// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(TITLE_PLAYER,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&pD3DXBuffMatTitle_Model,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&nNumMatTitle_Model,		// D3DXMATERIAL構造体の数
		&pD3DXMeshTitle_Model)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}
	return S_OK;


}

//*******************************************************************************
// 終了処理
//*******************************************************************************
void UninitTitle_Model(void)
{
	if (pD3DTextureTitle_Model != NULL)
	{// テクスチャの開放
		pD3DTextureTitle_Model->Release();
		pD3DTextureTitle_Model = NULL;
	}

	if (pD3DXMeshTitle_Model != NULL)
	{// メッシュの開放
		pD3DXMeshTitle_Model->Release();
		pD3DXMeshTitle_Model = NULL;
	}

	if (pD3DXBuffMatTitle_Model != NULL)
	{// マテリアルの開放
		pD3DXBuffMatTitle_Model->Release();
		pD3DXBuffMatTitle_Model = NULL;
	}

}

//*********************************************************************************
// 更新処理
//*********************************************************************************
void UpdateTitle_Model(void)
{

	D3DXVECTOR3 rotCamera = GetRotCamera();
	if (enter == false)
	{
		Title_model_move.x -= sinf(rotCamera.y) * SPEED *title_speed;
		Title_model_move.z -= cosf(rotCamera.y) * SPEED *title_speed;
		Title_model_rot.x += SPEED * 3;



		Title_model_rotDest.y = rotCamera.y;

		Title_model_pos += Title_model_move;


		if (Title_model_pos.z < -250.f)
		{
			int i = rand() % 300;
			if (i % 2 == 0)
			{
				i = i * (-1);
			}
			Title_model_pos.x = (float)i;
			Title_model_pos.z = 1000.0f + title_speed;
		}

		title_speed += 0.002f;
		if (title_speed > 0.1f)
		{
			title_speed = 0.0f;
		}
	}
	else
	{

	}
}

//****************************************************************************************
// 描画処理
//****************************************************************************************
void DrawTitle_Model(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Title_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, Title_model_scl.x, Title_model_scl.y, Title_model_scl.z);
	D3DXMatrixMultiply(&Title_mtxWorld, &Title_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Title_model_rot.y, Title_model_rot.x, Title_model_rot.z);
	D3DXMatrixMultiply(&Title_mtxWorld, &Title_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, Title_model_pos.x, Title_model_pos.y, Title_model_pos.z);
	D3DXMatrixMultiply(&Title_mtxWorld, &Title_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &Title_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatTitle_Model->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatTitle_Model; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pD3DTextureTitle_Model);

		// 描画
		pD3DXMeshTitle_Model->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);


}
