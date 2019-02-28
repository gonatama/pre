#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "game.h"

#define	ENEMY_MAX		(6000)
#define ENEMY_HIT		(20.0f)
#define ENEMY_SIZE_Y	(18.0f)			


typedef struct
{

	D3DXVECTOR3			posEnemy;				// モデルの位置
	D3DXVECTOR3			rotEnemy;				// モデルの向き(回転)
	D3DXVECTOR3			oldEnemy;
	D3DXVECTOR3			sclEnemy;				// モデルの大きさ(スケール)
	D3DXVECTOR3			moveEnemy;				// 移動量
	D3DXVECTOR3			Dest;
	D3DXQUATERNION		Quaternion;


	int					enemy_nIdxShadow;				// 影ID
	float				enemy_fSizeShadow;				// 影のサイズ
	D3DXCOLOR			enemy_colShadow;				// 影の色

	D3DXVECTOR3			rotDestEnemy;				// 目的の向き
	float				vel = 0.0f;

	bool bUse;
	bool in;
	bool air;
	bool drop;
	bool ground;
	bool blown;
}ENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);


//void DrawEnemy_Head(void);

D3DXMATRIX *GeEnemytMatrix(void);

ENEMY	*GetEnemy(int pno);

void homing(ENEMY *enemy);

void DeleteEnemy(int nIdxEnemy);


#endif
#pragma once
