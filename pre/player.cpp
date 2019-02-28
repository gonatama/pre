//=============================================================================
//
// プレイヤー処理 [player.cp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "item.h"
#include "meshfield.h"
#include "meshwall.h"
#include "life.h"
#include "physics.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_PANJAN_CORE		"data/PLAYER/panjan_color.x"		// 読み込むモデル名
//#define PLAYER_PANJAN_SHAFT_R	"data/PLAYER/panjan_shaft_Right.x"
//#define PLAYER_PANJAN_SHAFT_L	"data/PLAYER/panjan_shaft_Left.x"
//#define PLAYER_PANJAN_WHEEL_R	"data/PLAYER/panjan_wheel_Right.x"
//#define PLAYER_PANJAN_WHEEL_L	"data/PLAYER/panjan_wheel_Left.x"
//#define PLAYER_PANJAN_ENJIN_R	"data/PLAYER/panjan_enjin_Right.x"
//#define PLAYER_PANJAN_ENJIN_L	"data/PLAYER/panjan_enjin_Left.x"
//
#define	PLAYER_RADIUS		(40.0f)						// 半径
#define PLAYER_RADIUS_BOM	(600.0f)					// 爆発半径
#define	VALUE_MOVE_PLAYER	(0.1f)						// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTexturePlayer;		// テクスチャ読み込み場所
LPD3DXMESH			MeshPlayer;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXMatBuffPlayer;		// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer;			// 属性情報の総数



//LPD3DXMESH			MeshPlayer_shaft_r;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_shaft_r;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_shaft_r;				// 属性情報の総数
//
//LPD3DXMESH			MeshPlayer_shaft_l;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_shaft_l;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_shaft_l;				// 属性情報の総数
//
//LPD3DXMESH			MeshPlayer_wheel_r;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_wheel_r;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_wheel_r;				// 属性情報の総数
//
//LPD3DXMESH			MeshPlayer_wheel_l;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_wheel_l;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_wheel_l;				// 属性情報の総数
//
//LPD3DXMESH			MeshPlayer_enjin_r;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_enjin_r;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_enjin_r;				// 属性情報の総数
//
//LPD3DXMESH			MeshPlayer_enjin_l;					// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXMatBuffPlayer_enjin_l;			// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_enjin_l;				// 属性情報の総数


D3DXMATRIX			mtxWorld;							// ワールドマトリックス
//D3DXMATRIX			mtxWorld_core;						// ワールドマトリックス(core)

PLAYER				player;								// プレイヤーワーク


D3DVECTOR			Speed_reset;
float				speed = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;

	speed = 0.0f;

	player.pos		= D3DXVECTOR3(0.0f, 300.0f, -2500.0f);
	player.old		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.scl		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	player.rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.Dest		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	player.fRadius = PLAYER_RADIUS;
	player.use = true;
	Speed_reset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXQuaternionIdentity(&player.Quaternion);



	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;
	NumMatPlayer = 0;

	//MeshPlayer_shaft_r = NULL;
	//D3DXMatBuffPlayer_shaft_r = NULL;
	//NumMatPlayer_shaft_r = 0;

	//MeshPlayer_shaft_l = NULL;
	//D3DXMatBuffPlayer_shaft_l = NULL;
	//NumMatPlayer_shaft_l = 0;

	//MeshPlayer_wheel_r = NULL;
	//D3DXMatBuffPlayer_wheel_r = NULL;
	//NumMatPlayer_wheel_r = 0;

	//MeshPlayer_wheel_l = NULL;
	//D3DXMatBuffPlayer_wheel_l = NULL;
	//NumMatPlayer_wheel_l = 0;

	//MeshPlayer_enjin_r = NULL;
	//D3DXMatBuffPlayer_enjin_r = NULL;
	//NumMatPlayer_enjin_r = 0;

	//MeshPlayer_enjin_l = NULL;
	//D3DXMatBuffPlayer_enjin_l = NULL;
	//NumMatPlayer_enjin_l = 0;



	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_CORE,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXMatBuffPlayer,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer,		// D3DXMATERIAL構造体の数
		&MeshPlayer)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_R,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_shaft_r,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_shaft_r,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_shaft_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_L,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_shaft_l,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_shaft_l,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_shaft_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_R,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_wheel_r,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_wheel_r,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_wheel_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_L,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_wheel_l,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_wheel_l,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_wheel_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_R,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_enjin_r,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_enjin_r,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_enjin_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_L,				// 読み込むモデルファイル名(Xファイル)
	//	D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
	//	pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
	//	NULL,					// 隣接性データを含むバッファへのポインタ
	//	&D3DXMatBuffPlayer_enjin_l,	// マテリアルデータを含むバッファへのポインタ
	//	NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
	//	&NumMatPlayer_enjin_l,		// D3DXMATERIAL構造体の数
	//	&MeshPlayer_enjin_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	//{
	//	return E_FAIL;
	//}



