//=============================================================================
//
// モデル処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "physics.h"
#include "input.h"
#include "shadow.h"
#include "sound.h"
#include "DebugTimer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_MODEL		"data/ENEMY/enemy_bar.x"							// 読み込むモデル名

//#define ENEMY_HEAD		"data/ENEMY/e_head.x"
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
//#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
//#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値
#define	ENEMY_MOVE		(4.0f)											// 移動量
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)								// 回転量
//#define	NUM_LIGHT		(3)												// ライトの数

#define ENEMY_LABEL	"Enemy"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void EnemyHoming(void);
void EnemySanten(void);
void EnemyQuaternion(void);
void EnemyBlow(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	pD3DTextureEnemy;		// テクスチャへのポインタ
LPD3DXMESH			pD3DXMeshEnemy;		// メッシュ情報へのポインタ
LPD3DXBUFFER		pD3DXBuffMatEnemy;	// マテリアル情報へのポインタ
DWORD				nNumMatEnemy;			// マテリアル情報の数


//LPDIRECT3DTEXTURE9	pD3DTextureEnemy_head;		// テクスチャへのポインタ
//LPD3DXMESH			pD3DXMeshEnemy_head;		// メッシュ情報へのポインタ
//LPD3DXBUFFER		pD3DXBuffMatEnemy_head;	// マテリアル情報へのポインタ
//DWORD				nNumMatEnemy_head;			// マテリアル情報の数
//

ENEMY				enemy[ENEMY_MAX];

//D3DXVECTOR3			posEnemy;				// モデルの位置
//D3DXVECTOR3			rotEnemy;				// モデルの向き(回転)
//D3DXVECTOR3			sclEnemy;				// モデルの大きさ(スケール)

D3DXMATRIX			enemy_mtxWorld;				// ワールドマトリックス
D3DXMATRIX			mtxWorld_body;

static LPDIRECTSOUNDBUFFER8		g_pSE;						// SE用バッファ


//D3DLIGHT9			aLight[NUM_LIGHT];	// ライト情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	RegisterDebugTimer(ENEMY_LABEL);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	//// カメラの初期化
	//InitCamera();

	//// ライトの初期化
	//InitLight();
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int j = rand() % 800;
		if (j % 2 == 0)
		{
			j = j * (-1);
		}
		// 位置・回転・スケールの初期設定
		enemy->posEnemy = D3DXVECTOR3((float)j, 200.0f, 1.5f*(i));
		enemy->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//enemy->sclEnemy = D3DXVECTOR3((float)scl, (float)scl, (float)scl);
		enemy->sclEnemy = D3DXVECTOR3(1.0f, 1.0f,1.0f);


		enemy->moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->oldEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->Dest = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXQuaternionIdentity(&enemy->Quaternion);

		enemy->bUse = true;

		pD3DTextureEnemy = NULL;
		pD3DXMeshEnemy = NULL;
		pD3DXBuffMatEnemy = NULL;
		nNumMatEnemy = 0;



		//pD3DXMeshEnemy_head = NULL;
		//pD3DXBuffMatEnemy_head = NULL;
		//nNumMatEnemy = 0;
		

		{
			SetPositionShadow(enemy->enemy_nIdxShadow, D3DXVECTOR3(enemy->posEnemy.x, 0.1f, enemy->posEnemy.z));

			float fSizeX = 20.0f + (enemy->posEnemy.y - 10.0f) * 0.05f;
			if (fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (enemy->posEnemy.y - 10.0f) * 0.05f;
			if (fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			SetVertexShadow(enemy->enemy_nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (enemy->posEnemy.y - 10.0f)) / 400.0f;
			if (colA < 0.0f)
			{
				colA = 0.0f;
			}
			SetColorShadow(enemy->enemy_nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
		}

	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(ENEMY_MODEL,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&pD3DXBuffMatEnemy,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&nNumMatEnemy,		// D3DXMATERIAL構造体の数
		&pD3DXMeshEnemy)))	// ID3DXMeshインターフェイスへのポインタのアドレス

	{
		return E_FAIL;
	}
	//　音関係

	g_pSE = LoadSound(SE_HIT);


	//if (FAILED(D3DXLoadMeshFromX(ENEMY_HEAD,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&pD3DXBuffMatEnemy_head,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&nNumMatEnemy_head,		// D3DXMATERIAL構造体の数
	//	&pD3DXMeshEnemy_head)))	// ID3DXMeshインターフェイスへのポインタのアドレス

	//{
	//	return E_FAIL;
	//}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&pD3DTextureEnemy);	// 読み込むメモリー
#endif
		// 影の生成
	//enemy_nIdxShadow = CreateShadow(enemy->posEnemy, 25.0f, 25.0f);
	//enemy_fSizeShadow = 25.0f;
	//enemy_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	// 影の位置設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
	void UninitEnemy(void)
	{
		if (pD3DTextureEnemy != NULL)
		{// テクスチャの開放
			pD3DTextureEnemy->Release();
			pD3DTextureEnemy = NULL;
		}

		if (pD3DXMeshEnemy != NULL)
		{// メッシュの開放
			pD3DXMeshEnemy->Release();
			pD3DXMeshEnemy = NULL;
		}

		if (pD3DXBuffMatEnemy != NULL)
		{// マテリアルの開放
			pD3DXBuffMatEnemy->Release();
			pD3DXBuffMatEnemy = NULL;
		}
		if (g_pSE != NULL)
		{	// テクスチャの開放
			g_pSE->Release();
			g_pSE = NULL;
		}

	}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
	
	CountDebugTimer(ENEMY_LABEL, "homing");
	EnemyHoming();
	CountDebugTimer(ENEMY_LABEL, "homing");

	CountDebugTimer(ENEMY_LABEL, "santen");
	EnemySanten();
	CountDebugTimer(ENEMY_LABEL, "santen");

	CountDebugTimer(ENEMY_LABEL, "quaternion");
	EnemyQuaternion();
	CountDebugTimer(ENEMY_LABEL, "quaternion");

	CountDebugTimer(ENEMY_LABEL, "blow");
	EnemyBlow();
	CountDebugTimer(ENEMY_LABEL, "blow");

	DrawDebugTimer(ENEMY_LABEL);


	//for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	//{
	//	int rand_rot;
	//	rand_rot = rand() % 360;
	//	if ((enemy->blown == false) && (enemy->bUse == true))
	//	{
	//		//homing
	//		if (rand_rot % 7 == 0)
	//		{
	//			homing(enemy);
	//		}
	//		enemy->posEnemy.x += enemy->moveEnemy.x;
	//		//enemy->posEnemy.y += enemy->moveEnemy.y;
	//		enemy->posEnemy.z += enemy->moveEnemy.z;


	//		//三点測量
	//		if (enemy->posEnemy.y < Hitfield_Enemy(enemy))
	//		{
	//			enemy->posEnemy.y = Hitfield_Enemy(enemy) /*+ 18.62*/;
	//			enemy->air = false;
	//			enemy->drop = false;
	//			enemy->ground=true;


	//			enemy->vel = 0.0f;
	//		}
	//		else
	//		{
	//			enemy->air = true;
	//			enemy->drop = true;
	//			enemy->ground = false;
	//		}			
	//		if ((enemy->air == true) || (enemy->drop == true))
	//		{
	//			Gravity(enemy);
	//		}

	//		/************************************************************************************
	//		**　姿勢制御
	//		*************************************************************************************/
	//		if (enemy->ground == true)
	//		{

	//			D3DXVECTOR3 vx, nvx;
	//			D3DXQUATERNION quat;
	//			float len, angle;

	//			D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &enemy->Dest);

	//			len = D3DXVec3Length(&vx);
	//			D3DXVec3Normalize(&nvx, &vx);

	//			angle = asinf(len);

	//			D3DXQuaternionIdentity(&quat);
	//			D3DXQuaternionRotationAxis(&quat, &nvx, angle);
	//			D3DXQuaternionSlerp(&enemy->Quaternion, &enemy->Quaternion, &quat, 0.05f);
	//		}


	//		//if ((enemy->vel > -100.0f) && (enemy->posEnemy.y < Hitfield(enemy->posEnemy)))
	//		//{
	//		//	Repelling(enemy);
	//		//}





	//	}

	//	//blow
	//	if ((enemy->blown == true) && (enemy->bUse == true))
	//	{
	//		enemy->moveEnemy.x += 0.15f;
	//		enemy->moveEnemy.y += 0.25f;
	//		enemy->moveEnemy.z += 0.15f;

	//		enemy->posEnemy.x += enemy->moveEnemy.x;
	//		enemy->posEnemy.y += enemy->moveEnemy.y;
	//		enemy->posEnemy.z += enemy->moveEnemy.z;

	//		enemy->rotEnemy.x += rand_rot;
	//		enemy->rotEnemy.y += rand_rot;
	//		enemy->rotEnemy.z += rand_rot;

	//		SetParticle(enemy->posEnemy, enemy->moveEnemy, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20, 0);

	//		//SetParticle(enemy->posEnemy, move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20);
	//		
	//		D3DXVECTOR3		vec = enemy->moveEnemy - enemy->oldEnemy;
	//		float			len = D3DXVec3Length(&vec);
	//		if (len > 0.1f)
	//		{	// ビルボードとの当たり判定

	//			if (hitCheckWall(enemy->posEnemy, enemy->oldEnemy) != 0)
	//			{
	//				// 当たっているので元の位置に戻す
	//				enemy->blown = false;
	//				enemy->bUse = false;
	//			}
	//		}
	//	}
	//	if (enemy->posEnemy.y > 1000.0f)
	//	{
	//		enemy->blown = false;
	//		enemy->bUse = false;
	//		//PlaySound(g_pSE, E_DS8_FLAG_NONE);

	//	}

	//}
}

void EnemyHoming()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{
			//homing
			if (rand_rot % 7 == 0)
			{
				homing(enemy);
			}
			enemy->posEnemy.x += enemy->moveEnemy.x;
			//enemy->posEnemy.y += enemy->moveEnemy.y;
			enemy->posEnemy.z += enemy->moveEnemy.z;
		}
	}
}

