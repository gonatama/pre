//=============================================================================
//
// �v���C���[���� [player.cp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "item.h"
#include "meshfield.h"
#include "meshwall.h"
#include "life.h"
#include "physics.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_PANJAN_CORE		"data/PLAYER/panjan_color.x"		// �ǂݍ��ރ��f����
//#define PLAYER_PANJAN_SHAFT_R	"data/PLAYER/panjan_shaft_Right.x"
//#define PLAYER_PANJAN_SHAFT_L	"data/PLAYER/panjan_shaft_Left.x"
//#define PLAYER_PANJAN_WHEEL_R	"data/PLAYER/panjan_wheel_Right.x"
//#define PLAYER_PANJAN_WHEEL_L	"data/PLAYER/panjan_wheel_Left.x"
//#define PLAYER_PANJAN_ENJIN_R	"data/PLAYER/panjan_enjin_Right.x"
//#define PLAYER_PANJAN_ENJIN_L	"data/PLAYER/panjan_enjin_Left.x"
//
#define	PLAYER_RADIUS		(40.0f)						// ���a
#define PLAYER_RADIUS_BOM	(600.0f)					// �������a
#define	VALUE_MOVE_PLAYER	(0.1f)						// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTexturePlayer;		// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			MeshPlayer;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXMatBuffPlayer;		// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer;			// �������̑���



//LPD3DXMESH			MeshPlayer_shaft_r;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_shaft_r;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_shaft_r;				// �������̑���
//
//LPD3DXMESH			MeshPlayer_shaft_l;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_shaft_l;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_shaft_l;				// �������̑���
//
//LPD3DXMESH			MeshPlayer_wheel_r;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_wheel_r;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_wheel_r;				// �������̑���
//
//LPD3DXMESH			MeshPlayer_wheel_l;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_wheel_l;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_wheel_l;				// �������̑���
//
//LPD3DXMESH			MeshPlayer_enjin_r;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_enjin_r;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_enjin_r;				// �������̑���
//
//LPD3DXMESH			MeshPlayer_enjin_l;					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXMatBuffPlayer_enjin_l;			// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_enjin_l;				// �������̑���


D3DXMATRIX			mtxWorld;							// ���[���h�}�g���b�N�X
//D3DXMATRIX			mtxWorld_core;						// ���[���h�}�g���b�N�X(core)

PLAYER				player;								// �v���C���[���[�N


D3DVECTOR			Speed_reset;
float				speed = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;

	speed = 0.0f;

	player.pos		= D3DXVECTOR3(0.0f, 300.0f, -2500.0f);
	player.old		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.scl		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	player.rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player.Dest		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	player.fRadius = PLAYER_RADIUS;
	player.use = true;
	Speed_reset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXQuaternionIdentity(&player.Quaternion);



	D3DTexturePlayer = NULL;
	MeshPlayer = NULL;
	D3DXMatBuffPlayer = NULL;
	NumMatPlayer = 0;

	//MeshPlayer_shaft_r = NULL;
	//D3DXMatBuffPlayer_shaft_r = NULL;
	//NumMatPlayer_shaft_r = 0;

	//MeshPlayer_shaft_l = NULL;
	//D3DXMatBuffPlayer_shaft_l = NULL;
	//NumMatPlayer_shaft_l = 0;

	//MeshPlayer_wheel_r = NULL;
	//D3DXMatBuffPlayer_wheel_r = NULL;
	//NumMatPlayer_wheel_r = 0;

	//MeshPlayer_wheel_l = NULL;
	//D3DXMatBuffPlayer_wheel_l = NULL;
	//NumMatPlayer_wheel_l = 0;

	//MeshPlayer_enjin_r = NULL;
	//D3DXMatBuffPlayer_enjin_r = NULL;
	//NumMatPlayer_enjin_r = 0;

	//MeshPlayer_enjin_l = NULL;
	//D3DXMatBuffPlayer_enjin_l = NULL;
	//NumMatPlayer_enjin_l = 0;



	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_CORE,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXMatBuffPlayer,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer,		// D3DXMATERIAL�\���̂̐�
		&MeshPlayer)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_shaft_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_shaft_r,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_shaft_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_shaft_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_shaft_l,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_shaft_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_wheel_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_wheel_r,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_wheel_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_wheel_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_wheel_l,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_wheel_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_enjin_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_enjin_r,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_enjin_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
	//	D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
	//	pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//	NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	&D3DXMatBuffPlayer_enjin_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
	//	NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
	//	&NumMatPlayer_enjin_l,		// D3DXMATERIAL�\���̂̐�
	//	&MeshPlayer_enjin_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	//{
	//	return E_FAIL;
	//}



