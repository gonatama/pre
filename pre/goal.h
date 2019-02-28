#pragma once
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"
#include "game.h"
typedef struct
{

	D3DXVECTOR3			posGoal;				// ���f���̈ʒu
	D3DXVECTOR3			rotGoal;				// ���f���̌���(��])
	D3DXVECTOR3			sclGoal;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			moveGoal;				// �ړ���
	D3DXVECTOR3			rotDestGoal;				// �ړI�̌���


}GOAL;

#define GOAL_MAX		(1)

//*****************************************************************************
// �v���g�^�C�v�錾
//******1111***********************************************************************
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);




GOAL	*GetGoal(int pno);


#endif
#pragma once