void EnemySanten(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{

			//三点測量
			FLOAT height = Hitfield_Enemy(enemy);
			enemy->posEnemy.y = height;
			//if (enemy->posEnemy.y < height)
			//{
			//	enemy->posEnemy.y = height /*+ 18.62*/;
			//	enemy->air = false;
			//	enemy->drop = false;
			//	enemy->ground = true;


			//	enemy->vel = 0.0f;
			//}
			//else
			//{
			//	enemy->air = true;
			//	enemy->drop = true;
			//	enemy->ground = false;
			//}
			////if ((enemy->air == true) || (enemy->drop == true))
			////{
			////	Gravity(enemy);
			////}

		}
	}
}

void EnemyQuaternion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{
			/************************************************************************************
			**　姿勢制御
			*************************************************************************************/
			if (enemy->ground == true)
			{

				D3DXVECTOR3 vx, nvx;
				D3DXQUATERNION quat;
				float len, angle;

				D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &enemy->Dest);

				len = D3DXVec3Length(&vx);
				D3DXVec3Normalize(&nvx, &vx);

				angle = asinf(len);

				D3DXQuaternionIdentity(&quat);
				D3DXQuaternionRotationAxis(&quat, &nvx, angle);
				D3DXQuaternionSlerp(&enemy->Quaternion, &enemy->Quaternion, &quat, 0.05f);
			}

		}
	}
}

