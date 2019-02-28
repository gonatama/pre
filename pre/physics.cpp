
/*****************************************************************
**physics.cpp
*****************************************************************/
#include "input.h"
#include "physics.h"
#include "meshfield.h"
/***********************************
*�O���[�o���ϐ�
************************************/

float vel = 0.0f;

/*********************************
*�d��(player)
**********************************/
void Gravity(PLAYER *player)
{

		vel -= GRAVITY * UNIT_TIME;
		player->pos.y += 0.5f * vel * UNIT_TIME;

}
/*********************************
*�W�����v(player)
*********************************/
void Jump(PLAYER *player)
{
	player->air = true;
	player->drop = true;
	vel += player->jumpspeed;

}
/********************************
*����
*********************************/
void Repelling(PLAYER *player)
{
	vel = vel * REPELLING;
}

/*********************************
*�d��(enemy)
**********************************/
void Gravity(ENEMY *enemy)
{

	enemy->vel -= GRAVITY * UNIT_TIME;
	enemy->posEnemy.y += 0.5f * enemy->vel * UNIT_TIME;

}
///*********************************
//*�W�����v(player)
//*********************************/
//void Jump(ENEMY *enemy)
//{
//	enemy->air = true;
//	enemy->drop = true;
//	vel += enemy->jumpspeed;
//
//}
/********************************
*����
*********************************/
void Repelling(ENEMY *enemy)
{
	enemy->vel = enemy->vel * REPELLING;
}

/************************************
*SetVel
* Vel��0�ɖ߂��B
*************************************/
void SetVel(void)
{
	vel = 0;
}


float Getvel(void)
{
	return vel;
}
