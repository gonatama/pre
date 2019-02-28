//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : 
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "enemy.h"

#define	TEXTURE_FIELD	"data/TEXTURE/field004.jpg"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int NumBlockX, int NumBlockZ, float BlockSizeX, float BlockSizeZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

//int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
//int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
//
FLOAT Hitfield(D3DXVECTOR3 pos);
FLOAT Hitfield_Enemy(ENEMY *enemy);
LPDIRECT3DVERTEXBUFFER9 GetMeshFieldVtx(void);

#endif
