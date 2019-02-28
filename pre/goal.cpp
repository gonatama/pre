//=============================================================================
//
// ���f������ [goal.cpp]
// Author : 
//
//=============================================================================
#include "goal.h"
#include "meshfield.h"
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	GOAL_TEX		"data/OBJ/Sky_Tower.x"							// �ǂݍ��ރ��f����
#define	VIEW_NEAR_GOAL		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_GOAL		(1000.0f)										// �r���[���ʂ�FarZ�l
//#define	GOAL_MAX		(3)
#define	GOAL_MOVE		(2.0f)											// �ړ���
#define	GOAL_ROTATE	(D3DX_PI * 0.02f)								// ��]��

//#define	NUM_LIGHT		(3)												// ���C�g�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureGoal;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			D3DXMeshGoal;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatGoal;	// �}�e���A�����ւ̃|�C���^
DWORD				nNumMatGoal;			// �}�e���A�����̐�

GOAL				goal[GOAL_MAX];

//D3DXVECTOR3			posGoal;				// ���f���̈ʒu
//D3DXVECTOR3			rotGoal;				// ���f���̌���(��])
//D3DXVECTOR3			sclGoal;				// ���f���̑傫��(�X�P�[��)

D3DXMATRIX			goal_mtxWorld;				// ���[���h�}�g���b�N�X


//int					goal_nIdxShadow;				// �eID
//float				goal_fSizeShadow;				// �e�̃T�C�Y
//D3DXCOLOR			goal_colShadow;				// �e�̐F

//D3DLIGHT9			aLight[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GOAL *goal = GetGoal(0);
	//// �J�����̏�����
	//InitCamera();

	//// ���C�g�̏�����
	//InitLight();
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		goal->posGoal = D3DXVECTOR3(0.0f, 0.0f, 3000.0f);
		goal->rotGoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		goal->sclGoal = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

		goal->moveGoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		goal->rotGoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		goal->rotDestGoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DTextureGoal = NULL;
		D3DXMeshGoal = NULL;
		D3DXBuffMatGoal = NULL;
		nNumMatGoal = 0;

	}
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(GOAL_TEX,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatGoal,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&nNumMatGoal,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshGoal)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&pD3DTextureGoal);	// �ǂݍ��ރ������[
#endif
		// �e�̐���
	//goal_nIdxShadow = CreateShadow(goal->posGoal, 25.0f, 25.0f);
	//goal_fSizeShadow = 25.0f;
	//goal_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGoal(void)
{
	if (D3DTextureGoal != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureGoal->Release();
		D3DTextureGoal = NULL;
	}

	if (D3DXMeshGoal != NULL)
	{// ���b�V���̊J��
		D3DXMeshGoal->Release();
		D3DXMeshGoal = NULL;
	}

	if (D3DXBuffMatGoal != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatGoal->Release();
		D3DXBuffMatGoal = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GOAL *goal = GetGoal(0);

	goal->posGoal.y = Hitfield(goal->posGoal);


	/*���ڕW��ǂ�������ǔ��e
		�ǔ��e�̊֐��i�j
	{
	�e�̏�����
	���ڕW�܂ł̋������o
	���E�ڕW�܂ł̋����w���i�ڕW�̍��W�w�j�[�i�e�̍��W�w�j
	���E�ڕW�܂ł̋����x���i�ڕW�̍��W�x�j�[�i�e�̍��W�x�j
	���@��
	���e�̔��ˊp�x�擾���ړ��ʐݒ�
	 �E�e�̔��ˊp�x��atan2(�ڕW�܂ł̋����x�A�ڕW�܂ł̋����w)
	 �E�e�̈ړ��ʂw��cos(�e�̔��ˊp�x)�~�e�̃X�s�[�h
	 �E�e�̈ړ��ʂx��sin(�e�̔��ˊp�x)�~�e�̃X�s�[�h
	   ��
	�����C�ӂ̊Ԋu�ŖڕW�̈ʒu���Č��o
	���@�@��
	���ړ��ʂ��v�Z���ړI�n�܂Ŕ�΂�
	���E�e�̍��W�w�{���e�̈ړ��ʂw
	���E�e�̍��W�x�{���e�̈ړ��ʂx
	���@��
	���������ĂȂ��Ȃ�J��Ԃ�
	  ��
	���������珉������
	}
	*/
	//if (GetKeyboardTrigger(DIK_RETURN))
	//{
	//	SetStage(STAGE_RESULT);
	//}
}
//// �e�̈ʒu�ݒ�
//SetPositionShadow(goal_nIdxShadow, D3DXVECTOR3(goal->posGoal.x, 0.1f, goal->posGoal.z));

//SetVertexShadow(goal_nIdxShadow, goal_fSizeShadow, goal_fSizeShadow);
//SetColorShadow(goal_nIdxShadow, goal_colShadow);

//	//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", goal->posGoal.x, goal->posGoal.y, goal->posGoal.z);
//	//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", goal->rotModel.y, goal->rotDestModel.y);
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
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&goal_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, goal->sclGoal.x, goal->sclGoal.y, goal->sclGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, goal->rotGoal.y, goal->rotGoal.x, goal->rotGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, goal->posGoal.x, goal->posGoal.y, goal->posGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxTranslate);




	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &goal_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatGoal->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatGoal; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureGoal);

		// �`��
		D3DXMeshGoal->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}
/**********************************************************************************
** GetGoal
***********************************************************************************/
GOAL	*GetGoal(int pno)
{
	return &goal[pno];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionGoal(void)
{
	return goal->posGoal;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationGoal(void)
{
	return goal->rotGoal;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestGoal(void)
{
	return goal->rotDestGoal;
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
//	VERTEX_3D		*pVtx;
//	D3DXBuffMatGoal->Lock(0, 0, (void**)&pVtx, 0);
//
//
//	for (int i = 0; i < 1; i++, pVtx += 4)
//	{	// �܂��́A�|���S���̒��_�����߂�
//
//		// ���_���W�̐ݒ�
//		pos[0] = pVtx[0].vtx;
//		pos[1] = pVtx[1].vtx;
//		pos[2] = pVtx[2].vtx;
//		pos[3] = pVtx[3].vtx;
//
//		// �������|���S���Ɛ����̓����蔻��
//		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
//		if (ans != 0) break;
//
//		// �E�㑤�|���S���Ɛ����̓����蔻��
//		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
//		if (ans != 0) break;
//
//	}
//
//	// ���_�f�[�^���A�����b�N����
//	D3DXBuffMatGoal->Unlock();
//
//
//
//
//	return(ans);
//
//}




//bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
//{
//	size1 /= 3.0f;	// ���T�C�Y�ɂ���
//	size2 /= 2.0f;	// ����
//
//	if ((pos2.x + size2.x > pos1.x - size1.x && pos1.x + size1.x > pos2.x - size2.x) && (pos2.y + size2.y > pos1.y - size1.y && pos1.y + size1.y > pos2.y - size2.y))
//	{
//		//if (player->drop_at == false)
//		//{
//		//	player->use = false;
//
//		return true;
//		//	}
//		//	else if (player->drop_at == true)
//		//	{
//		//		enemy->use = false;
//		//	}
//	}
//
//	return false;
//}
