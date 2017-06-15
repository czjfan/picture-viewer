// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

//����GUIϵͳ��ʹ��
#define USE_D3DGUI
//#define USE_GDIGUI//�Ƿ����GDIʽGUI

#ifdef USE_GDIGUI
// Windows ͷ�ļ�
//afxwin.h is OK before windows.h
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#endif // USE_GDIGUI

#include <windows.h>
#include <atltypes.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifdef USE_GDIGUI
struct memDCBMP
{
	CBitmap *poldbmp;

	CDC *pDC;
	CBitmap *bmp;
	int x, y, cx, cy;
};

CBitmap *MyDrawPrepareOne(CDC *pDC, memDCBMP *mDC, int x, int y, CRect &r);
void DeleteMemDCBMP(memDCBMP *mDC);
#endif // USE_GDIGUI
