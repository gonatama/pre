//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"


#define PLAYER_SIZE_Y		(18.0f)			

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// 現在の位置
	D3DXVECTOR3			old;				// 過去の位置
	D3DXVECTOR3			move;				// 移動量
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			rotDest;			// 目的の向き
	D3DXVECTOR3			Dest;
	D3DXQUATERNION		Quaternion;


	float fRadius;			// 半径
	float jumpspeed=150.f;
	int nIdxShadow;			// 影ID
	bool use;
	bool air;
	bool drop;
	bool ground;
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);


//void DrawPlayer_shaft_r(void);
//void DrawPlayer_shaft_l(void);
//void DrawPlayer_wheel_r(void);
//void DrawPlayer_wheel_l(void);
//void DrawPlayer_enjin_r(void);
//void DrawPlayer_enjin_l(void);


D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);

//D3DXMATRIX *GetPlayerMatrix(void);

#endif