#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&D3DTextureplayer);	// 読み込むメモリー
#endif

	// 影の設定
	player.nIdxShadow = SetShadow(player.pos, player.fRadius * 2.0f, player.fRadius * 2.0f);		// 影の設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if(D3DTexturePlayer != NULL)
	{// テクスチャの開放
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}

	if(MeshPlayer != NULL)
	{// メッシュの開放
		MeshPlayer->Release();
		MeshPlayer = NULL;
	}

	if(D3DXMatBuffPlayer != NULL)
	{// マテリアルの開放
		D3DXMatBuffPlayer->Release();
		D3DXMatBuffPlayer = NULL;
	}

}

	//=============================================================================
	// 更新処理
	//=============================================================================
	void UpdatePlayer(void)
	{
		D3DXVECTOR3 rotCamera;
		player.old = player.pos;		// 元の位置

		float fDiffRotY;
		float ground = 0;
		// カメラの向き取得
		rotCamera = GetRotCamera();
		if(player.use==true)
		{
			if((GetKeyboardPress(DIK_LEFT))||(IsButtonPressed(0, BUTTON_LEFT)))
		{
			speed += 0.02f;

			player.rot.x -= VALUE_ROTATE_PLAYER;

				if((GetKeyboardPress(DIK_UP))||(IsButtonPressed(0,BUTTON_UP)))
				{// 左前移動
					//if (GetKeyboardRelease(DIK_UP))

					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;

					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}

				}
				else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
				{// 左後移動
					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;

					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}


				}
				else
				{// 左移動
					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
			
					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}

				}
		}
		else if((GetKeyboardPress(DIK_RIGHT)) || (IsButtonPressed(0, BUTTON_RIGHT)))
		{
			player.rot.x -= VALUE_ROTATE_PLAYER;
			speed += 0.02f;

			if((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_UP)))
			{// 右前移動
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
			}
			else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
			{// 右後移動
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
			}
			else
			{// 右移動
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;

			}
		}
		else if((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_UP)))
		{// 前移動
			speed += 0.02f;	

			player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.rot.x -= VALUE_ROTATE_PLAYER * speed;


			player.rotDest.y = D3DX_PI + rotCamera.y;
		
		}
		else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
		{// 後移動
			speed += 0.02f;

			player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.rot.x -= VALUE_ROTATE_PLAYER * speed;


			player.rotDest.y = rotCamera.y;
		}


		if( (GetKeyboardTrigger(DIK_Z)) || (IsButtonPressed(0, BUTTON_C)))
		{
		
			player.use = false;

		}

		// 目的の角度までの差分
		fDiffRotY = player.rotDest.y - player.rot.y;
		if(fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if(fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// 目的の角度まで慣性をかける
		player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if(player.rot.y > D3DX_PI)
		{
			player.rot.y -= D3DX_PI * 2.0f;
		}
		if(player.rot.y < -D3DX_PI)
		{
			player.rot.y += D3DX_PI * 2.0f;
		}


		if (speed >= 5.0f)
		{
			speed = 5.0f;
		}

		// 位置移動
		player.pos.x += player.move.x;
		player.pos.y += player.move.y;
		player.pos.z += player.move.z;




		ground = Hitfield(player.pos);

			//三点測量
			if (player.pos.y < ground)
			{
				player.ground = true;
				player.pos.y = ground /*+ 18.62*/;
				player.air = false;
				player.drop = false;
			SetVel();

			}
			else
			{
				player.ground = false;
				player.air = true;
				player.drop = true;
			}
		


		if ((player.move.y< 0.0f)&& player.pos.y < ground)
		{
			Repelling(&player);

		}

		if (player.ground == false)/*(player.air == true) || (player.drop == true))*/
		{
			Gravity(&player);
		}
		if (((GetKeyboardPress(DIK_SPACE)) || (IsButtonPressed(0, BUTTON_B))) && (player.air == false))
		{
			Jump(&player);
		}

		// 移動量に慣性をかける
		player.move.x += (0.0f - player.move.x) * RATE_MOVE_PLAYER;
		player.move.y += (0.0f - player.move.y) * RATE_MOVE_PLAYER;
		player.move.z += (0.0f - player.move.z) * RATE_MOVE_PLAYER;





		D3DXVECTOR3		vec = player.move - player.old;
		float			len = D3DXVec3Length(&vec);
		if (len > 0.1f)
		{	// ビルボードとの当たり判定

			if (hitCheckWall(player.pos, player.old) != 0)
			{
				// 当たっているので元の位置に戻す
				speed = speed/10;
				player.move = Speed_reset;
				player.pos = player.old;

				ChangeLife(-1);
			}
		}

		speed -= 0.003f;

		if (speed <= 0)
		{
			speed = 0;
		}

		// 影の位置設定
		{
			SetPositionShadow(player.nIdxShadow, D3DXVECTOR3(player.pos.x, ground, player.pos.z));

			float fSizeX = 20.0f + (player.pos.y - 10.0f) * 0.05f;
			if(fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (player.pos.y - 10.0f) * 0.05f;
			if(fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			SetVertexShadow(player.nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (player.pos.y - 10.0f)) / 400.0f;
			if(colA < 0.0f)
			{
				colA = 0.0f;
			}
			SetColorShadow(player.nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
		}


		if((player.move.x * player.move.x
		+ player.move.y * player.move.y
		+ player.move.z * player.move.z) > 1.0f)
		{
			D3DXVECTOR3 pos;

			//pos.x = player.pos.x + sinf(player.rot.y) * 10.0f;
			//pos.y = player.pos.y + 2.0f;
			//pos.z = player.pos.z + cosf(player.rot.y) * 10.0f;

			// エフェクトの設定
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
		}



		{
			ENEMY *enemy = GetEnemy(0);

			for (int i = 0; i < ENEMY_MAX; i++, enemy++)
			{

				if ((enemy->bUse == true) && (enemy->blown == false))
				{
					float fLength;

					fLength = (player.pos.x - enemy->posEnemy.x) * (player.pos.x - enemy->posEnemy.x)
						+ (player.pos.y - enemy->posEnemy.y) * (player.pos.y - enemy->posEnemy.y)
						+ (player.pos.z - enemy->posEnemy.z) * (player.pos.z - enemy->posEnemy.z);

					if (player.use == false)
					{
						player.fRadius = PLAYER_RADIUS_BOM;
					}
					if (fLength < (player.fRadius + ENEMY_HIT) * (player.fRadius + ENEMY_HIT))
					{
						//int j = 0;
						int repel_x = 0;
						int repel_z = 0;
						repel_x = rand() % 100;
						repel_z = rand() % 50;

						if (repel_x % 2 == 0)
						{
							repel_x = repel_x * (-1);
						}

						DeleteEnemy(i);
						SetParticle(enemy->posEnemy, Speed_reset, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 10,1);
						enemy->moveEnemy.x = player.move.x*repel_x;
						enemy->moveEnemy.z = repel_z;

						// スコア加算
						ChangeScore(100);

						// SE再生


					}
				}
		/************************************************************************************
		**　姿勢制御
		*************************************************************************************/
				if(player.ground==true)
				{
					
					D3DXVECTOR3 vx, nvx;
					D3DXQUATERNION quat;
					float len, angle;

					D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &player.Dest);

					len = D3DXVec3Length(&vx);
					D3DXVec3Normalize(&nvx, &vx);

					angle = asinf(len);

					D3DXQuaternionIdentity(&quat);
					D3DXQuaternionRotationAxis(&quat, &nvx, angle);
					D3DXQuaternionSlerp(&player.Quaternion, &player.Quaternion, &quat, 0.05f);
				}
			}
		}
		
			if (GetLife()==0)
			{
				player.use = false;
			}


			if (player.use == false)
			{
				player.pos.y += 20.0f;
				player.pos.z -= 100.0f;

				SetExplosion(player.pos, 300.0f, 300.f);
				SetFade(FADE_OUT);

			}
		}

		//PrintDebugProc("[飛行機の位置  ：(%f : %f : %f)]\n", player.pos.x, player.pos.y, player.pos.z);
		//PrintDebugProc("[飛行機の向き  ：(%f) < 目的の向き:(%f) >]\n", player.rot.y, player.rotDest.y);
		//PrintDebugProc("[ジャンプ判定：(%d)\n", player.air);
		//PrintDebugProc("[落ちる判定：(%d)\n", player.drop);
		//PrintDebugProc("[vel  ：(%d)\n", Getvel());
		//PrintDebugProc("[vel  ：(%d)\n", Getvel());

		//PrintDebugProc("\n");

		//PrintDebugProc("*** 飛行機操作 ***\n");
		//PrintDebugProc("前移動 : ↑\n");
		//PrintDebugProc("後移動 : ↓\n");
		//PrintDebugProc("左移動 : ←\n");
		//PrintDebugProc("右移動 : →\n");
		//PrintDebugProc("上昇   : Ｉ\n");
		//PrintDebugProc("下降   : Ｋ\n");
		//PrintDebugProc("左旋回 : LSHIFT\n");
		//PrintDebugProc("右旋回 : RSHIFT\n");

	}

//=============================================================================
// CORE(親)の描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, quatMatrixs, shadowMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	if (player.use == true)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, 1.0f,1.0f,1.0f/*player.scl.x, player.scl.y, player.scl.z*/);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//クォータニオン→マトリクス
		D3DXMatrixRotationQuaternion(&quatMatrixs, &player.Quaternion);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &quatMatrixs);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);




		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)NumMatPlayer; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTexturePlayer);

			// 描画
			MeshPlayer->DrawSubset(nCntMat);
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);

		//mtxWorld_core = mtxWorld;


		//
		//DrawPlayer_shaft_r();
		//DrawPlayer_shaft_l();
		//DrawPlayer_wheel_r();
		//DrawPlayer_wheel_l();
		//DrawPlayer_enjin_r();
		//DrawPlayer_enjin_l();
		
	}
}
////********************************************************************************************************
////***	シャフト（右）の描画処理
////********************************************************************************************************
//void DrawPlayer_shaft_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	//親のマトリクスをかける
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_shaft_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_r; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_shaft_r->DrawSubset(nCntMat);
//	}
//	//mtxWorldplayer_body = mtxWorldplayer;
//
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	シャフト（左）の描画処理
////********************************************************************************************************
//void DrawPlayer_shaft_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//	//親のマトリクスをかける
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_shaft_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_l; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_shaft_l->DrawSubset(nCntMat);
//	}
//	//mtxWorldplayer_body = mtxWorldplayer;
//
//	mtxWorld_core = mtxWorld;
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	車輪（右）の描画処理
////********************************************************************************************************
//void DrawPlayer_wheel_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_wheel_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_r; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_wheel_r->DrawSubset(nCntMat);
//	}
//
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	車輪（左）の描画処理
////********************************************************************************************************
//void DrawPlayer_wheel_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_wheel_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_l; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_wheel_l->DrawSubset(nCntMat);
//	}
//
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	エンジン（右）の描画処理
////********************************************************************************************************
//void DrawPlayer_enjin_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_enjin_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_r; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_enjin_r->DrawSubset(nCntMat);
//	}
//
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	エンジン（左）の描画処理
////********************************************************************************************************
//void DrawPlayer_enjin_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// スケールを反映
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// 回転を反映
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// 移動を反映
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアル情報に対するポインタを取得
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_enjin_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_l; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// 描画
//		MeshPlayer_enjin_l->DrawSubset(nCntMat);
//	}
//
//	// マテリアルをデフォルトに戻す
//	pDevice->SetMaterial(&matDef);
//}

//=============================================================================
// プレイヤーを取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &player;
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return player.pos;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player.rot;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return player.rotDest;
}

//=============================================================================
// 移動量取得
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return player.move;
}
////*******************************************************************************
////**
////*******************************************************************************
//D3DXMATRIX *GetPlayerMatrix(void)
//{
//	return &mtxWorld_core;
//}
//