#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&D3DTextureplayer);	// �ǂݍ��ރ������[
#endif

	// �e�̐ݒ�
	player.nIdxShadow = SetShadow(player.pos, player.fRadius * 2.0f, player.fRadius * 2.0f);		// �e�̐ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if(D3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}

	if(MeshPlayer != NULL)
	{// ���b�V���̊J��
		MeshPlayer->Release();
		MeshPlayer = NULL;
	}

	if(D3DXMatBuffPlayer != NULL)
	{// �}�e���A���̊J��
		D3DXMatBuffPlayer->Release();
		D3DXMatBuffPlayer = NULL;
	}

}

	//=============================================================================
	// �X�V����
	//=============================================================================
	void UpdatePlayer(void)
	{
		D3DXVECTOR3 rotCamera;
		player.old = player.pos;		// ���̈ʒu

		float fDiffRotY;
		float ground = 0;
		// �J�����̌����擾
		rotCamera = GetRotCamera();
		if(player.use==true)
		{
			if((GetKeyboardPress(DIK_LEFT))||(IsButtonPressed(0, BUTTON_LEFT)))
		{
			speed += 0.02f;

			player.rot.x -= VALUE_ROTATE_PLAYER;

				if((GetKeyboardPress(DIK_UP))||(IsButtonPressed(0,BUTTON_UP)))
				{// ���O�ړ�
					//if (GetKeyboardRelease(DIK_UP))

					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;

					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}

				}
				else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
				{// ����ړ�
					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;

					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}


				}
				else
				{// ���ړ�
					player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;
					player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;

					player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
			
					player.rotDest.y -= VALUE_ROTATE_PLAYER;
					if (player.rotDest.y < -D3DX_PI)
					{
						player.rotDest.y += D3DX_PI * 2.0f*speed;
					}

				}
		}
		else if((GetKeyboardPress(DIK_RIGHT)) || (IsButtonPressed(0, BUTTON_RIGHT)))
		{
			player.rot.x -= VALUE_ROTATE_PLAYER;
			speed += 0.02f;

			if((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_UP)))
			{// �E�O�ړ�
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
			}
			else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
			{// �E��ړ�
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
			}
			else
			{// �E�ړ�
				player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;
				player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER*speed;

				player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;

			}
		}
		else if((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_UP)))
		{// �O�ړ�
			speed += 0.02f;	

			player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.rot.x -= VALUE_ROTATE_PLAYER * speed;


			player.rotDest.y = D3DX_PI + rotCamera.y;
		
		}
		else if((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_DOWN)))
		{// ��ړ�
			speed += 0.02f;

			player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER*speed;
			player.rot.x -= VALUE_ROTATE_PLAYER * speed;


			player.rotDest.y = rotCamera.y;
		}


		if( (GetKeyboardTrigger(DIK_Z)) || (IsButtonPressed(0, BUTTON_C)))
		{
		
			player.use = false;

		}

		// �ړI�̊p�x�܂ł̍���
		fDiffRotY = player.rotDest.y - player.rot.y;
		if(fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if(fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// �ړI�̊p�x�܂Ŋ�����������
		player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if(player.rot.y > D3DX_PI)
		{
			player.rot.y -= D3DX_PI * 2.0f;
		}
		if(player.rot.y < -D3DX_PI)
		{
			player.rot.y += D3DX_PI * 2.0f;
		}


		if (speed >= 5.0f)
		{
			speed = 5.0f;
		}

		// �ʒu�ړ�
		player.pos.x += player.move.x;
		player.pos.y += player.move.y;
		player.pos.z += player.move.z;




		ground = Hitfield(player.pos);

			//�O�_����
			if (player.pos.y < ground)
			{
				player.ground = true;
				player.pos.y = ground /*+ 18.62*/;
				player.air = false;
				player.drop = false;
			SetVel();

			}
			else
			{
				player.ground = false;
				player.air = true;
				player.drop = true;
			}
		


		if ((player.move.y< 0.0f)&& player.pos.y < ground)
		{
			Repelling(&player);

		}

		if (player.ground == false)/*(player.air == true) || (player.drop == true))*/
		{
			Gravity(&player);
		}
		if (((GetKeyboardPress(DIK_SPACE)) || (IsButtonPressed(0, BUTTON_B))) && (player.air == false))
		{
			Jump(&player);
		}

		// �ړ��ʂɊ�����������
		player.move.x += (0.0f - player.move.x) * RATE_MOVE_PLAYER;
		player.move.y += (0.0f - player.move.y) * RATE_MOVE_PLAYER;
		player.move.z += (0.0f - player.move.z) * RATE_MOVE_PLAYER;





		D3DXVECTOR3		vec = player.move - player.old;
		float			len = D3DXVec3Length(&vec);
		if (len > 0.1f)
		{	// �r���{�[�h�Ƃ̓����蔻��

			if (hitCheckWall(player.pos, player.old) != 0)
			{
				// �������Ă���̂Ō��̈ʒu�ɖ߂�
				speed = speed/10;
				player.move = Speed_reset;
				player.pos = player.old;

				ChangeLife(-1);
			}
		}

		speed -= 0.003f;

		if (speed <= 0)
		{
			speed = 0;
		}

		// �e�̈ʒu�ݒ�
		{
			SetPositionShadow(player.nIdxShadow, D3DXVECTOR3(player.pos.x, ground, player.pos.z));

			float fSizeX = 20.0f + (player.pos.y - 10.0f) * 0.05f;
			if(fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (player.pos.y - 10.0f) * 0.05f;
			if(fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			SetVertexShadow(player.nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (player.pos.y - 10.0f)) / 400.0f;
			if(colA < 0.0f)
			{
				colA = 0.0f;
			}
			SetColorShadow(player.nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
		}


		if((player.move.x * player.move.x
		+ player.move.y * player.move.y
		+ player.move.z * player.move.z) > 1.0f)
		{
			D3DXVECTOR3 pos;

			//pos.x = player.pos.x + sinf(player.rot.y) * 10.0f;
			//pos.y = player.pos.y + 2.0f;
			//pos.z = player.pos.z + cosf(player.rot.y) * 10.0f;

			// �G�t�F�N�g�̐ݒ�
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
			//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//					D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
		}



		{
			ENEMY *enemy = GetEnemy(0);

			for (int i = 0; i < ENEMY_MAX; i++, enemy++)
			{

				if ((enemy->bUse == true) && (enemy->blown == false))
				{
					float fLength;

					fLength = (player.pos.x - enemy->posEnemy.x) * (player.pos.x - enemy->posEnemy.x)
						+ (player.pos.y - enemy->posEnemy.y) * (player.pos.y - enemy->posEnemy.y)
						+ (player.pos.z - enemy->posEnemy.z) * (player.pos.z - enemy->posEnemy.z);

					if (player.use == false)
					{
						player.fRadius = PLAYER_RADIUS_BOM;
					}
					if (fLength < (player.fRadius + ENEMY_HIT) * (player.fRadius + ENEMY_HIT))
					{
						//int j = 0;
						int repel_x = 0;
						int repel_z = 0;
						repel_x = rand() % 100;
						repel_z = rand() % 50;

						if (repel_x % 2 == 0)
						{
							repel_x = repel_x * (-1);
						}

						DeleteEnemy(i);
						SetParticle(enemy->posEnemy, Speed_reset, D3DXCOLOR(0.8f, 0.7f, 0.2f, 0.85f), 30.0f, 30.0f, 10,1);
						enemy->moveEnemy.x = player.move.x*repel_x;
						enemy->moveEnemy.z = repel_z;

						// �X�R�A���Z
						ChangeScore(100);

						// SE�Đ�


					}
				}
		/************************************************************************************
		**�@�p������
		*************************************************************************************/
				if(player.ground==true)
				{
					
					D3DXVECTOR3 vx, nvx;
					D3DXQUATERNION quat;
					float len, angle;

					D3DXVec3Cross(&vx, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &player.Dest);

					len = D3DXVec3Length(&vx);
					D3DXVec3Normalize(&nvx, &vx);

					angle = asinf(len);

					D3DXQuaternionIdentity(&quat);
					D3DXQuaternionRotationAxis(&quat, &nvx, angle);
					D3DXQuaternionSlerp(&player.Quaternion, &player.Quaternion, &quat, 0.05f);
				}
			}
		}
		
			if (GetLife()==0)
			{
				player.use = false;
			}


			if (player.use == false)
			{
				player.pos.y += 20.0f;
				player.pos.z -= 100.0f;

				SetExplosion(player.pos, 300.0f, 300.f);
				SetFade(FADE_OUT);

			}
		}

		//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", player.pos.x, player.pos.y, player.pos.z);
		//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", player.rot.y, player.rotDest.y);
		//PrintDebugProc("[�W�����v����F(%d)\n", player.air);
		//PrintDebugProc("[�����锻��F(%d)\n", player.drop);
		//PrintDebugProc("[vel  �F(%d)\n", Getvel());
		//PrintDebugProc("[vel  �F(%d)\n", Getvel());

		//PrintDebugProc("\n");

		//PrintDebugProc("*** ��s�@���� ***\n");
		//PrintDebugProc("�O�ړ� : ��\n");
		//PrintDebugProc("��ړ� : ��\n");
		//PrintDebugProc("���ړ� : ��\n");
		//PrintDebugProc("�E�ړ� : ��\n");
		//PrintDebugProc("�㏸   : �h\n");
		//PrintDebugProc("���~   : �j\n");
		//PrintDebugProc("������ : LSHIFT\n");
		//PrintDebugProc("�E���� : RSHIFT\n");

	}

//=============================================================================
// CORE(�e)�̕`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, quatMatrixs, shadowMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	if (player.use == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, 1.0f,1.0f,1.0f/*player.scl.x, player.scl.y, player.scl.z*/);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//�N�H�[�^�j�I�����}�g���N�X
		D3DXMatrixRotationQuaternion(&quatMatrixs, &player.Quaternion);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &quatMatrixs);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);




		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)NumMatPlayer; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTexturePlayer);

			// �`��
			MeshPlayer->DrawSubset(nCntMat);
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);

		//mtxWorld_core = mtxWorld;


		//
		//DrawPlayer_shaft_r();
		//DrawPlayer_shaft_l();
		//DrawPlayer_wheel_r();
		//DrawPlayer_wheel_l();
		//DrawPlayer_enjin_r();
		//DrawPlayer_enjin_l();
		
	}
}
////********************************************************************************************************
////***	�V���t�g�i�E�j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_shaft_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	//�e�̃}�g���N�X��������
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_shaft_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_r; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_shaft_r->DrawSubset(nCntMat);
//	}
//	//mtxWorldplayer_body = mtxWorldplayer;
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	�V���t�g�i���j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_shaft_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//	//�e�̃}�g���N�X��������
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_shaft_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_l; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_shaft_l->DrawSubset(nCntMat);
//	}
//	//mtxWorldplayer_body = mtxWorldplayer;
//
//	mtxWorld_core = mtxWorld;
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	�ԗցi�E�j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_wheel_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_wheel_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_r; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_wheel_r->DrawSubset(nCntMat);
//	}
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	�ԗցi���j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_wheel_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_wheel_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_l; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_wheel_l->DrawSubset(nCntMat);
//	}
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	�G���W���i�E�j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_enjin_r(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_enjin_r->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_r; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_enjin_r->DrawSubset(nCntMat);
//	}
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}
//
////********************************************************************************************************
////***	�G���W���i���j�̕`�揈��
////********************************************************************************************************
//void DrawPlayer_enjin_l(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//	D3DXMATERIAL *pD3DXMat;
//	D3DMATERIAL9 matDef;
//
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxWorld);
//
//	// �X�P�[���𔽉f
//	D3DXMatrixScaling(&mtxScl, player.scl.x, player.scl.y, player.scl.z);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
//
//	// ��]�𔽉f
//	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rot.y, player.rot.x, player.rot.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	//D3DXMatrixTranslation(&mtxTranslate, player.pos.x, player.pos.y, player.pos.z);
//	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
//
//	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());
//
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
//
//	// ���݂̃}�e���A�����擾
//	pDevice->GetMaterial(&matDef);
//
//	// �}�e���A�����ɑ΂���|�C���^���擾
//	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffPlayer_enjin_l->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_l; nCntMat++)
//	{
//		// �}�e���A���̐ݒ�
//		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, D3DTexturePlayer);
//
//		// �`��
//		MeshPlayer_enjin_l->DrawSubset(nCntMat);
//	}
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	pDevice->SetMaterial(&matDef);
//}

//=============================================================================
// �v���C���[���擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &player;
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return player.pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player.rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return player.rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return player.move;
}
////*******************************************************************************
////**
////*******************************************************************************
//D3DXMATRIX *GetPlayerMatrix(void)
//{
//	return &mtxWorld_core;
//}
//
