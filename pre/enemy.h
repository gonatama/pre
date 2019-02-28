#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "game.h"

#define	ENEMY_MAX		(6000)
#define ENEMY_HIT		(20.0f)
#define ENEMY_SIZE_Y	(18.0f)			


typedef struct
{

	D3DXVECTOR3			posEnemy;				// ���f���̈ʒu
	D3DXVECTOR3			rotEnemy;				// ���f���̌���(��])
	D3DXVECTOR3			oldEnemy;
	D3DXVECTOR3			sclEnemy;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			moveEnemy;				// �ړ���
	D3DXVECTOR3			Dest;
	D3DXQUATERNION		Quaternion;


	int					enemy_nIdxShadow;				// �eID
	float				enemy_fSizeShadow;				// �e�̃T�C�Y
	D3DXCOLOR			enemy_colShadow;				// �e�̐F

	D3DXVECTOR3			rotDestEnemy;				// �ړI�̌���
	float				vel = 0.0f;

	bool bUse;
	bool in;
	bool air;
	bool drop;
	bool ground;
	bool blown;
}ENEMY;


//*****************************************************************************
// �v���g�^�C�v�錾
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
