//=============================================================================
//
// ライフ処理 [life.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

void ChangeLife(int value);
int GetLife(void);

#endif
