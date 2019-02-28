#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "main.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
// �}�N����`


#define GRAVITY			(50.0f)				//�d�͉����x
#define UNIT_TIME		(1/20.0f)			//�P�ʎ���(1/20[sec]
#define REPELLING		(-0.8f)				//�����W��
#define	RATE_REGIST		(0.035f)			// ��R�W��

//*****************************************************************************
// �v���g�^�C�v�錾
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
*�O���[�o���ϐ�
***********************************************************************************/
void SetVel(void);
float Getvel(void);


#endif
#pragma once
