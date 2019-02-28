//=============================================================================
//
// ���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "physics.h"
#include "input.h"
#include "shadow.h"
#include "sound.h"
#include "DebugTimer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ENEMY_MODEL		"data/ENEMY/enemy_bar.x"							// �ǂݍ��ރ��f����

//#define ENEMY_HEAD		"data/ENEMY/e_head.x"
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
//#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
//#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l
#define	ENEMY_MOVE		(4.0f)											// �ړ���
#define	ENEMY_ROTATE	(D3DX_PI * 0.02f)								// ��]��
//#define	NUM_LIGHT		(3)												// ���C�g�̐�

#define ENEMY_LABEL	"Enemy"
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void EnemyHoming(void);
void EnemySanten(void);
void EnemyQuaternion(void);
void EnemyBlow(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	pD3DTextureEnemy;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			pD3DXMeshEnemy;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		pD3DXBuffMatEnemy;	// �}�e���A�����ւ̃|�C���^
DWORD				nNumMatEnemy;			// �}�e���A�����̐�


//LPDIRECT3DTEXTURE9	pD3DTextureEnemy_head;		// �e�N�X�`���ւ̃|�C���^
//LPD3DXMESH			pD3DXMeshEnemy_head;		// ���b�V�����ւ̃|�C���^
//LPD3DXBUFFER		pD3DXBuffMatEnemy_head;	// �}�e���A�����ւ̃|�C���^
//DWORD				nNumMatEnemy_head;			// �}�e���A�����̐�
//

ENEMY				enemy[ENEMY_MAX];

//D3DXVECTOR3			posEnemy;				// ���f���̈ʒu
//D3DXVECTOR3			rotEnemy;				// ���f���̌���(��])
//D3DXVECTOR3			sclEnemy;				// ���f���̑傫��(�X�P�[��)

D3DXMATRIX			enemy_mtxWorld;				// ���[���h�}�g���b�N�X
D3DXMATRIX			mtxWorld_body;

static LPDIRECTSOUNDBUFFER8		g_pSE;						// SE�p�o�b�t�@


//D3DLIGHT9			aLight[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	RegisterDebugTimer(ENEMY_LABEL);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	//// �J�����̏�����
	//InitCamera();

	//// ���C�g�̏�����
	//InitLight();
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int j = rand() % 800;
		if (j % 2 == 0)
		{
			j = j * (-1);
		}
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		enemy->posEnemy = D3DXVECTOR3((float)j, 200.0f, 1.5f*(i));
		enemy->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//enemy->sclEnemy = D3DXVECTOR3((float)scl, (float)scl, (float)scl);
		enemy->sclEnemy = D3DXVECTOR3(1.0f, 1.0f,1.0f);


		enemy->moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->oldEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy->Dest = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXQuaternionIdentity(&enemy->Quaternion);

		enemy->bUse = true;

		pD3DTextureEnemy = NULL;
		pD3DXMeshEnemy = NULL;
		pD3DXBuffMatEnemy = NULL;
		nNumMatEnemy = 0;



		//pD3DXMeshEnemy_head = NULL;
		//pD3DXBuffMatEnemy_head = NULL;
		//nNumMatEnemy = 0;
		

		{
			SetPositionShadow(enemy->enemy_nIdxShadow, D3DXVECTOR3(enemy->posEnemy.x, 0.1f, enemy->posEnemy.z));

			float fSizeX = 20.0f + (enemy->posEnemy.y - 10.0f) * 0.05f;
			if (fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (enemy->posEnemy.y - 10.0f) * 0.05f;
			if (fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			SetVertexShadow(enemy->enemy_nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (enemy->posEnemy.y - 10.0f)) / 400.0f;
			if (colA < 0.0f)
			{
				colA = 0.0f;
			}
			SetColorShadow(enemy->enemy_nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
		}

	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(ENEMY_MODEL,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&pD3DXBuffMatEnemy,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&nNumMatEnemy,		// D3DXMATERIAL�\���̂̐�
		&pD3DXMeshEnemy)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X

	{
		return E_FAIL;
	}
	//�@���֌W

	g_pSE = LoadSound(SE_HIT);


	//if (FAILED(D3DXLoadMeshFromX(ENEMY_HEAD,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&pD3DXBuffMatEnemy_head,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&nNumMatEnemy_head,		// D3DXMATERIAL�\���̂̐�
	//	&pD3DXMeshEnemy_head)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X

	//{
	//	return E_FAIL;
	//}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&pD3DTextureEnemy);	// �ǂݍ��ރ������[
#endif
		// �e�̐���
	//enemy_nIdxShadow = CreateShadow(enemy->posEnemy, 25.0f, 25.0f);
	//enemy_fSizeShadow = 25.0f;
	//enemy_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	// �e�̈ʒu�ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
	void UninitEnemy(void)
	{
		if (pD3DTextureEnemy != NULL)
		{// �e�N�X�`���̊J��
			pD3DTextureEnemy->Release();
			pD3DTextureEnemy = NULL;
		}

		if (pD3DXMeshEnemy != NULL)
		{// ���b�V���̊J��
			pD3DXMeshEnemy->Release();
			pD3DXMeshEnemy = NULL;
		}

		if (pD3DXBuffMatEnemy != NULL)
		{// �}�e���A���̊J��
			pD3DXBuffMatEnemy->Release();
			pD3DXBuffMatEnemy = NULL;
		}
		if (g_pSE != NULL)
		{	// �e�N�X�`���̊J��
			g_pSE->Release();
			g_pSE = NULL;
		}

	}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
	
	CountDebugTimer(ENEMY_LABEL, "homing");
	EnemyHoming();
	CountDebugTimer(ENEMY_LABEL, "homing");

	CountDebugTimer(ENEMY_LABEL, "santen");
	EnemySanten();
	CountDebugTimer(ENEMY_LABEL, "santen");

	CountDebugTimer(ENEMY_LABEL, "quaternion");
	EnemyQuaternion();
	CountDebugTimer(ENEMY_LABEL, "quaternion");

	CountDebugTimer(ENEMY_LABEL, "blow");
	EnemyBlow();
	CountDebugTimer(ENEMY_LABEL, "blow");

	DrawDebugTimer(ENEMY_LABEL);


	//for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	//{
	//	int rand_rot;
	//	rand_rot = rand() % 360;
	//	if ((enemy->blown == false) && (enemy->bUse == true))
	//	{
	//		//homing
	//		if (rand_rot % 7 == 0)
	//		{
	//			homing(enemy);
	//		}
	//		enemy->posEnemy.x += enemy->moveEnemy.x;
	//		//enemy->posEnemy.y += enemy->moveEnemy.y;
	//		enemy->posEnemy.z += enemy->moveEnemy.z;


	//		//�O�_����
	//		if (enemy->posEnemy.y < Hitfield_Enemy(enemy))
	//		{
	//			enemy->posEnemy.y = Hitfield_Enemy(enemy) /*+ 18.62*/;
	//			enemy->air = false;
	//			enemy->drop = false;
	//			enemy->ground=true;


	//			enemy->vel = 0.0f;
	//		}
	//		else
	//		{
	//			enemy->air = true;
	//			enemy->drop = true;
	//			enemy->ground = false;
	//		}			
	//		if ((enemy->air == true) || (enemy->drop == true))
	//		{
	//			Gravity(enemy);
	//		}

	//		/************************************************************************************
	//		**�@�p������
	//		*************************************************************************************/
	//		if (enemy->ground == true)
	//		{

	//			D3DXVECTOR3 vx, nvx;
	//			D3DXQUATERNION quat;
	//			float len, angle;

	//			D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &enemy->Dest);

	//			len = D3DXVec3Length(&vx);
	//			D3DXVec3Normalize(&nvx, &vx);

	//			angle = asinf(len);

	//			D3DXQuaternionIdentity(&quat);
	//			D3DXQuaternionRotationAxis(&quat, &nvx, angle);
	//			D3DXQuaternionSlerp(&enemy->Quaternion, &enemy->Quaternion, &quat, 0.05f);
	//		}


	//		//if ((enemy->vel > -100.0f) && (enemy->posEnemy.y < Hitfield(enemy->posEnemy)))
	//		//{
	//		//	Repelling(enemy);
	//		//}





	//	}

	//	//blow
	//	if ((enemy->blown == true) && (enemy->bUse == true))
	//	{
	//		enemy->moveEnemy.x += 0.15f;
	//		enemy->moveEnemy.y += 0.25f;
	//		enemy->moveEnemy.z += 0.15f;

	//		enemy->posEnemy.x += enemy->moveEnemy.x;
	//		enemy->posEnemy.y += enemy->moveEnemy.y;
	//		enemy->posEnemy.z += enemy->moveEnemy.z;

	//		enemy->rotEnemy.x += rand_rot;
	//		enemy->rotEnemy.y += rand_rot;
	//		enemy->rotEnemy.z += rand_rot;

	//		SetParticle(enemy->posEnemy, enemy->moveEnemy, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20, 0);

	//		//SetParticle(enemy->posEnemy, move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20);
	//		
	//		D3DXVECTOR3		vec = enemy->moveEnemy - enemy->oldEnemy;
	//		float			len = D3DXVec3Length(&vec);
	//		if (len > 0.1f)
	//		{	// �r���{�[�h�Ƃ̓����蔻��

	//			if (hitCheckWall(enemy->posEnemy, enemy->oldEnemy) != 0)
	//			{
	//				// �������Ă���̂Ō��̈ʒu�ɖ߂�
	//				enemy->blown = false;
	//				enemy->bUse = false;
	//			}
	//		}
	//	}
	//	if (enemy->posEnemy.y > 1000.0f)
	//	{
	//		enemy->blown = false;
	//		enemy->bUse = false;
	//		//PlaySound(g_pSE, E_DS8_FLAG_NONE);

	//	}

	//}
}

void EnemyHoming()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{
			//homing
			if (rand_rot % 7 == 0)
			{
				homing(enemy);
			}
			enemy->posEnemy.x += enemy->moveEnemy.x;
			//enemy->posEnemy.y += enemy->moveEnemy.y;
			enemy->posEnemy.z += enemy->moveEnemy.z;
		}
	}
}

void EnemySanten(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{

			//�O�_����
			FLOAT height = Hitfield_Enemy(enemy);
			enemy->posEnemy.y = height;
			//if (enemy->posEnemy.y < height)
			//{
			//	enemy->posEnemy.y = height /*+ 18.62*/;
			//	enemy->air = false;
			//	enemy->drop = false;
			//	enemy->ground = true;


			//	enemy->vel = 0.0f;
			//}
			//else
			//{
			//	enemy->air = true;
			//	enemy->drop = true;
			//	enemy->ground = false;
			//}
			////if ((enemy->air == true) || (enemy->drop == true))
			////{
			////	Gravity(enemy);
			////}

		}
	}
}

