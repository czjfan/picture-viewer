// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// picture.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H �������κ�����ĸ���ͷ�ļ���
//�������ڴ��ļ�������


#ifdef USE_GDIGUI

CBitmap *MyDrawPrepareOne(CDC *pDC, memDCBMP *mDC, int x, int y, CRect &r)
{
	mDC->x = x;
	mDC->y = y;
	mDC->pDC = new CDC;
	mDC->pDC->CreateCompatibleDC(pDC);
	mDC->bmp = new CBitmap;
	mDC->bmp->CreateCompatibleBitmap(pDC, r.Width(), r.Height());
	mDC->poldbmp = mDC->pDC->SelectObject(mDC->bmp);
	CRect r0(0, 0, r.Width(), r.Height());
	mDC->pDC->IntersectClipRect(&r0);
	mDC->cx = r.Width();
	mDC->cy = r.Height();

	return mDC->poldbmp;
}



void DeleteMemDCBMP(memDCBMP *mDC)
{
	if (mDC)
	{
		if (mDC->pDC)
		{
			mDC->pDC->SelectObject(mDC->poldbmp);
			mDC->pDC->DeleteDC();
		}
		if (mDC->bmp)
		{
			mDC->bmp->DeleteObject();
		}
	}
}

#endif // USE_GDIGUI