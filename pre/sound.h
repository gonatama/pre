
#ifndef ___SOUND_H___
#define ___SOUND_H___

#include "main.h"
#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

// �}�N����`
enum
{	// �T�E���h�ʂ��i���o�[
	BGM_00,
	SE_EXPLOSION,
	SE_HIT,
	FIRE_START,
	ENTER,
	SOUND_MAX
};

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

HRESULT					InitSound(HWND hWnd);	// ������
void					UninitSound();			// ��Еt��
LPDIRECTSOUNDBUFFER8	LoadSound(int no);	// �T�E���h�̃��[�h
void					PlaySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0);	// �����ƂɍĐ�
bool					IsPlaying(LPDIRECTSOUNDBUFFER8 pBuffer);					// �Đ������ǂ���

#endif


////=============================================================================
////
//// �T�E���h���� [sound.h]
//// Author : AKIRA TANAKA
////
////=============================================================================
//#ifndef _SOUND_H_
//#define _SOUND_H_
//
//#include "main.h"
//
//// �T�E���h�t�@�C��
//typedef enum
//{
//	SOUND_LABEL_BGM000 = 0,		// BGM0
//	SOUND_LABEL_BGM001,			// BGM1
//	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
//	SOUND_LABEL_SE_EXPLOSION,	// ������
//	SOUND_LABEL_SE_COIN,		// �Փˉ�0
//	SOUND_LABEL_MAX,
//} SOUND_LABEL;
//
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT InitSound(HWND hWnd);
//void UninitSound(void);
//HRESULT PlaySound(SOUND_LABEL label);
//void StopSound(SOUND_LABEL label);
//void StopSound(void);
//
//#endif