void EnemyQuaternion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		if ((enemy->blown == false) && (enemy->bUse == true))
		{
			/************************************************************************************
			**�@�p������
			*************************************************************************************/
			if (enemy->ground == true)
			{

				D3DXVECTOR3 vx, nvx;
				D3DXQUATERNION quat;
				float len, angle;

				D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &enemy->Dest);

				len = D3DXVec3Length(&vx);
				D3DXVec3Normalize(&nvx, &vx);

				angle = asinf(len);

				D3DXQuaternionIdentity(&quat);
				D3DXQuaternionRotationAxis(&quat, &nvx, angle);
				D3DXQuaternionSlerp(&enemy->Quaternion, &enemy->Quaternion, &quat, 0.05f);
			}

		}
	}
}

void EnemyBlow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy(0);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		int rand_rot;
		rand_rot = rand() % 360;
		//blow
		if ((enemy->blown == true) && (enemy->bUse == true))
		{
			enemy->moveEnemy.x += 0.15f;
			enemy->moveEnemy.y += 0.25f;
			enemy->moveEnemy.z += 0.15f;

			enemy->posEnemy.x += enemy->moveEnemy.x;
			enemy->posEnemy.y += enemy->moveEnemy.y;
			enemy->posEnemy.z += enemy->moveEnemy.z;

			enemy->rotEnemy.x += rand_rot;
			enemy->rotEnemy.y += rand_rot;
			enemy->rotEnemy.z += rand_rot;

			SetParticle(enemy->posEnemy, enemy->moveEnemy, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 10, 0);

			//SetParticle(enemy->posEnemy, move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 20);

			D3DXVECTOR3		vec = enemy->moveEnemy - enemy->oldEnemy;
			float			len = D3DXVec3Length(&vec);
			if (len > 0.1f)
			{	// �r���{�[�h�Ƃ̓����蔻��

				if (hitCheckWall(enemy->posEnemy, enemy->oldEnemy) != 0)
				{
					// �������Ă���̂Ō��̈ʒu�ɖ߂�
					enemy->blown = false;
					enemy->bUse = false;
				}
			}
		}
		if (enemy->posEnemy.y > 1000.0f)
		{
			enemy->blown = false;
			enemy->bUse = false;
			PlaySound(g_pSE, E_DS8_FLAG_NONE);

		}

	}
}