void EnemyBlow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		//blow
		if ((enemy->blown == true) && (enemy->bUse == true))
		{
			enemy->moveEnemy.x += 0.15f;
			enemy->moveEnemy.y += 0.25f;
			enemy->moveEnemy.z += 0.15f;

			enemy->posEnemy.x += enemy->moveEnemy.x;
			enemy->posEnemy.y += enemy->moveEnemy.y;
			enemy->posEnemy.z += enemy->moveEnemy.z;

			enemy->rotEnemy.x += rand_rot;
			enemy->rotEnemy.y += rand_rot;
			enemy->rotEnemy.z += rand_rot;

			SetParticle(enemy->posEnemy, enemy->moveEnemy, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 10, 0);

			//SetParticle(enemy->posEnemy, move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20);

			D3DXVECTOR3		vec = enemy->moveEnemy - enemy->oldEnemy;
			float			len = D3DXVec3Length(&vec);
			if (len > 0.1f)
			{	// ビルボードとの当たり判定

				if (hitCheckWall(enemy->posEnemy, enemy->oldEnemy) != 0)
				{
					// 当たっているので元の位置に戻す
					enemy->blown = false;
					enemy->bUse = false;
				}
			}
		}
		if (enemy->posEnemy.y > 1000.0f)
		{
			enemy->blown = false;
			enemy->bUse = false;
			PlaySound(g_pSE, E_DS8_FLAG_NONE);

		}

	}
}


