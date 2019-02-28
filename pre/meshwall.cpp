//=============================================================================
//
// ���b�V���ǂ̏��� [meshwall.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "meshwall.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_MESH_WALL		(10)							// �ǂ̑���
#define	TEXTURE_FILENAME	"data/TEXTURE/wall001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_WALL		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)				// ��]���x

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTexture = NULL;		// �e�N�X�`���ǂݍ��ݏꏊ

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;			// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;							// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3 rot;							// �|���S���̉�]�p
	int nNumBlockX, nNumBlockY;					// �u���b�N��
	int nNumVertex;								// �����_��	
	int nNumVertexIndex;						// ���C���f�b�N�X��
	int nNumPolygon;							// ���|���S����
	float fBlockSizeX, fBlockSizeY;				// �u���b�N�T�C�Y
} MESH_WALL;

MESH_WALL g_aMeshWall[MAX_MESH_WALL];			// ���b�V���ǃ��[�N
int NumMeshWall = 0;						// ���b�V���ǂ̐�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
	int nNumBlockX, int nNumBlockY, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_WALL *pMesh;

	if (NumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	if (!g_pD3DTexture)
	{
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&g_pD3DTexture);		// �ǂݍ��ރ������[
	}

	pMesh = &g_aMeshWall[NumMeshWall];
	NumMeshWall++;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh->pos = pos;

	pMesh->rot = rot;

	// �u���b�N���̐ݒ�
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;

	// ���_���̐ݒ�
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// �|���S�����̐ݒ�
	pMesh->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	pMesh->fBlockSizeX = fBlockSizeX;
	pMesh->fBlockSizeY = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pMesh->pD3DVtxBuff,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pMesh->pD3DIdxBuff,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxY = 0; nCntVtxY < (pMesh->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.x = -(pMesh->nNumBlockX / 2.0f) * pMesh->fBlockSizeX + nCntVtxX * pMesh->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.y = pMesh->nNumBlockY * pMesh->fBlockSizeY - nCntVtxY * pMesh->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.z = 0.0f;

				// �@���̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���ˌ��̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].diffuse = col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxY;
			}
		}

		// ���_�f�[�^���A�����b�N����
		pMesh->pD3DVtxBuff->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxY + 1) * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxY < (pMesh->nNumBlockY - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + pMesh->nNumBlockX;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		pMesh->pD3DIdxBuff->Unlock();
	}



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < NumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		if (pMesh->pD3DVtxBuff)
		{// ���_�o�b�t�@�̊J��
			pMesh->pD3DVtxBuff->Release();
			pMesh->pD3DVtxBuff = NULL;
		}

		if (pMesh->pD3DIdxBuff)
		{// �C���f�b�N�X�o�b�t�@�̊J��
			pMesh->pD3DIdxBuff->Release();
			pMesh->pD3DIdxBuff = NULL;
		}
	}

	if (g_pD3DTexture)
	{// �e�N�X�`���̊J��
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3		nor;		// �|���S���̖@��
	D3DXMATRIX mtxRot, mtxTranslate;

	MESH_WALL *pMesh;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < NumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];


		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, pMesh->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(pMesh->pD3DIdxBuff);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexture);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);
	}
}



//=============================================================================
// ����
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{

	//float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
	float ans = D3DXVec3Dot(vl, vr);


	return(ans);
}


//=============================================================================
// �O��
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//#if 0

	//ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	//ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	//ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	//#else
		//�֐�
	D3DXVec3Cross(ret, vl, vr);


	//#endif

}


//=============================================================================
// �|���S���Ɛ����Ƃ̓����蔻��
// p0  :�|���S���̒��_1
// p1  :�|���S���̒��_2
// p2  :�|���S���̒��_3
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// �|���S���̖@��

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�B�����Đ��K�����Ă����B
		D3DXVECTOR3		vec01 = p1 - p0;
		D3DXVECTOR3		vec02 = p2 - p0;
		crossProduct(&nor, &vec01, &vec02);
		D3DXVec3Normalize(&nor, &nor);


	}

	{	// ����(�O�p�`�|���S��)�Ɛ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);
		if ((a * b) > 0)
		{
			// �������Ă���\���͖���
			return(0);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		//D3DXVECTOR3		vec1 = pos0 - p0;
		//D3DXVECTOR3		vec2 = pos0 - p0;
		//float			d1;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		//float			d2;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		//float			a;					// ������
		//D3DXVECTOR3		vec3;
		//D3DXVECTOR3		p3;					// ��_


		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float			d1 = fabs(dotProduct(&nor, &vec1));
		float			d2 = fabs(dotProduct(&nor, &vec2));
		float			a = d1 / (d1 + d2);
		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
		D3DXVECTOR3		p3 = p0 + vec3;




		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// �e�ӂƂ̊O�ςŖ@�������߂�
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);



			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);



		}
	}

	return(1);
}


//=============================================================================
// �r���{�[�h�Ƃ̓����蔻��
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
int hitCheckWall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���

	MESH_WALL *pMesh;
	D3DXVECTOR3		pos[4];
	D3DXVECTOR3		nor;		// �|���S���̖@��
	VERTEX_3D		*pVtx;

	for (int i = 0; i < 8; i++)
	{

		pMesh = &g_aMeshWall[i];

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾

		pMesh->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// ���_���W�̐ݒ�
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		D3DXMATRIX mtxRot;

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		for (int j = 0; j < 4; j++)
		{
			D3DXVec3TransformCoord(&pos[j], &pos[j], &mtxRot);
		}

		D3DXMATRIX mtxTranslate;
		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		for (int j = 0; j < 4; j++)
		{
			D3DXVec3TransformCoord(&pos[j], &pos[j], &mtxTranslate);
		}

		// �������|���S���Ɛ����̓����蔻��
		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
		if (ans != 0) break;

		// �E�㑤�|���S���Ɛ����̓����蔻��
		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
		if (ans != 0) break;


	// ���_�f�[�^���A�����b�N����
	pMesh->pD3DVtxBuff->Unlock();


	}




	return(ans);

}