//// �e�̈ʒu�ݒ�
//SetPositionShadow(enemy_nIdxShadow, D3DXVECTOR3(enemy->posEnemy.x, 0.1f, enemy->posEnemy.z));

//SetVertexShadow(enemy_nIdxShadow, enemy_fSizeShadow, enemy_fSizeShadow);
//SetColorShadow(enemy_nIdxShadow, enemy_colShadow);

//	//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", enemy->posEnemy.x, enemy->posEnemy.y, enemy->posEnemy.z);
//	//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", enemy->rotModel.y, enemy->rotDestModel.y);
//	//PrintDebugProc("\n");
//
//	//PrintDebugProc("*** ��s�@���� ***\n");
//	//PrintDebugProc("�O�ړ� : ��\n");
//	//PrintDebugProc("��ړ� : ��\n");
//	//PrintDebugProc("���ړ� : ��\n");
//	//PrintDebugProc("�E�ړ� : ��\n");
//	//PrintDebugProc("�㏸   : �h\n");
//	//PrintDebugProc("���~   : �j\n");
//	//PrintDebugProc("������ : LSHIFT\n");
//	//PrintDebugProc("�E���� : RSHIFT\n");
//
//}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate ,quatMatrixs, shadowMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	ENEMY *enemy = GetEnemy(0);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&enemy_mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, enemy->sclEnemy.x, enemy->sclEnemy.y, enemy->sclEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy->rotEnemy.y, enemy->rotEnemy.x, enemy->rotEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);

			//�N�H�[�^�j�I�����}�g���N�X
			D3DXMatrixRotationQuaternion(&quatMatrixs, &enemy->Quaternion);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &quatMatrixs);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, enemy->posEnemy.x, enemy->posEnemy.y, enemy->posEnemy.z);
			D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);



			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)nNumMatEnemy; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pD3DTextureEnemy);

				// �`��
				pD3DXMeshEnemy->DrawSubset(nCntMat);
			}

			//mtxWorld_body = enemy_mtxWorld;

			//DrawEnemy_Head();

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