//// 影の位置設定
//SetPositionShadow(enemy_nIdxShadow, D3DXVECTOR3(enemy->posEnemy.x, 0.1f, enemy->posEnemy.z));

//SetVertexShadow(enemy_nIdxShadow, enemy_fSizeShadow, enemy_fSizeShadow);
//SetColorShadow(enemy_nIdxShadow, enemy_colShadow);

//	//PrintDebugProc("[飛行機の位置  ：(%f : %f : %f)]\n", enemy->posEnemy.x, enemy->posEnemy.y, enemy->posEnemy.z);
//	//PrintDebugProc("[飛行機の向き  ：(%f) < 目的の向き:(%f) >]\n", enemy->rotModel.y, enemy->rotDestModel.y);
//	//PrintDebugProc("\n");
//
//	//PrintDebugProc("*** 飛行機操作 ***\n");
//	//PrintDebugProc("前移動 : ↑\n");
//	//PrintDebugProc("後移動 : ↓\n");
//	//PrintDebugProc("左移動 : ←\n");
//	//PrintDebugProc("右移動 : →\n");
//	//PrintDebugProc("上昇   : Ｉ\n");
//	//PrintDebugProc("下降   : Ｋ\n");
//	//PrintDebugProc("左旋回 : LSHIFT\n");
//	//PrintDebugProc("右旋回 : RSHIFT\n");
//
//}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate ,quatMatrixs, shadowMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	ENEMY *enemy = GetEnemy(0);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&enemy_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, enemy->sclEnemy.x, enemy->sclEnemy.y, enemy->sclEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy->rotEnemy.y, enemy->rotEnemy.x, enemy->rotEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);

			//クォータニオン→マトリクス
			D3DXMatrixRotationQuaternion(&quatMatrixs, &enemy->Quaternion);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &quatMatrixs);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, enemy->posEnemy.x, enemy->posEnemy.y, enemy->posEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);



			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)nNumMatEnemy; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, pD3DTextureEnemy);

				// 描画
				pD3DXMeshEnemy->DrawSubset(nCntMat);
			}

			//mtxWorld_body = enemy_mtxWorld;

			//DrawEnemy_Head();

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

////=============================================================================
//// 描画処理　頭
////=============================================================================
//void DrawEnemy_Head(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//	ENEMY *enemy = GetEnemy(0);
//
//	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
//	{
//		// ワールドマトリックスの初期化
//		D3DXMatrixIdentity(&enemy_mtxWorld);
//
//		// スケールを反映
//		D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);
//
//		// 回転を反映
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);
//
//		// 移動を反映
//		D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 0.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);
//
//
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, GeEnemytMatrix());
//
//
//		// ワールドマトリックスの設定
//		pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);
//
//		// 現在のマテリアルを取得
//		pDevice->GetMaterial(&matDef);
//
//		// マテリアル情報に対するポインタを取得
//		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatEnemy_head->GetBufferPointer();
//
//		for (int nCntMat = 0; nCntMat < (int)nNumMatEnemy_head; nCntMat++)
//		{
//			// マテリアルの設定
//			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//			// テクスチャの設定
//			pDevice->SetTexture(0, pD3DTextureEnemy);
//
//			// 描画
//			pD3DXMeshEnemy_head->DrawSubset(nCntMat);
//		}
//
//		// マテリアルをデフォルトに戻す
//		pDevice->SetMaterial(&matDef);
//
//	}
//}

