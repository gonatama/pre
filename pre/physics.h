#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "main.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
// マクロ定義


#define GRAVITY			(50.0f)				//重力加速度
#define UNIT_TIME		(1/20.0f)			//単位時間(1/20[sec]
#define REPELLING		(-0.8f)				//反発係数
#define	RATE_REGIST		(0.035f)			// 抵抗係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



/*******************************************************************************
*player
********************************************************************************/
void Jump(PLAYER *player);
void Gravity(PLAYER *player);
void Repelling(PLAYER *player);

//void Jump(ENEMY *enemy);
void Gravity(ENEMY *enemy);
void Repelling(ENEMY *enemy);

/**********************************************************************************
*グローバル変数
***********************************************************************************/
void SetVel(void);
float Getvel(void);


#endif
#pragma once