////=============================================================================
//// �`�揈���@��
////=============================================================================
//void DrawEnemy_Head(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//	ENEMY *enemy = GetEnemy(0);
//
//	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
//	{
//		// ���[���h�}�g���b�N�X�̏�����
//		D3DXMatrixIdentity(&enemy_mtxWorld);
//
//		// �X�P�[���𔽉f
//		D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);
//
//		// ��]�𔽉f
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);
//
//		// �ړ��𔽉f
//		D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 0.0f);
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);
//
//
//		D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, GeEnemytMatrix());
//
//
//		// ���[���h�}�g���b�N�X�̐ݒ�
//		pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);
//
//		// ���݂̃}�e���A�����擾
//		pDevice->GetMaterial(&matDef);
//
//		// �}�e���A�����ɑ΂���|�C���^���擾
//		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatEnemy_head->GetBufferPointer();
//
//		for (int nCntMat = 0; nCntMat < (int)nNumMatEnemy_head; nCntMat++)
//		{
//			// �}�e���A���̐ݒ�
//			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//			// �e�N�X�`���̐ݒ�
//			pDevice->SetTexture(0, pD3DTextureEnemy);
//
//			// �`��
//			pD3DXMeshEnemy_head->DrawSubset(nCntMat);
//		}
//
//		// �}�e���A�����f�t�H���g�ɖ߂�
//		pDevice->SetMaterial(&matDef);
//
//	}
//}