/**********************************************************************************
** GetEnemy
***********************************************************************************/
ENEMY	*GetEnemy(int pno)
{
	return &enemy[pno];
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionModel(void)
{
	return enemy->posEnemy;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationModel(void)
{
	return enemy->rotEnemy;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestModel(void)
{
	return enemy->rotDestEnemy;
}


/*********************************************************************************
**ホーミング
************************************************************************************/
void homing(ENEMY *enemy)
{
	D3DXVECTOR3 gap,pos;
	float dot;
	D3DXVECTOR3 cross;

	//PLAYER *player = GetPlayer();
	pos=GetPositionPlayer();

	//enemy->moveEnemy +=
	gap = (pos - enemy->posEnemy);
	D3DXVec3Normalize(&gap, &gap);
	//	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
	//内積
	dot = (enemy->posEnemy.x * pos.x) + (enemy->posEnemy.y *pos.y) + (enemy->posEnemy.z * pos.z);
	dot = acosf(dot);

	//外積
	cross.x = (enemy->posEnemy.y * pos.z) - (enemy->posEnemy.z * pos.y);
	cross.y = (enemy->posEnemy.z * pos.x) - (enemy->posEnemy.x - pos.z);
	cross.z = (enemy->posEnemy.x * pos.y) - (enemy->posEnemy.y - pos.x);


	if (cross < 0)
	{
		cross = -cross;
	}

	enemy->rotDestEnemy = gap;

	//		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
//		if (player->rotPlayer.x < -D3DX_PI)
//		{
//			player->rotPlayer.x -= D3DX_PI * 1.5f;
//		}

	//enemy->posEnemy += dot * ENEMY_MOVE;

	enemy->moveEnemy = gap * ENEMY_MOVE;

	//enemy->moveEnemy.x -= sinf(D3DX_PI*cross.x)*(gap.x*ENEMY_MOVE);
	//enemy->moveEnemy.z -= cosf(D3DX_PI*cross.z)*(gap.z*ENEMY_MOVE);
	//player->rotDestPlayer.y = D3DX_PI + cwk->rotCameraP.y;


	// 回転
	//enemy->rotEnemy = cross;



}

//*******************************************************************************
//**
//*******************************************************************************
D3DXMATRIX *GeEnemytMatrix(void)
{
	return &mtxWorld_body;
}

/******************************************************************************
**
******************************************************************************/
void DeleteEnemy(int nIdxEnemy)
{
	if (nIdxEnemy >= 0 && nIdxEnemy < ENEMY_MAX)
	{
		DeleteShadow(enemy[nIdxEnemy].enemy_nIdxShadow);
		//enemy[nIdxEnemy].bUse = false;
		enemy[nIdxEnemy].blown = true;
	}
}


////=============================================================================
//// 内積
////=============================================================================
//float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//
//	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
//
//
//	return(ans);
//}
//
//
////=============================================================================
//// 外積
////=============================================================================
//void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//	//#if 0
//
//	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
//	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
//	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
//	//#else
//		//関数
//		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);
//
//
//	//#endif
//
//}
//
//
////=============================================================================
//// ポリゴンと線分との当たり判定
//// p0  :ポリゴンの頂点1
//// p1  :ポリゴンの頂点2
//// p2  :ポリゴンの頂点3
//// pos0:始点（移動前）
//// pos1:終点（移動後）
////=============================================================================
//int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		nor;		// ポリゴンの法線
//
//	{	// ポリゴンの外積をとって法線を求める。そして正規化しておく。
//		D3DXVECTOR3		vec01 = p1 - p0;
//		D3DXVECTOR3		vec02 = p2 - p0;
//		crossProduct(&nor, &vec01, &vec02);
//		D3DXVec3Normalize(&nor, &nor);
//
//
//	}
//
//	{	// 平面(三角形ポリゴン)と線分の内積とって衝突している可能性を調べる
//		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float a = dotProduct(&vec1, &nor);
//		float b = dotProduct(&vec2, &nor);
//		if ((a * b) > 0)
//		{
//			// 当たっている可能性は無い
//			return(0);
//		}
//	}
//
//
//	{	// ポリゴンと線分の交点を求める
//		//D3DXVECTOR3		vec1 = pos0 - p0;
//		//D3DXVECTOR3		vec2 = pos0 - p0;
//		//float			d1;					// 内分比を求める為の点とポリゴンとの距離
//		//float			d2;					// 内分比を求める為の点とポリゴンとの距離
//		//float			a;					// 内分比
//		//D3DXVECTOR3		vec3;
//		//D3DXVECTOR3		p3;					// 交点
//
//
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float			d1 = fabs(dotProduct(&nor, &vec1));
//		float			d2 = fabs(dotProduct(&nor, &vec2));
//		float			a = d1 / (d1 + d2);
//		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
//		D3DXVECTOR3		p3 = p0 + vec3;
//
//
//
//
//		{	// 求めた交点がポリゴンの中にあるか調べる
//
//			// ポリゴンの各辺のベクトル
//			D3DXVECTOR3		v1 = p1 - p0;
//			D3DXVECTOR3		v2 = p2 - p1;
//			D3DXVECTOR3		v3 = p3 = p0;
//
//			// 各頂点と交点とのベクトル
//			D3DXVECTOR3		v4 = p3 - p1;
//			D3DXVECTOR3		v5 = p3 - p2;
//			D3DXVECTOR3		v6 = p3 - p0;
//
//			// 各辺との外積で法線を求める
//			D3DXVECTOR3		n1, n2, n3;
//			crossProduct(&n1, &v1, &v4);
//			crossProduct(&n2, &v2, &v5);
//			crossProduct(&n3, &v3, &v6);
//
//
//
//			// それぞれのベクトルの向きを内積でチェック
//			if (dotProduct(&n1, &nor) < 0) return(0);
//			if (dotProduct(&n2, &nor) < 0) return(0);
//			if (dotProduct(&n3, &nor) < 0) return(0);
//
//
//
//		}
//	}
//
//	return(1);
//}
//
//
////=============================================================================
//// ビルボードとの当たり判定
//// pos0:始点（移動前）
//// pos1:終点（移動後）
////=============================================================================
//int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	PLAYER *player = GetPlayer(0);
//	int				ans = 0;	// 一先ず当たっていないをセットしておく
//
//	D3DXVECTOR3		pos[4];
//	D3DXVECTOR3		nor;		// ポリゴンの法線
//
//								// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	//VERTEX_3D		*pVtx;
//	//D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//	//pVt(D3DXMATERIAL*)pD3DXBuffMatEnemy->GetBufferPointer();
//	//for (int i = 0; i < 1; i++, pVtx += 4)
//	//{	// まずは、ポリゴンの頂点を求める
//
//	//	// 頂点座標の設定
//	//	pos[0] = pVtx[0].vtx;
//	//	pos[1] = pVtx[1].vtx;
//	//	pos[2] = pVtx[2].vtx;
//	//	pos[3] = pVtx[3].vtx;
//
//	//	// 左下側ポリゴンと線分の当たり判定
//	//	ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
//	//	if (ans != 0) break;
//
//	//	// 右上側ポリゴンと線分の当たり判定
//	//	ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
//	//	if (ans != 0) break;
//
//	//}
//
//	// 頂点データをアンロックする
//	//D3DVtxBuffField->Unlock();
//
//
//
//
//	return(ans);
//
//}
//

