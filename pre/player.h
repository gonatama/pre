//=============================================================================
//
// �v���C���[���� [player.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"


#define PLAYER_SIZE_Y		(18.0f)			

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// ���݂̈ʒu
	D3DXVECTOR3			old;				// �ߋ��̈ʒu
	D3DXVECTOR3			move;				// �ړ���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			rotDest;			// �ړI�̌���
	D3DXVECTOR3			Dest;
	D3DXQUATERNION		Quaternion;


	float fRadius;			// ���a
	float jumpspeed=150.f;
	int nIdxShadow;			// �eID
	bool use;
	bool air;
	bool drop;
	bool ground;
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
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