/**********************************************************************************
** GetEnemy
***********************************************************************************/
ENEMY	*GetEnemy(int pno)
{
	return &enemy[pno];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionModel(void)
{
	return enemy->posEnemy;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationModel(void)
{
	return enemy->rotEnemy;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestModel(void)
{
	return enemy->rotDestEnemy;
}


/*********************************************************************************
**�z�[�~���O
************************************************************************************/
void homing(ENEMY *enemy)
{
	D3DXVECTOR3 gap,pos;
	float dot;
	D3DXVECTOR3 cross;

	//PLAYER *player = GetPlayer();
	pos=GetPositionPlayer();

	//enemy->moveEnemy +=
	gap = (pos - enemy->posEnemy);
	D3DXVec3Normalize(&gap, &gap);
	//	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
	//����
	dot = (enemy->posEnemy.x * pos.x) + (enemy->posEnemy.y *pos.y) + (enemy->posEnemy.z * pos.z);
	dot = acosf(dot);

	//�O��
	cross.x = (enemy->posEnemy.y * pos.z) - (enemy->posEnemy.z * pos.y);
	cross.y = (enemy->posEnemy.z * pos.x) - (enemy->posEnemy.x - pos.z);
	cross.z = (enemy->posEnemy.x * pos.y) - (enemy->posEnemy.y - pos.x);


	if (cross < 0)
	{
		cross = -cross;
	}

	enemy->rotDestEnemy = gap;

	//		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
//		if (player->rotPlayer.x < -D3DX_PI)
//		{
//			player->rotPlayer.x -= D3DX_PI * 1.5f;
//		}

	//enemy->posEnemy += dot * ENEMY_MOVE;

	enemy->moveEnemy = gap * ENEMY_MOVE;

	//enemy->moveEnemy.x -= sinf(D3DX_PI*cross.x)*(gap.x*ENEMY_MOVE);
	//enemy->moveEnemy.z -= cosf(D3DX_PI*cross.z)*(gap.z*ENEMY_MOVE);
	//player->rotDestPlayer.y = D3DX_PI + cwk->rotCameraP.y;


	// ��]
	//enemy->rotEnemy = cross;



}

//*******************************************************************************
//**
//*******************************************************************************
D3DXMATRIX *GeEnemytMatrix(void)
{
	return &mtxWorld_body;
}

/******************************************************************************
**
******************************************************************************/
void DeleteEnemy(int nIdxEnemy)
{
	if (nIdxEnemy >= 0 && nIdxEnemy < ENEMY_MAX)
	{
		DeleteShadow(enemy[nIdxEnemy].enemy_nIdxShadow);
		//enemy[nIdxEnemy].bUse = false;
		enemy[nIdxEnemy].blown = true;
	}
}


////=============================================================================
//// ����
////=============================================================================
//float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//
//	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
//
//
//	return(ans);
//}
//
//
////=============================================================================
//// �O��
////=============================================================================
//void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//	//#if 0
//
//	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
//	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
//	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
//	//#else
//		//�֐�
//		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);
//
//
//	//#endif
//
//}
//
//
////=============================================================================
//// �|���S���Ɛ����Ƃ̓����蔻��
//// p0  :�|���S���̒��_1
//// p1  :�|���S���̒��_2
//// p2  :�|���S���̒��_3
//// pos0:�n�_�i�ړ��O�j
//// pos1:�I�_�i�ړ���j
////=============================================================================
//int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		nor;		// �|���S���̖@��
//
//	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�B�����Đ��K�����Ă����B
//		D3DXVECTOR3		vec01 = p1 - p0;
//		D3DXVECTOR3		vec02 = p2 - p0;
//		crossProduct(&nor, &vec01, &vec02);
//		D3DXVec3Normalize(&nor, &nor);
//
//
//	}
//
//	{	// ����(�O�p�`�|���S��)�Ɛ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
//		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float a = dotProduct(&vec1, &nor);
//		float b = dotProduct(&vec2, &nor);
//		if ((a * b) > 0)
//		{
//			// �������Ă���\���͖���
//			return(0);
//		}
//	}
//
//
//	{	// �|���S���Ɛ����̌�_�����߂�
//		//D3DXVECTOR3		vec1 = pos0 - p0;
//		//D3DXVECTOR3		vec2 = pos0 - p0;
//		//float			d1;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
//		//float			d2;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
//		//float			a;					// ������
//		//D3DXVECTOR3		vec3;
//		//D3DXVECTOR3		p3;					// ��_
//
//
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float			d1 = fabs(dotProduct(&nor, &vec1));
//		float			d2 = fabs(dotProduct(&nor, &vec2));
//		float			a = d1 / (d1 + d2);
//		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
//		D3DXVECTOR3		p3 = p0 + vec3;
//
//
//
//
//		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�
//
//			// �|���S���̊e�ӂ̃x�N�g��
//			D3DXVECTOR3		v1 = p1 - p0;
//			D3DXVECTOR3		v2 = p2 - p1;
//			D3DXVECTOR3		v3 = p3 = p0;
//
//			// �e���_�ƌ�_�Ƃ̃x�N�g��
//			D3DXVECTOR3		v4 = p3 - p1;
//			D3DXVECTOR3		v5 = p3 - p2;
//			D3DXVECTOR3		v6 = p3 - p0;
//
//			// �e�ӂƂ̊O�ςŖ@�������߂�
//			D3DXVECTOR3		n1, n2, n3;
//			crossProduct(&n1, &v1, &v4);
//			crossProduct(&n2, &v2, &v5);
//			crossProduct(&n3, &v3, &v6);
//
//
//
//			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
//			if (dotProduct(&n1, &nor) < 0) return(0);
//			if (dotProduct(&n2, &nor) < 0) return(0);
//			if (dotProduct(&n3, &nor) < 0) return(0);
//
//
//
//		}
//	}
//
//	return(1);
//}
//
//
////=============================================================================
//// �r���{�[�h�Ƃ̓����蔻��
//// pos0:�n�_�i�ړ��O�j
//// pos1:�I�_�i�ړ���j
////=============================================================================
//int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	PLAYER *player = GetPlayer(0);
//	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���
//
//	D3DXVECTOR3		pos[4];
//	D3DXVECTOR3		nor;		// �|���S���̖@��
//
//								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//	//VERTEX_3D		*pVtx;
//	//D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//	//pVt(D3DXMATERIAL*)pD3DXBuffMatEnemy->GetBufferPointer();
//	//for (int i = 0; i < 1; i++, pVtx += 4)
//	//{	// �܂��́A�|���S���̒��_�����߂�
//
//	//	// ���_���W�̐ݒ�
//	//	pos[0] = pVtx[0].vtx;
//	//	pos[1] = pVtx[1].vtx;
//	//	pos[2] = pVtx[2].vtx;
//	//	pos[3] = pVtx[3].vtx;
//
//	//	// �������|���S���Ɛ����̓����蔻��
//	//	ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
//	//	if (ans != 0) break;
//
//	//	// �E�㑤�|���S���Ɛ����̓����蔻��
//	//	ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
//	//	if (ans != 0) break;
//
//	//}
//
//	// ���_�f�[�^���A�����b�N����
//	//D3DVtxBuffField->Unlock();
//
//
//
//
//	return(ans);
//
//}
//

