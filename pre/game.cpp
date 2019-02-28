//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "enemy.h"
#include "player.h"
#include "physics.h"
#include "shadow.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
#include "score.h"
#include "sound.h"
#include "debugproc.h"
#include "DebugTimer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMESCRNR_LABEL		"GameScene"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{

	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();


	// 地面の初期化
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), FILED_TIP_X, FILED_TIP_Z, FILED_SIZE_X, FILED_SIZE_Z);


	// 壁の初期化
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(-500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);

	// 壁(裏側用)
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(-500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, -6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);

	// 影の初期化
	InitShadow();

	InitEnemy();
	// プレイヤーの初期化
	InitPlayer();


	InitParticle();

	// 爆発の初期化
	InitExplosion();

	// エフェクトの初期化
	InitEffect();

	// ライフの初期化
	InitLife();

	// タイマーの初期化
	InitTimer();

	ResetTimer(20);

	// スコアの初期化
	InitScore();

	//InitDebugProc();

	RegisterDebugTimer(GAMESCRNR_LABEL);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// 地面の終了処理
	UninitMeshField();

	// 壁の終了処理
	//UninitMeshWall();

	// 影の終了処理
	UninitShadow();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	//パーティクルの終了処理
	UninitParticle();

	// 爆発の終了処理
	UninitExplosion();

	// エフェトの終了処理
	UninitEffect();

	// ライフの終了処理
	UninitLife();

	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();


	//UninitDebugProc();
	// BGM停止
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// カメラ更新
	UpdateCamera();

	// 地面処理の更新
	UpdateMeshField();

	//// 壁処理の更新
	UpdateMeshWall();

	// 影処理の更新
	UpdateShadow();

	// プレイヤー処理の更新
	CountDebugTimer(GAMESCRNR_LABEL, "Update Enemy");
	UpdateEnemy();
	CountDebugTimer(GAMESCRNR_LABEL, "Update Enemy");


	// プレイヤー処理の更新
	CountDebugTimer(GAMESCRNR_LABEL, "Update player");
	UpdatePlayer();
	CountDebugTimer(GAMESCRNR_LABEL, "Update player");


	// パーティクルの更新処理
	CountDebugTimer(GAMESCRNR_LABEL, "update particle");
	UpdateParticle();
	CountDebugTimer(GAMESCRNR_LABEL, "update particle");

	// 爆発処理の更新
	CountDebugTimer(GAMESCRNR_LABEL, "update explosion");
	UpdateExplosion();
	CountDebugTimer(GAMESCRNR_LABEL, "update explosion");

	// エフェクト処理の更新
	CountDebugTimer(GAMESCRNR_LABEL, "update effect");
	UpdateEffect();
	CountDebugTimer(GAMESCRNR_LABEL, "update effect");

	// ライフ処理の更新
	UpdateLife();

	// タイマー処理の更新
	UpdateTimer();

	// スコア処理の更新
	UpdateScore();


	//UpdateDebugProc();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();
	CountDebugTimer(GAMESCRNR_LABEL, "draw");

	// 地面処理の描画
	DrawMeshField();

	// 影処理の描画
	DrawShadow();

	// エネミー処理の更新
	DrawEnemy();

	// プレイヤー処理の描画
	DrawPlayer();


	//　パーティクルの描画処理
	DrawParticle();

	//// 壁処理の描画
	DrawMeshWall();

	// 爆発処理の描画
	DrawExplosion();
	
	// エフェクト処理の描画
	DrawEffect();

	// ライフ処理の描画
	DrawLife();

	// タイマー処理の描画
	DrawTimer();

	// スコア処理の描画
	DrawScore();
	CountDebugTimer(GAMESCRNR_LABEL, "draw");

	//DrawDebugProc();

	DrawDebugTimer(GAMESCRNR_LABEL);
}
