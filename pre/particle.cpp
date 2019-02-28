//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_HIT			"data/TEXTURE/hit_explosion.png"
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE			(2048)						// ビルボード最大数

#define	DISP_SHADOW				// 影の表示
//#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	int type;				// タイプ
	bool bUse;				// 使用しているかどうか
} PARTICLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorParticle(int nIdxParticle, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle[2];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldParticle;				// ワールドマトリックス

PARTICLE				g_aParticle[MAX_PARTICLE];		// パーティクルワーク
D3DXVECTOR3				g_posBase;						// ビルボード発生位置
float					g_fWidthBase = 5.0f;			// 基準の幅
float					g_fHeightBase = 10.0f;			// 基準の高さ
bool					g_bPause = false;				// ポーズON/OFF

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_PARTICLE,			// ファイルの名前
		&g_pD3DTextureParticle[0]);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_HIT,			// ファイルの名前
		&g_pD3DTextureParticle[1]);	// 読み込むメモリー

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		//g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	g_posBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if (g_pD3DTextureParticle != NULL)
	{// テクスチャの開放
		for (int i = 0; i < TEX_MAX; i++)
		{
			g_pD3DTextureParticle[i]->Release();
			g_pD3DTextureParticle[i] = NULL;
		}
	}

	if (g_pD3DVtxBuffParticle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffParticle->Release();
		g_pD3DVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	//D3DXVECTOR3 rotCamera;

	//// カメラの回転を取得
	//rotCamera = GetRotCamera();

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse)
			{// 使用中
				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
				//if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 2)
				//{// 着地した
				//	g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 2;
				//	g_aParticle[nCntParticle].move.y = -g_aParticle[nCntParticle].move.y * 0.75f;
				//}

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.015f;
				g_aParticle[nCntParticle].move.y -= 0.25f;
				g_aParticle[nCntParticle].move.z += (0.0f - g_aParticle[nCntParticle].move.z) * 0.015f;
			}
				g_aParticle[nCntParticle].nLife--;
				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					g_aParticle[nCntParticle].bUse = false;
					//DeleteShadow(g_aParticle[nCntParticle].nIdxShadow);
					//g_aParticle[nCntParticle].nIdxShadow = -1;
				}
				else
				{
					if (g_aParticle[nCntParticle].nLife <= 80)
					{
						g_aParticle[nCntParticle].col.r = 0.60f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle[nCntParticle].col.g = 0.70f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						g_aParticle[nCntParticle].col.b = 0.05f;
					}

					if (g_aParticle[nCntParticle].nLife <= 20)
					{
						// α値設定
						g_aParticle[nCntParticle].col.a -= 0.05f;
						if (g_aParticle[nCntParticle].col.a < 0.0f)
						{
							g_aParticle[nCntParticle].col.a = 0.0f;
						}
					}

					// 色の設定
					SetColorParticle(nCntParticle, g_aParticle[nCntParticle].col);
				}
			}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//// ライティングを無効に
	////pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// パーティクルっぽく設定をする


	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし



	//for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	//{
	//	if (g_aParticle[nCntParticle].bUse)
	//	{
	//		// ワールドマトリックスの初期化
	//		D3DXMatrixIdentity(&g_mtxWorldParticle);

	//		// ビューマトリックスを取得
	//		mtxView = GetMtxView();

	//		g_mtxWorldParticle._11 = mtxView._11;
	//		g_mtxWorldParticle._12 = mtxView._21;
	//		g_mtxWorldParticle._13 = mtxView._31;
	//		g_mtxWorldParticle._21 = mtxView._12;
	//		g_mtxWorldParticle._22 = mtxView._22;
	//		g_mtxWorldParticle._23 = mtxView._32;
	//		g_mtxWorldParticle._31 = mtxView._13;
	//		g_mtxWorldParticle._32 = mtxView._23;
	//		g_mtxWorldParticle._33 = mtxView._33;

	//		// スケールを反映
	//		D3DXMatrixScaling(&mtxScale, g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
	//		D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxScale);

	//		// 移動を反映
	//		D3DXMatrixTranslation(&mtxTranslate, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
	//		D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTranslate);

	//		// ワールドマトリックスの設定
	//		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

	//		// 頂点バッファをデバイスのデータストリームにバインド
	//		pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

	//		// 頂点フォーマットの設定
	//		pDevice->SetFVF(FVF_VERTEX_3D);

	//		// テクスチャの設定
	//		pDevice->SetTexture(0, g_pD3DTextureParticle);

	//		// ポリゴンの描画
	//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);
	//	}
	//}



	//// ライティングを有効に
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// 通常ブレンドに戻す
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle);
			 //ビューマトリックスを取得
			mtxView = GetMtxView();

			g_mtxWorldParticle._11 = mtxView._11;
			g_mtxWorldParticle._12 = mtxView._21;
			g_mtxWorldParticle._13 = mtxView._31;
			g_mtxWorldParticle._21 = mtxView._12;
			g_mtxWorldParticle._22 = mtxView._22;
			g_mtxWorldParticle._23 = mtxView._32;
			g_mtxWorldParticle._31 = mtxView._13;
			g_mtxWorldParticle._32 = mtxView._23;
			g_mtxWorldParticle._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureParticle[g_aParticle[nCntParticle].type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffParticle,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].bUse = true;

			// 頂点座標の設定
			SetVertexParticle(nCntParticle, fSizeX, fSizeY);

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			//// 影の設定
			//g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// 影の設定
			//if (g_aParticle[nCntParticle].nIdxShadow != -1)
			//{
			//	SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			//}
#endif

			break;
		}
	}

	return nIdxParticle;
}
