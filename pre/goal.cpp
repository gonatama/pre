//=============================================================================
//
// モデル処理 [goal.cpp]
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
// マクロ定義
//*****************************************************************************
#define	GOAL_TEX		"data/OBJ/Sky_Tower.x"							// 読み込むモデル名
#define	VIEW_NEAR_GOAL		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_GOAL		(1000.0f)										// ビュー平面のFarZ値
//#define	GOAL_MAX		(3)
#define	GOAL_MOVE		(2.0f)											// 移動量
#define	GOAL_ROTATE	(D3DX_PI * 0.02f)								// 回転量

//#define	NUM_LIGHT		(3)												// ライトの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureGoal;		// テクスチャへのポインタ
LPD3DXMESH			D3DXMeshGoal;		// メッシュ情報へのポインタ
LPD3DXBUFFER		D3DXBuffMatGoal;	// マテリアル情報へのポインタ
DWORD				nNumMatGoal;			// マテリアル情報の数

GOAL				goal[GOAL_MAX];

//D3DXVECTOR3			posGoal;				// モデルの位置
//D3DXVECTOR3			rotGoal;				// モデルの向き(回転)
//D3DXVECTOR3			sclGoal;				// モデルの大きさ(スケール)

D3DXMATRIX			goal_mtxWorld;				// ワールドマトリックス


//int					goal_nIdxShadow;				// 影ID
//float				goal_fSizeShadow;				// 影のサイズ
//D3DXCOLOR			goal_colShadow;				// 影の色

//D3DLIGHT9			aLight[NUM_LIGHT];	// ライト情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GOAL *goal = GetGoal(0);
	//// カメラの初期化
	//InitCamera();

	//// ライトの初期化
	//InitLight();
	for (int i = 0; i < GOAL_MAX; i++, goal++)
	{
		// 位置・回転・スケールの初期設定
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
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(GOAL_TEX,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatGoal,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&nNumMatGoal,		// D3DXMATERIAL構造体の数
		&D3DXMeshGoal)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&pD3DTextureGoal);	// 読み込むメモリー
