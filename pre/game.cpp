//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "enemy.h"
#include "player.h"
#include "physics.h"
#include "shadow.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
#include "score.h"
#include "sound.h"
#include "debugproc.h"
#include "DebugTimer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAMESCRNR_LABEL		"GameScene"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{

	// ���C�g�̏�����
	InitLight();

	// �J�����̏�����
	InitCamera();


	// �n�ʂ̏�����
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), FILED_TIP_X, FILED_TIP_Z, FILED_SIZE_X, FILED_SIZE_Z);


	// �ǂ̏�����
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(-500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, 16000.0f, 16000.0f);

	// ��(�����p)
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, 6000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(-500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(500.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);
	InitMeshWall(D3DXVECTOR3(0.0f, -1000.0f, -6000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 16000.0f, 16000.0f);

	// �e�̏�����
	InitShadow();

	InitEnemy();
	// �v���C���[�̏�����
	InitPlayer();


	InitParticle();

	// �����̏�����
	InitExplosion();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// ���C�t�̏�����
	InitLife();

	// �^�C�}�[�̏�����
	InitTimer();

	ResetTimer(20);

	// �X�R�A�̏�����
	InitScore();

	//InitDebugProc();

	RegisterDebugTimer(GAMESCRNR_LABEL);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// ���C�g�̏I������
	UninitLight();

	// �J�����̏I������
	UninitCamera();

	// �n�ʂ̏I������
	UninitMeshField();

	// �ǂ̏I������
	//UninitMeshWall();

	// �e�̏I������
	UninitShadow();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�g�̏I������
	UninitEffect();

	// ���C�t�̏I������
	UninitLife();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �X�R�A�̏I������
	UninitScore();


	//UninitDebugProc();
	// BGM��~
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	// �J�����X�V
	UpdateCamera();

	// �n�ʏ����̍X�V
	UpdateMeshField();

	//// �Ǐ����̍X�V
	UpdateMeshWall();

	// �e�����̍X�V
	UpdateShadow();

	// �v���C���[�����̍X�V
	CountDebugTimer(GAMESCRNR_LABEL, "Update Enemy");
	UpdateEnemy();
	CountDebugTimer(GAMESCRNR_LABEL, "Update Enemy");


	// �v���C���[�����̍X�V
	CountDebugTimer(GAMESCRNR_LABEL, "Update player");
	UpdatePlayer();
	CountDebugTimer(GAMESCRNR_LABEL, "Update player");


	// �p�[�e�B�N���̍X�V����
	CountDebugTimer(GAMESCRNR_LABEL, "update particle");
	UpdateParticle();
	CountDebugTimer(GAMESCRNR_LABEL, "update particle");

	// ���������̍X�V
	CountDebugTimer(GAMESCRNR_LABEL, "update explosion");
	UpdateExplosion();
	CountDebugTimer(GAMESCRNR_LABEL, "update explosion");

	// �G�t�F�N�g�����̍X�V
	CountDebugTimer(GAMESCRNR_LABEL, "update effect");
	UpdateEffect();
	CountDebugTimer(GAMESCRNR_LABEL, "update effect");

	// ���C�t�����̍X�V
	UpdateLife();

	// �^�C�}�[�����̍X�V
	UpdateTimer();

	// �X�R�A�����̍X�V
	UpdateScore();


	//UpdateDebugProc();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();
	CountDebugTimer(GAMESCRNR_LABEL, "draw");

	// �n�ʏ����̕`��
	DrawMeshField();

	// �e�����̕`��
	DrawShadow();

	// �G�l�~�[�����̍X�V
	DrawEnemy();

	// �v���C���[�����̕`��
	DrawPlayer();


	//�@�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//// �Ǐ����̕`��
	DrawMeshWall();

	// ���������̕`��
	DrawExplosion();
	
	// �G�t�F�N�g�����̕`��
	DrawEffect();

	// ���C�t�����̕`��
	DrawLife();

	// �^�C�}�[�����̕`��
	DrawTimer();

	// �X�R�A�����̕`��
	DrawScore();
	CountDebugTimer(GAMESCRNR_LABEL, "draw");

	//DrawDebugProc();

	DrawDebugTimer(GAMESCRNR_LABEL);
}
