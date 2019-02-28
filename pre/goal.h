#pragma once
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"
#include "game.h"
typedef struct
{

	D3DXVECTOR3			posGoal;				// モデルの位置
	D3DXVECTOR3			rotGoal;				// モデルの向き(回転)
	D3DXVECTOR3			sclGoal;				// モデルの大きさ(スケール)
	D3DXVECTOR3			moveGoal;				// 移動量
	D3DXVECTOR3			rotDestGoal;				// 目的の向き


}GOAL;

#define GOAL_MAX		(1)

//*****************************************************************************
// プロトタイプ宣言
//******1111***********************************************************************
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);




GOAL	*GetGoal(int pno);


#endif
#pragma once