#endif
		// 影の生成
	//goal_nIdxShadow = CreateShadow(goal->posGoal, 25.0f, 25.0f);
	//goal_fSizeShadow = 25.0f;
	//goal_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGoal(void)
{
	if (D3DTextureGoal != NULL)
	{// テクスチャの開放
		D3DTextureGoal->Release();
		D3DTextureGoal = NULL;
	}

	if (D3DXMeshGoal != NULL)
	{// メッシュの開放
		D3DXMeshGoal->Release();
		D3DXMeshGoal = NULL;
	}

	if (D3DXBuffMatGoal != NULL)
	{// マテリアルの開放
		D3DXBuffMatGoal->Release();
		D3DXBuffMatGoal = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GOAL *goal = GetGoal(0);

	goal->posGoal.y = Hitfield(goal->posGoal);


	/*■目標を追いかける追尾弾
		追尾弾の関数（）
	{
	弾の初期化
	├目標までの距離検出
	│・目標までの距離Ｘ＝（目標の座標Ｘ）ー（弾の座標Ｘ）
	│・目標までの距離Ｙ＝（目標の座標Ｙ）ー（弾の座標Ｙ）
	│　↓
	└弾の発射角度取得し移動量設定
	 ・弾の発射角度＝atan2(目標までの距離Ｙ、目標までの距離Ｘ)
	 ・弾の移動量Ｘ＝cos(弾の発射角度)×弾のスピード
	 ・弾の移動量Ｙ＝sin(弾の発射角度)×弾のスピード
	   ↓
	┌→任意の間隔で目標の位置を再検出
	│　　↓
	│移動量を計算し目的地まで飛ばす
	│・弾の座標Ｘ＋＝弾の移動量Ｘ
	│・弾の座標Ｙ＋＝弾の移動量Ｙ
	│　↓
	└当たってないなら繰り返し
	  ↓
	当たったら初期化へ
	}
	*/
	//if (GetKeyboardTrigger(DIK_RETURN))
	//{
	//	SetStage(STAGE_RESULT);
	//}
}
//// 影の位置設定
//SetPositionShadow(goal_nIdxShadow, D3DXVECTOR3(goal->posGoal.x, 0.1f, goal->posGoal.z));

//SetVertexShadow(goal_nIdxShadow, goal_fSizeShadow, goal_fSizeShadow);
//SetColorShadow(goal_nIdxShadow, goal_colShadow);

//	//PrintDebugProc("[飛行機の位置  ：(%f : %f : %f)]\n", goal->posGoal.x, goal->posGoal.y, goal->posGoal.z);
//	//PrintDebugProc("[飛行機の向き  ：(%f) < 目的の向き:(%f) >]\n", goal->rotModel.y, goal->rotDestModel.y);
//	//PrintDebugProc("\n");
//
//	//PrintDebugProc("*** 飛行機操作 ***\n");
//	//PrintDebugProc("前移動 : ↑\n");
//	//PrintDebugProc("後移動 : ↓\n");
//	//PrintDebugProc("左移動 : ←\n");
//	//PrintDebugProc("右移動 : →\n");
//	//PrintDebugProc("上昇   : Ｉ\n");
//	//PrintDebugProc("下降   : Ｋ\n");
//	//PrintDebugProc("左旋回 : LSHIFT\n");
//	//PrintDebugProc("右旋回 : RSHIFT\n");
//
//}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&goal_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, goal->sclGoal.x, goal->sclGoal.y, goal->sclGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, goal->rotGoal.y, goal->rotGoal.x, goal->rotGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, goal->posGoal.x, goal->posGoal.y, goal->posGoal.z);
	D3DXMatrixMultiply(&goal_mtxWorld, &goal_mtxWorld, &mtxTranslate);




	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &goal_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatGoal->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatGoal; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureGoal);

		// 描画
		D3DXMeshGoal->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
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
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionGoal(void)
{
	return goal->posGoal;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationGoal(void)
{
	return goal->rotGoal;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestGoal(void)
{
	return goal->rotDestGoal;
}


////=============================================================================
//// 内積
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
//// 外積
////=============================================================================
//void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//	//#if 0
//
//	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
//	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
//	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
//	//#else
//		//関数
//		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);
//
//
//	//#endif
//
//}
//
//
////=============================================================================
//// ポリゴンと線分との当たり判定
//// p0  :ポリゴンの頂点1
//// p1  :ポリゴンの頂点2
//// p2  :ポリゴンの頂点3
//// pos0:始点（移動前）
//// pos1:終点（移動後）
////=============================================================================
//int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		nor;		// ポリゴンの法線
//
//	{	// ポリゴンの外積をとって法線を求める。そして正規化しておく。
//		D3DXVECTOR3		vec01 = p1 - p0;
//		D3DXVECTOR3		vec02 = p2 - p0;
//		crossProduct(&nor, &vec01, &vec02);
//		D3DXVec3Normalize(&nor, &nor);
//
//
//	}
//
//	{	// 平面(三角形ポリゴン)と線分の内積とって衝突している可能性を調べる
//		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float a = dotProduct(&vec1, &nor);
//		float b = dotProduct(&vec2, &nor);
//		if ((a * b) > 0)
//		{
//			// 当たっている可能性は無い
//			return(0);
//		}
//	}
//
//
//	{	// ポリゴンと線分の交点を求める
//		//D3DXVECTOR3		vec1 = pos0 - p0;
//		//D3DXVECTOR3		vec2 = pos0 - p0;
//		//float			d1;					// 内分比を求める為の点とポリゴンとの距離
//		//float			d2;					// 内分比を求める為の点とポリゴンとの距離
//		//float			a;					// 内分比
//		//D3DXVECTOR3		vec3;
//		//D3DXVECTOR3		p3;					// 交点
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
//		{	// 求めた交点がポリゴンの中にあるか調べる
//
//			// ポリゴンの各辺のベクトル
//			D3DXVECTOR3		v1 = p1 - p0;
//			D3DXVECTOR3		v2 = p2 - p1;
//			D3DXVECTOR3		v3 = p3 = p0;
//
//			// 各頂点と交点とのベクトル
//			D3DXVECTOR3		v4 = p3 - p1;
//			D3DXVECTOR3		v5 = p3 - p2;
//			D3DXVECTOR3		v6 = p3 - p0;
//
//			// 各辺との外積で法線を求める
//			D3DXVECTOR3		n1, n2, n3;
//			crossProduct(&n1, &v1, &v4);
//			crossProduct(&n2, &v2, &v5);
//			crossProduct(&n3, &v3, &v6);
//
//
//
//			// それぞれのベクトルの向きを内積でチェック
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
//// ビルボードとの当たり判定
//// pos0:始点（移動前）
//// pos1:終点（移動後）
////=============================================================================
//int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	PLAYER *player = GetPlayer(0);
//	int				ans = 0;	// 一先ず当たっていないをセットしておく
//
//	D3DXVECTOR3		pos[4];
//	D3DXVECTOR3		nor;		// ポリゴンの法線
//
//								// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	VERTEX_3D		*pVtx;
//	D3DXBuffMatGoal->Lock(0, 0, (void**)&pVtx, 0);
//
//
//	for (int i = 0; i < 1; i++, pVtx += 4)
//	{	// まずは、ポリゴンの頂点を求める
//
//		// 頂点座標の設定
//		pos[0] = pVtx[0].vtx;
//		pos[1] = pVtx[1].vtx;
//		pos[2] = pVtx[2].vtx;
//		pos[3] = pVtx[3].vtx;
//
//		// 左下側ポリゴンと線分の当たり判定
//		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
//		if (ans != 0) break;
//
//		// 右上側ポリゴンと線分の当たり判定
//		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
//		if (ans != 0) break;
//
//	}
//
//	// 頂点データをアンロックする
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
//	size1 /= 3.0f;	// 半サイズにする
//	size2 /= 2.0f;	// 同上
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
