//=============================================================================
//
// メッシュ壁の処理 [meshwall.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "meshwall.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MESH_WALL		(10)							// 壁の総数
#define	TEXTURE_FILENAME	"data/TEXTURE/wall001.jpg"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_WALL		(5.0f)							// 移動速度
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)				// 回転速度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTexture = NULL;		// テクスチャ読み込み場所

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;			// インデックスバッファインターフェースへのポインタ

	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 pos;							// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rot;							// ポリゴンの回転角
	int nNumBlockX, nNumBlockY;					// ブロック数
	int nNumVertex;								// 総頂点数	
	int nNumVertexIndex;						// 総インデックス数
	int nNumPolygon;							// 総ポリゴン数
	float fBlockSizeX, fBlockSizeY;				// ブロックサイズ
} MESH_WALL;

MESH_WALL g_aMeshWall[MAX_MESH_WALL];			// メッシュ壁ワーク
int NumMeshWall = 0;						// メッシュ壁の数

//=============================================================================
// 初期化処理
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

	// テクスチャの読み込み
	if (!g_pD3DTexture)
	{
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&g_pD3DTexture);		// 読み込むメモリー
	}

	pMesh = &g_aMeshWall[NumMeshWall];
	NumMeshWall++;

	// ポリゴン表示位置の中心座標を設定
	pMesh->pos = pos;

	pMesh->rot = rot;

	// ブロック数の設定
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;

	// 頂点数の設定
	pMesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

	// インデックス数の設定
	pMesh->nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

	// ポリゴン数の設定
	pMesh->nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

	// ブロックサイズの設定
	pMesh->fBlockSizeX = fBlockSizeX;
	pMesh->fBlockSizeY = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pMesh->pD3DVtxBuff,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pMesh->pD3DIdxBuff,				// インデックスバッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockY;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxY = 0; nCntVtxY < (pMesh->nNumBlockY + 1); nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (pMesh->nNumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.x = -(pMesh->nNumBlockX / 2.0f) * pMesh->fBlockSizeX + nCntVtxX * pMesh->fBlockSizeX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.y = pMesh->nNumBlockY * pMesh->fBlockSizeY - nCntVtxY * pMesh->fBlockSizeY;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].vtx.z = 0.0f;

				// 法線の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 反射光の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].diffuse = col;

				// テクスチャ座標の設定
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxY * (pMesh->nNumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxY;
			}
		}

		// 頂点データをアンロックする
		pMesh->pD3DVtxBuff->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxY = 0; nCntVtxY < pMesh->nNumBlockY; nCntVtxY++)
		{
			if (nCntVtxY > 0)
			{// 縮退ポリゴンのためのダブりの設定
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
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxY * (pMesh->nNumBlockX + 1) + pMesh->nNumBlockX;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		pMesh->pD3DIdxBuff->Unlock();
	}



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < NumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];

		if (pMesh->pD3DVtxBuff)
		{// 頂点バッファの開放
			pMesh->pD3DVtxBuff->Release();
			pMesh->pD3DVtxBuff = NULL;
		}

		if (pMesh->pD3DIdxBuff)
		{// インデックスバッファの開放
			pMesh->pD3DIdxBuff->Release();
			pMesh->pD3DIdxBuff = NULL;
		}
	}

	if (g_pD3DTexture)
	{// テクスチャの開放
		g_pD3DTexture->Release();
		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3		nor;		// ポリゴンの法線
	D3DXMATRIX mtxRot, mtxTranslate;

	MESH_WALL *pMesh;
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < NumMeshWall; nCntMeshField++)
	{
		pMesh = &g_aMeshWall[nCntMeshField];


		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, pMesh->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(pMesh->pD3DIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexture);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);
	}
}



//=============================================================================
// 内積
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{

	//float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
	float ans = D3DXVec3Dot(vl, vr);


	return(ans);
}


//=============================================================================
// 外積
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//#if 0

	//ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	//ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	//ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	//#else
		//関数
	D3DXVec3Cross(ret, vl, vr);


	//#endif

}


//=============================================================================
// ポリゴンと線分との当たり判定
// p0  :ポリゴンの頂点1
// p1  :ポリゴンの頂点2
// p2  :ポリゴンの頂点3
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// ポリゴンの法線

	{	// ポリゴンの外積をとって法線を求める。そして正規化しておく。
		D3DXVECTOR3		vec01 = p1 - p0;
		D3DXVECTOR3		vec02 = p2 - p0;
		crossProduct(&nor, &vec01, &vec02);
		D3DXVec3Normalize(&nor, &nor);


	}

	{	// 平面(三角形ポリゴン)と線分の内積とって衝突している可能性を調べる
		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);
		if ((a * b) > 0)
		{
			// 当たっている可能性は無い
			return(0);
		}
	}


	{	// ポリゴンと線分の交点を求める
		//D3DXVECTOR3		vec1 = pos0 - p0;
		//D3DXVECTOR3		vec2 = pos0 - p0;
		//float			d1;					// 内分比を求める為の点とポリゴンとの距離
		//float			d2;					// 内分比を求める為の点とポリゴンとの距離
		//float			a;					// 内分比
		//D3DXVECTOR3		vec3;
		//D3DXVECTOR3		p3;					// 交点


		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float			d1 = fabs(dotProduct(&nor, &vec1));
		float			d2 = fabs(dotProduct(&nor, &vec2));
		float			a = d1 / (d1 + d2);
		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
		D3DXVECTOR3		p3 = p0 + vec3;




		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p0 - p2;

			// 各頂点と交点とのベクトル
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// 各辺との外積で法線を求める
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);



			// それぞれのベクトルの向きを内積でチェック
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);



		}
	}

	return(1);
}


//=============================================================================
// ビルボードとの当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
int hitCheckWall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	int				ans = 0;	// 一先ず当たっていないをセットしておく

	MESH_WALL *pMesh;
	D3DXVECTOR3		pos[4];
	D3DXVECTOR3		nor;		// ポリゴンの法線
	VERTEX_3D		*pVtx;

	for (int i = 0; i < 8; i++)
	{

		pMesh = &g_aMeshWall[i];

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得

		pMesh->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// 頂点座標の設定
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		D3DXMATRIX mtxRot;

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		for (int j = 0; j < 4; j++)
		{
			D3DXVec3TransformCoord(&pos[j], &pos[j], &mtxRot);
		}

		D3DXMATRIX mtxTranslate;
		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		for (int j = 0; j < 4; j++)
		{
			D3DXVec3TransformCoord(&pos[j], &pos[j], &mtxTranslate);
		}

		// 左下側ポリゴンと線分の当たり判定
		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
		if (ans != 0) break;

		// 右上側ポリゴンと線分の当たり判定
		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
		if (ans != 0) break;


	// 頂点データをアンロックする
	pMesh->pD3DVtxBuff->Unlock();


	}




	return(ans);

}


