//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define FILED_TIP_X			(30)
#define FILED_TIP_Z			(150)
#define FILED_SIZE_X		(250.0f)
#define FILED_SIZE_Z		(250.0f)
#define WALL_Y				(100.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

#endif
