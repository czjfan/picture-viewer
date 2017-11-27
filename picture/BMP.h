#pragma once 

#include <new>
#include <iostream>
#include <stdlib.h>
#include <winnt.h>
#include <strsafe.h>

#include "picfile.h"
#include "generalgeo.h"

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// ͸������Ч��
//#define TRANSPARENTBACK_SQUAREWIDTH		8
#define TRANSPARENTBACK_MASK			0b1000
#define TRANSPARENTBACK_HOLLOWCOLOR		D3DCOLOR_XRGB(255, 255, 255)
#define TRANSPARENTBACK_FILLCOLOR		D3DCOLOR_XRGB(204, 203, 204)
#define TRANSPARENTBACK_HOLLOWDENSITY	255
#define TRANSPARENTBACK_FILLDENSITY		230

#define RGB2GRAY(R, G, B)				(((R)*38 + (G)*75 + (B)*15) >> 7)

// �ڴ�
#define SAFE_RELEASE(p)					{ if (p) { (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p)					{ if (p) { delete (p); (p) = NULL;} }
#define SAFE_DELETE_LIST(p)				{ if (p) { delete[] (p); (p) = NULL;} }
#define DELETE_LIST(p)					{ delete[] (p); (p) = NULL; }
#define FAILED_RETURN(hr)				{ if (FAILED(hr)) return hr; }
#define FAILED_RETURN_FALSE(hr)			{ if (FAILED(hr)) return false; }
#define FALSE_RETURN(r)					{ if (!(r)) return false;}

#define MALLOC(type, size)				(type *)malloc(size*sizeof(type))
#define REALLOC(P, type, size)			P = (type *)realloc(P, size*sizeof(type));

#define _CDECL							__cdecl
#define _STDCALL						__stdcall
#define _FASTCALL						__fastcall
#define MYCALL1							_CDECL

// ͼƬ����
#define ZOOM_MAX						FLT_MAX
#define ZOOM_MIN						0.0f
#define ZOOM_MINDELTA					0.0001f

#define ZOOMFACTOR_DRAG					0.015f //����������
#define ZOOMFACTOR_WHEEL				0.025f
#define ZOOMFACTOR_ZOOMOUTSHRINK		0.92f

#define ZOOM_MAXCONTROLED				0.5f

// ��������
#define SAMPLE_SCHEMA_UNKNOWN			0
#define SAMPLE_SCHEMA_NN				1
#define SAMPLE_SCHEMA_BILINEAR			2
#define SAMPLE_SCHEMA_CUBE				3
#define SAMPLE_SCHEMA_SINGLE			4
#define SAMPLE_SCHEMA_MAX				5

#define NN_MIN							3.0f	//��������Ŵ���ʹ��NN����
#define MAXSAMPLE_WIDTH_UPPERLIMIT		20000	//��������ߴ�ͼƬ������������MAXSAMPLE
#define MAXSAMPLE_HEIGHT_UPPERLIMIT		MAXSAMPLE_WIDTH_UPPERLIMIT

#define MIN_NORMALRENEW_WIDTH			300		//ͼƬС������ߴ磬�ڼ���ʱ������ʹ��SingleSample(̫�����Ź��̿����п���)
#define MIN_NORMALRENEW_HEIGHT			MIN_NORMALRENEW_WIDTH

#define MIN_FORCECLIP_WIDTH				1500	//ͼƬ��������ߴ磬������bClip�����������ٶ�
#define MIN_FORCECLIP_HEIGHT			MIN_FORCECLIP_WIDTH

#define MAX_NOCLIP_WIDTH_DRAG			3000	//ͼƬС������ߴ磬�϶�ͼƬʱȡ��bClip�������϶��ٶ�
#define MAX_NOCLIP_HEIGHT_DRAG			MAX_NOCLIP_WIDTH_DRAG

// ͼƬ�б�
#define PICPACKLIST_SIZE_INIT			3
#define PICPACKLIST_SIZE_INCREMENT		2

// ���λ��
#define CURSORPOS_PIC					0
#define CURSORPOS_BLANK					1
#define CURSORPOS_OUTWINDOW				2

// ��ɫ
#define BACKCOLOR_INIT					0xFF888888
#ifndef SETALPHA
#define SETALPHA(C, A)					((C & 0xFFFFFF) | (A << 24))
#endif

using std::string;
using std::bad_alloc;
using std::endl;
using std::cerr;

typedef float	COLORTYPE;

inline int ceilf2i(float f)
{
	if ((int)f != f)
		return (int)(f + 1);
	else
		return (int)f;
}

struct COLOR_F3 {
	COLORTYPE a;

	COLORTYPE r;
	COLORTYPE g;
	COLORTYPE b;

	COLOR_F3();
	COLOR_F3(COLORTYPE f);

	inline COLOR_F3& operator=(COLOR_F3& cf) {
		a = cf.a;

		r = cf.r;
		g = cf.g;
		b = cf.b;
	}
	inline COLOR_F3& operator=(const COLORTYPE &f) {
		a = r = g = b = f;
		return *this;
	}
	inline void Add(DWORD dw, const COLORTYPE &mul = 1.0, const COLORTYPE &div = 1.0) {
		a += ((dw >> 24) & 0xFF) *mul / div;
		r += (((dw >> 16) & 0xFF)) *mul / div;
		g += ((dw >> 8) & 0xFF) *mul / div;
		b += (dw & 0xFF) *mul / div;
	}
	void Normalize();

	inline DWORD GetDW()
	{
		DWORD res = 0;
		res += (byte)(a + 0.5f) << 24;
		res += (byte)(r + 0.5f) << 16;
		res += (byte)(g + 0.5f) << 8;
		res += (byte)(b + 0.5f);

		return res;
	}
	//inline DWORD AlphaBlend(DWORD blender) {
	//	byte alpha = (byte)a;
	//	DWORD result = 0;
	//	//result += (byte)round(a) << 24;
	//	result += (DWORD)((r*alpha + ((blender >> 16) & 0xFF)*(255 - alpha)) / 255) << 16;
	//	result += (DWORD)((g*alpha + ((blender >> 8) & 0xFF)*(255 - alpha)) / 255) << 8;
	//	result += (DWORD)((b*alpha + (blender & 0xFF)*(255 - alpha)) / 255);
	//	return result;
	//}
};

class ALPHABLEND {
public:
	//�Ե�һ����2�֣�destcolor��������srccolor��alpha��alphablend�Ĳ�ѯ��
	static DWORD lastcolor1, lastresult1;
	static DWORD lastcolor2, lastresult2;
	static byte **alphablendtable1, **alphablendtable2;

	//������destcolor������srccolor����alphablend�Ĳ�ѯ��
	static byte ***alphablendcube1;

	ALPHABLEND() {}

	//����ʾ��
	/*static inline DWORD AlphaBlend1(COLOR_F3 src) {
		byte alpha = roundf2b(src.a);
		DWORD result = 0;
		result += (DWORD)alphablendtable1[(byte)(src.r)][alpha] << 16;
		result += (DWORD)alphablendtable1[(byte)(src.g)][alpha] << 8;
		result += (DWORD)alphablendtable1[(byte)(src.b)][alpha];

		return result;
	}*/
	//����ʾ��
	/*static inline DWORD AlphaBlend1(DWORD srccolor) {
		if (srccolor == ALPHABLEND::lastcolor1)
		{
			return ALPHABLEND::lastresult1;
		}
		else
		{
			byte *src = (byte*)&srccolor;
			byte *dest = (byte*)&lastresult1;

			dest[2] = alphablendtable1[src[2]][src[3]];
			dest[1] = alphablendtable1[src[1]][src[3]];
			dest[0] = alphablendtable1[src[0]][src[3]];

			ALPHABLEND::lastcolor1 = srccolor;
			ALPHABLEND::lastresult1 = *(DWORD*)dest;

			return lastresult1;
		}
	}*/
	static void InitAlphBlendTable(byte color1, byte color2);

	static void InitAlphBlendTCube();
};

inline COLOR_F3 RGB2HSV_F3(DWORD c1)
{
	byte *ppix = (byte*)&c1;
	COLOR_F3 res = 0;
	res.a = ppix[3];

	byte r = ppix[2];
	byte g = ppix[1];
	byte b = ppix[0];
	byte max = 0;
	byte min = 1;
	if (ppix[1] > ppix[0])
	{
		max = 1;
		min = 0;
	}
	if (ppix[2] > ppix[max])
		max = 2;
	else if (ppix[2] < ppix[min])
		min = 2;
	byte minvalue = ppix[min];
	byte maxvalue = ppix[max];

	res.b = maxvalue;//v
	res.g = (maxvalue == 0 ? 0 : 1 - minvalue / (float)maxvalue);//s

	if (maxvalue == minvalue)
		res.r = 0;
	else
	{
		res.r = (maxvalue - r + g - minvalue + b - minvalue) / (float)(maxvalue - minvalue)*60.0f;
		/*if (max == 2)
		{
			result.r = (60.0f * (g - b) / (maxvalue - minvalue));
		}
		else if (max == 1)
		{
			result.r = (120.0f + 60.0f * (b - r) / (maxvalue - minvalue));
		}
		else
		{
			result.r = (240.0f + 60.0f * (r - g) / (maxvalue - minvalue));
		}*/
	}
	if (g < b)
		res.r = 360 - res.r;
	
	return res;
}

inline DWORD RGB2HSV(DWORD color)
{
	byte *pSrc = (byte*)&color;
	DWORD result = 0;
	BYTE* pResult = (byte*)&result;

	byte r = pSrc[2];
	byte g = pSrc[1];
	byte b = pSrc[0];
	byte max = 0;
	byte min = 1;
	if (pSrc[1] > pSrc[0])
	{
		max = 1;
		min = 0;
	}
	if (pSrc[2] > pSrc[max])
		max = 2;
	else if (pSrc[2] < pSrc[min])
		min = 2;
	byte minvalue = pSrc[min];
	byte maxvalue = pSrc[max];

	pResult[0] = maxvalue;//v
	pResult[1] = (maxvalue == 0 ? 0 : ROUNDF_BYTE(255.0f*(maxvalue - minvalue) / maxvalue));//s

	if (maxvalue == minvalue)
		pResult[2] = 0;
	else
	{
		if (max == 2)
		{
			pResult[2] = ROUNDF_BYTE(255.0f * (000.0f + 60.0f * (g - b) / (maxvalue - minvalue)) / 360.0f);
		}
		else if (max == 1)
		{
			pResult[2] = ROUNDF_BYTE(255.0f * (120.0f + 60.0f * (b - r) / (maxvalue - minvalue)) / 360.0f);
		}
		else
		{
			pResult[2] = ROUNDF_BYTE(255.0f * (240.0f + 60.0f * (r - g) / (maxvalue - minvalue)) / 360.0f);
		}
	}
	pResult[3] = pSrc[3];

	return result;
}

inline DWORD HSV2RGB_F3(COLOR_F3 color)
{
	DWORD result = 0;
	byte* pResult = (byte*)&result;
	
	float maxvalue = color.b;
	if (color.g == 0)
	{
		pResult[0] = ROUNDF_BYTE(maxvalue);
		pResult[1] = ROUNDF_BYTE(maxvalue);
		pResult[2] = ROUNDF_BYTE(maxvalue);
	}
	else
	{
		float �� = color.g * color.b;
		float minvalue = maxvalue - ��;
		byte nh = ROUNDF_BYTE(color.r);
		if (color.r > 180)
			nh = 360 - nh;
		float �� = nh / 60.0f * ��;
		float r, g, b;
		r = 2 * �� - ��;
		if (r < 0)
			r = 0;
		if (r > ��)
			r = ��;
		g = ��;
		if (g > ��)
			g = ��;

		b = �� - 2 * ��;
		if (b < 0)
			b = 0;

		if (color.r > 180)
		{
			float temp = b;
			b = g;
			g = temp;
		}

		pResult[0] = ROUNDF_BYTE(b + minvalue);
		pResult[1] = ROUNDF_BYTE(g + minvalue);
		pResult[2] = ROUNDF_BYTE(r + minvalue);

	}
	pResult[3] = ROUNDF_BYTE(color.a);

	return result;
}

inline DWORD HSV2RGB(DWORD color)
{
	byte *pSrc = (byte*)&color;
	DWORD result = 0;
	byte* pResult = (byte*)&result;

	byte h = pSrc[2];
	byte s = pSrc[1];
	byte v = pSrc[0];

	byte maxvalue = v;
	if (s == 0)
	{
		pResult[0] = maxvalue;
		pResult[1] = maxvalue;
		pResult[2] = maxvalue;
	}
	else
	{
		float �� = s * v / 255.0f;
		float minvalue = maxvalue - ��;
		byte nh = h;
		if (h >= 128)
			nh = 255 - h;
		float �� = nh * 6 / 255.0f * ��;
		float r, g, b;
		r = 2 * �� - ��;
		if (r < 0)
			r = 0;
		if (r > ��)
			r = ��;
		g = ��;
		if (g > ��)
			g = ��;

		b = �� - 2 * ��;
		if (b < 0)
			b = 0;

		if (h >= 128)
		{
			float temp = b;
			b = g;
			g = temp;
		}

		pResult[0] = ROUNDF_BYTE(b + minvalue);
		pResult[1] = ROUNDF_BYTE(g + minvalue);
		pResult[2] = ROUNDF_BYTE(r + minvalue);

	}
	pResult[3] = pSrc[3];

	return result;
}

class BMP {
private:
	/*static void __cdecl newhandlerfunc()
	{
		cerr << "Allocate failed in class BMP" << endl;
		abort();
	}*/
public:
	int width, height;
	byte **data;
	bool bAlpha;		//�Ƿ���͸��ͨ��

	BMP();
	~BMP();

	// ����
	void SetAlpha(bool alpha);
	bool Load(LPDIRECT3DSURFACE9 surf);			//��surface���ɶ�Ӧ��СͼƬ
	bool UpLoad(LPDIRECT3DSURFACE9 surf);		//������surface����{0,0}���뿪ʼ�������ܶ�Ŀ���

	/*
	 * �����Ѷ��Ƶ�surface�Ŀ���
	 */
	/***************************************************
	Ŀ���ַ�ռ䣬ÿ��pitch��Ŀ���ַ�ߴ�
	surfBias:surface���ԭͼƫ�ƣ�����ԭͼѡ����
	destBase:surface���client���ڣ����ڸ���͸���������ƣ�

	˫���Բ�ֵ�����ֲ��ԣ���ɫ���ɫ���Sample_Single����ϸ����
	****************************************************/
	// �������ʧ��������СЧ���Ϻã���С�ٶ�������
	inline bool MYCALL1 Sample_Area(DWORD *surfData, int pitch, const POINT &surfSize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		UINT32 zoomW = (UINT32)(width*zoom);
		UINT32 zoomH = (UINT32)(height*zoom);

		COLORTYPE xpixspan = (COLORTYPE)width / zoomW;//������x������
		COLORTYPE ypixspan = (COLORTYPE)height / zoomH;//������y������
		COLORTYPE spanarea = xpixspan*ypixspan;//��������� x*y
		float baseweight = 1.0f / spanarea;
		COLORTYPE *xbase = new COLORTYPE[surfSize.x + 1];//������������ʼ�����飬x����
		COLORTYPE *ybase = new COLORTYPE[surfSize.y + 1];//������������ʼ�����飬y����
		bool *xincolor1 = new bool[surfSize.x + 1];

		// �������������ֵ����,�޶���Ҫ������
		float actualzx = (float)width / zoomW;
		//float actualzy = (float)height / zoomH;
		for (int i = 0; i < surfSize.x + 1; i++)
		{
			xbase[i] = (COLORTYPE)(i + surfbias.x)*actualzx;
			xincolor1[i] = ((backbase.x + i) & TRANSPARENTBACK_MASK) > 0;
		}
		for (int i = 0; i < surfSize.y + 1; i++)
			ybase[i] = (COLORTYPE)(i + surfbias.y)*height / zoomH;//height / zoomH

		int index;
		COLOR_F3 rcolor = 0;//��ɫ�ۼ���
		for (int i = 0; i < surfSize.y; i++)
		{
			int starty = (int)ybase[i];//������������ʼ�㣬y����
			int endy = ceilf2i(ybase[i + 1]);//���������Ͻ����㣬y����
			bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

			for (int j = 0; j < surfSize.x; j++)
			{
				index = i*pitch + j;

				int startx = (int)xbase[j];//������������ʼ�㣬x����
				int endx = ceilf2i(xbase[j + 1]);//���������Ͻ����㣬x����

				rcolor = 0;
				COLORTYPE floatsy, floatey;//��ȷ����յ�

				bool isxbase, isybase;
				// ��Χ��������ɫ�ۼ�
				for (int m = starty; m < endy; m++)
				{
					isybase = true;
					if (m == (int)ybase[i])//�������
					{
						floatsy = ybase[i];
						isybase = false;
					}
					else
					{
						floatsy = (float)m;
					}
					if (ybase[i + 1] - m < 1)//�����յ�
					{
						floatey = ybase[i] + ypixspan;//ybase[srcy] + ypixspan��ybase[srcy + 1]
						isybase = false;
					}
					else
					{
						floatey = (float)m + 1;
					}

					COLORTYPE floatsx, floatex;
					for (int n = startx; n < endx; n++)
					{
						isxbase = true;
						if (n == (int)xbase[j])//�������
						{
							floatsx = xbase[j];
							isxbase = false;
						}
						else
						{
							floatsx = (float)n;
						}
						if (xbase[j + 1] - n < 1)//�����յ�
						{
							floatex = xbase[j] + xpixspan;//xbase[srcx] + xpixspan��xbase[srcx + 1]
							isxbase = false;
						}
						else
						{
							floatex = (float)n + 1;
						}

						//DWORD bmppixel = ((DWORD*)data[m])[n];//ԭ����
						//if (endx - startx == 1 && endy - starty == 1)//��ֹ�Ŵ�󣬴��������ɫ�����ȣ�����������
						//	rcolor.Add(bmppixel);
						//else

						//if (isxbase && isybase)//����Ȩֵ����ļ�������
						//	rcolor.Add(((DWORD*)data[m])[n], baseweight);
						//else
						rcolor.Add(((DWORD*)data[m])[n], (floatey - floatsy) * (floatex - floatsx), spanarea);
					}
				}

				//rcolor.Normalize();

				// ����alphablend����
				if (rcolor.a < 255.0f)
				{
					if (yincolor1 ^ xincolor1[j])
					{// ���ֱ���ɫ
						byte *dest = (byte*)&(surfData[index]);
						byte alpha;
						if (rcolor.a > 0)
							alpha = (byte)(rcolor.a + 0.5f);
						else
							alpha = (byte)(rcolor.a - 0.5f);
						//dest[3] = ALPHABLEND::alphablendtable2[(byte)(rcolor.a + 0.5f)][alpha];
						dest[2] = ALPHABLEND::alphablendtable2[(byte)(rcolor.r + 0.5f)][alpha];
						dest[1] = ALPHABLEND::alphablendtable2[(byte)(rcolor.g + 0.5f)][alpha];
						dest[0] = ALPHABLEND::alphablendtable2[(byte)(rcolor.b + 0.5f)][alpha];
					}
					else
					{
						byte *dest = (byte*)&(surfData[index]);
						byte alpha;
						if (rcolor.a > 0)
							alpha = (byte)(rcolor.a + 0.5f);
						else
							alpha = (byte)(rcolor.a - 0.5f);
						//dest[3] = ALPHABLEND::alphablendtable1[(byte)(rcolor.a + 0.5f)][alpha];
						dest[2] = ALPHABLEND::alphablendtable1[(byte)(rcolor.r + 0.5f)][alpha];
						dest[1] = ALPHABLEND::alphablendtable1[(byte)(rcolor.g + 0.5f)][alpha];
						dest[0] = ALPHABLEND::alphablendtable1[(byte)(rcolor.b + 0.5f)][alpha];
					}
				}
				else
					surfData[index] = rcolor.GetDW();
			}
		}
		delete[] xbase;
		delete[] ybase;

		return true;
	}
	// NN��bilinear��ϣ��罻�紦����ʹ��bilinear���Ż��������˰汾�Ŵ���С������bilinear��
	inline bool MYCALL1 Sample_Single(DWORD *surfData, int pitch, const POINT &surfSize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		const float auxz = 0.3f / zoom;//����
		float auxfront = 0.5f - auxz, auxrear = 0.5f + auxz;//����

		float *xsto = new float[surfSize.x];//����,��ֹ���ڶ���ѭ����x�ظ�����
		bool *xincolor1 = new bool[surfSize.x];
		for (int j = 0; j < surfSize.x; j++)//��ǰ����x����ı���
		{
			xsto[j] = (j + surfbias.x + 0.5f) / zoom - 0.5f;
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (bAlpha)// ��alphaͨ��
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				// y�������
				float fronty = (i + surfbias.y + 0.5f) / zoom - 0.5f;
				float frontybias = fronty - (int)fronty;
				if (frontybias < auxfront)
					frontybias = 0;
				else if (frontybias > auxrear)
					frontybias = 1;
				else
					frontybias = (frontybias - auxfront) * zoom;
				float rearybias = 1 - frontybias;

				int fy, ry;
				fy = (int)fronty;
				ry = fy + 1;
				
				if (ry > height - 1)
					ry -= 1;
				/*if (fronty < 0)
				frontybias = 0;*/
				DWORD *datafy = (DWORD*)data[fy];
				DWORD *datary = (DWORD*)data[ry];

				bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

				for (int j = 0; j < surfSize.x; j++)
				{
					index = i*pitch + j;

					float frontx = xsto[j];
					float frontxbias = frontx - (int)frontx;
					/*if (frontx < 0)//���Ӳ��ᱨ��
					frontxbias = 0;*/
					if (frontxbias < auxfront)
						frontxbias = 0;
					else if (frontxbias > auxrear)
						frontxbias = 1;
					else
						frontxbias = (frontxbias - auxfront) * zoom;

					byte pixel[4];
					if ((frontxbias == 0 || frontxbias == 1)
						&& (frontybias == 0 || frontybias == 1))
					{
						//memcpy_s(pixel, 4
						//	, ((byte*)&((DWORD*)data[(int)(fronty + frontybias)])[(int)(frontx + frontxbias)]), 4);
						*(DWORD*)pixel = ((DWORD*)data[(int)(fronty + frontybias)])[(int)(frontx + frontxbias)];
					}
					else
					{
						// ��Ӧԭͼ��Χ4����x�±�
						int fx, rx;
						fx = (int)xsto[j];
						rx = fx + 1;
						if (rx > width - 1)//��������Խ��
							rx -= 1;
						// ��Ӧԭͼ��Χ4����
						byte *lt, *rt, *lb, *rb;
						lt = (byte*)&datafy[fx];
						rt = (byte*)&datafy[rx];
						lb = (byte*)&datary[fx];
						rb = (byte*)&datary[rx];

						// 2�β�ֵ
						float rearxbias = 1 - frontxbias;

						float fxb, rxb, fxb2, rxb2, fyb, ryb, norm1, norm2, norm3;
						fxb = frontxbias * rt[3];
						rxb = rearxbias * lt[3];
						norm1 = fxb + rxb;
						if (norm1 == 0.0f)
						{
							fxb = rxb = 0.5;
						}
						else
						{
							fxb /= norm1;
							rxb /= norm1;
						}

						fxb2 = frontxbias * rb[3];
						rxb2 = rearxbias * lb[3];
						norm2 = fxb2 + rxb2;
						if (norm2 == 0.0f)
						{
							fxb2 = rxb2 = 0.5f;
						}
						else
						{
							fxb2 /= norm2;
							rxb2 /= norm2;
						}

						fyb = frontybias * norm2;
						ryb = rearybias * norm1;
						norm3 = fyb + ryb;
						if (norm3 == 0.0f)
						{
							fyb = ryb = 0.5f;
						}
						else
						{
							fyb /= norm3;
							ryb /= norm3;
						}
						// --#3--���ٳ��ԣ��Կ�
						float auxrxry = rxb*ryb, auxfxry = fxb*ryb
							, auxrxfy = rxb2*fyb, auxfxfy = fxb2*fyb;
						pixel[0] = (byte)(lt[0] * auxrxry + rt[0] * auxfxry
							+ lb[0] * auxrxfy + rb[0] * auxfxfy);
						pixel[1] = (byte)(lt[1] * auxrxry + rt[1] * auxfxry
							+ lb[1] * auxrxfy + rb[1] * auxfxfy);
						pixel[2] = (byte)(lt[2] * auxrxry + rt[2] * auxfxry
							+ lb[2] * auxrxfy + rb[2] * auxfxfy);

						// alphaֻ����λ�ñ�������
						pixel[3] = (byte)(((lt[3] * rearxbias + rt[3] * frontxbias) * rearybias
							+ (lb[3] * rearxbias + rb[3] * frontxbias) * frontybias));
					}
					//bmppixel = *((DWORD*)&pixel);//����Ч�ʽ���

					// ����alphablend����
					if (pixel[3] < (byte)255)
					{
						if (yincolor1 ^ xincolor1[j])
						{// ���ֱ���ɫ
							if (*((DWORD*)&pixel) == ALPHABLEND::lastcolor2)//�������
							{
								surfData[index] = ALPHABLEND::lastresult2;
							}
							else
							{
								byte *dest = ((byte*)&surfData[index]);

								dest[2] = ALPHABLEND::alphablendtable2[pixel[2]][pixel[3]];
								dest[1] = ALPHABLEND::alphablendtable2[pixel[1]][pixel[3]];
								dest[0] = ALPHABLEND::alphablendtable2[pixel[0]][pixel[3]];

								ALPHABLEND::lastcolor2 = *((DWORD*)&pixel);
								ALPHABLEND::lastresult2 = surfData[index];
							}
						}
						else
						{
							if (*((DWORD*)&pixel) == ALPHABLEND::lastcolor1)//�������
							{
								surfData[index] = ALPHABLEND::lastresult1;
							}
							else
							{
								byte *dest = ((byte*)&surfData[index]);

								dest[2] = ALPHABLEND::alphablendtable1[pixel[2]][pixel[3]];
								dest[1] = ALPHABLEND::alphablendtable1[pixel[1]][pixel[3]];
								dest[0] = ALPHABLEND::alphablendtable1[pixel[0]][pixel[3]];

								ALPHABLEND::lastcolor1 = *((DWORD*)&pixel);
								ALPHABLEND::lastresult1 = surfData[index];
							}
						}
					}
					else
						surfData[index] = *((DWORD*)&pixel);
				}
			}
		}
		else//��alphaͨ��
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				// y��������
				float fronty = (i + surfbias.y + 0.5f) / zoom - 0.5f;
				float frontybias = fronty - (int)fronty;
				/*if (fronty < 0)
				frontybias = 0;*/
				if (frontybias < auxfront)
					frontybias = 0;
				else if (frontybias > auxrear)
					frontybias = 1;
				else
					frontybias = (frontybias - auxfront) * zoom;
				float rearybias = 1 - frontybias;

				// ԭͼ��Χ4�����±�
				int fy = (int)fronty;
				int ry = fy + 1;
				if (ry > height - 1)
					ry -= 1;
				DWORD* datafy = (DWORD*)data[fy];
				DWORD* datary = (DWORD*)data[ry];

				for (int j = 0; j < surfSize.x; j++)
				{
					float frontx = xsto[j];

					float frontxbias = frontx - (int)frontx;
					/*if (frontx < 0)//���Ӳ��ᱨ��
					frontxbias = 0;*/
					if (frontxbias < auxfront)
						frontxbias = 0;
					else if (frontxbias > auxrear)
						frontxbias = 1;
					else
						frontxbias = (frontxbias - auxfront) * zoom;
					float rearxbias = 1 - frontxbias;

					//DWORD bmppixel;
					byte pixel[4];
					if ((frontxbias == 0 || frontxbias == 1)
						&& (frontybias == 0 || frontybias == 1))
					{
						//memcpy_s(pixel, 4
						//	, ((byte*)&((DWORD*)data[(int)(fronty + frontybias)])[(int)(frontx + frontxbias)]), 4);
						*(DWORD*)pixel = ((DWORD*)data[(int)(fronty + frontybias)])[(int)(frontx + frontxbias)];
					}
					else
					{
						byte *lt, *rt, *lb, *rb;
						// ԭͼ��Χ4�����±�
						int fx = (int)frontx;
						int rx = fx + 1;
						if (rx > width - 1)//��������Խ��
							rx -= 1;
						// ԭͼ��Χ4����
						lt = (byte*)&datafy[fx];
						rt = (byte*)&datafy[rx];
						lb = (byte*)&datary[fx];
						rb = (byte*)&datary[rx];

						// 2�β�ֵ-��ȥɫ�
						// --#1--��ȥɫ���rgbֵ������λ�ñ�������
						float auxrxry = rearxbias*rearybias, auxfxry = frontxbias*rearybias
							, auxrxfy = rearxbias*frontybias, auxfxfy = frontxbias*frontybias;
						pixel[0] = (byte)(lt[0] * auxrxry + rt[0] * auxfxry
							+ lb[0] * auxrxfy + rb[0] * auxfxfy);
						pixel[1] = (byte)(lt[1] * auxrxry + rt[1] * auxfxry
							+ lb[1] * auxrxfy + rb[1] * auxfxfy);
						pixel[2] = (byte)(lt[2] * auxrxry + rt[2] * auxfxry
							+ lb[2] * auxrxfy + rb[2] * auxfxfy);
						pixel[3] = (byte)(lt[3] * auxrxry + rt[3] * auxfxry
							+ lb[3] * auxrxfy + rb[3] * auxfxfy);
					}

					surfData[i*pitch + j] = *((DWORD*)&pixel);
				}
			}
		}
		delete[] xsto;
		delete[] xincolor1;

		return true;
	}
	// ����ڽ����Ŵ���С�����ԣ�Ч������̫�ã�
	inline bool MYCALL1 Sample_NN(DWORD *surfData, int pitch, const POINT &surfSize
		, float zoom, float rotate = 0, const POINT surfBias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		int *bxarray = new int[surfSize.x];
		bool *xincolor1 = new bool[surfSize.x];
		for (int j = 0; j < surfSize.x; j++)//����
		{
			bxarray[j] = (int)(min((j + surfBias.x + 0.5f) / zoom, width - 1));
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (bAlpha)
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				int by = (int)(min((i + surfBias.y + 0.5f) / zoom, height - 1));
				DWORD *databy = (DWORD*)data[by];

				bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

				for (int j = 0; j < surfSize.x; j++)
				{
					index = i*pitch + j;

					int bx = bxarray[j];
					DWORD bmppixel = databy[bx];//��ȡpBmp��Ϣʱ����surfaceƫ��

												//����alphablend����
					if ((byte)(bmppixel >> 24) < 255)
					{
						if (yincolor1 ^ xincolor1[j])
						{//���ֱ���ɫ
							if (bmppixel == ALPHABLEND::lastcolor2)//�������
							{
								surfData[index] = ALPHABLEND::lastresult2;
							}
							else
							{
								byte *src = (byte*)&bmppixel;
								byte *dest = (byte*)&(surfData[index]);

								dest[2] = (DWORD)ALPHABLEND::alphablendtable2[src[2]][src[3]];
								dest[1] = (DWORD)ALPHABLEND::alphablendtable2[src[1]][src[3]];
								dest[0] = (DWORD)ALPHABLEND::alphablendtable2[src[0]][src[3]];

								ALPHABLEND::lastcolor2 = bmppixel;
								ALPHABLEND::lastresult2 = *(DWORD*)dest;
							}
						}
						else
						{
							if (bmppixel == ALPHABLEND::lastcolor1)//�������
							{
								surfData[index] = ALPHABLEND::lastresult1;
							}
							else
							{
								byte *src = (byte*)&bmppixel;
								byte *dest = (byte*)&(surfData[index]);

								dest[2] = (DWORD)ALPHABLEND::alphablendtable1[src[2]][src[3]];
								dest[1] = (DWORD)ALPHABLEND::alphablendtable1[src[1]][src[3]];
								dest[0] = (DWORD)ALPHABLEND::alphablendtable1[src[0]][src[3]];

								ALPHABLEND::lastcolor1 = bmppixel;
								ALPHABLEND::lastresult1 = *(DWORD*)dest;
							}
						}
					}
					else
						surfData[index] = bmppixel;
				}
			}
		}
		else
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				int by = (int)(min((i + surfBias.y + 0.5f) / zoom, height - 1));
				DWORD *databy = (DWORD*)data[by];
				for (int j = 0; j < surfSize.x; j++)
				{
					int bx = bxarray[j];

					//copypixel����
					surfData[i*pitch + j] = databy[bx];
				}
			}
		}
		delete[] bxarray;
		delete[] xincolor1;

		return true;
	}
	// ˫���ԣ���С�����Ŵ����Ч���
	inline bool MYCALL1 Sample_BiLinear(DWORD *surfData, int pitch, const POINT &surfSize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		float *xsto = new float[surfSize.x];//����,��ֹ���ڶ���ѭ����x�ظ�����
		bool *xincolor1 = new bool[surfSize.x];
		for (int j = 0; j < surfSize.x; j++)//��ǰ����x����ı������ռ�����
		{
			xsto[j] = (j + surfbias.x + 0.5f) / zoom - 0.5f;
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (bAlpha)
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				float fronty = (i + surfbias.y + 0.5f) / zoom - 0.5f;
				float frontybias = fronty - (int)fronty;
				/*if (fronty < 0)
				frontybias = 0;*/
				float rearybias = 1 - frontybias;

				int fy = (int)fronty;
				int ry = fy + 1;
				if (ry > height - 1)//��������Խ��
					ry -= 1;
				DWORD* datafy = (DWORD*)data[fy];
				DWORD* datary = (DWORD*)data[ry];

				bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

				for (int j = 0; j < surfSize.x; j++)
				{
					index = i*pitch + j;

					// ��ӦԭͼƬ��λ��
					float frontx = xsto[j];
					float frontxbias = frontx - (int)frontx;
					/*if (frontx < 0)
					frontxbias = 0;*/
					float rearxbias = 1 - frontxbias;

					byte *lt, *rt, *lb, *rb;
					// ��Ӧԭͼ��Χ4�����±�
					int fx = (int)frontx;
					int rx = fx + 1;
					if (rx > width - 1)//��������Խ��
						rx -= 1;
					// ��Ӧԭͼ��Χ4����
					lt = (byte*)&datafy[fx];
					rt = (byte*)&datafy[rx];
					lb = (byte*)&datary[fx];
					rb = (byte*)&datary[rx];

					// 2�β�ֵ-��ɫ�
					float fxb, rxb, fxb2, rxb2, fyb, ryb, norm1, norm2, norm3;
					fxb = frontxbias * rt[3];
					rxb = rearxbias * lt[3];
					norm1 = fxb + rxb;
					if (norm1 == 0.0f)
					{
						fxb = rxb = 0.5;
					}
					else
					{
						fxb /= norm1;
						rxb /= norm1;
					}

					fxb2 = frontxbias * rb[3];
					rxb2 = rearxbias * lb[3];
					norm2 = fxb2 + rxb2;
					if (norm2 == 0.0f)
					{
						fxb2 = rxb2 = 0.5f;
					}
					else
					{
						fxb2 /= norm2;
						rxb2 /= norm2;
					}

					fyb = frontybias * norm2;
					ryb = rearybias * norm1;
					norm3 = fyb + ryb;
					if (norm3 == 0.0f)
					{
						fyb = ryb = 0.5f;
					}
					else
					{
						fyb /= norm3;
						ryb /= norm3;
					}
					// --#3--���ٳ��ԣ��Կ�
					byte pixel[4];
					float auxrxry = rxb*ryb, auxfxry = fxb*ryb, auxrxfy = rxb2*fyb, auxfxfy = fxb2*fyb;
					pixel[0] = (byte)(lt[0] * auxrxry + rt[0] * auxfxry
						+ lb[0] * auxrxfy + rb[0] * auxfxfy);
					pixel[1] = (byte)(lt[1] * auxrxry + rt[1] * auxfxry
						+ lb[1] * auxrxfy + rb[1] * auxfxfy);
					pixel[2] = (byte)(lt[2] * auxrxry + rt[2] * auxfxry
						+ lb[2] * auxrxfy + rb[2] * auxfxfy);

					// alphaֻ����λ�ñ�������
					pixel[3] = (byte)(((lt[3] * rearxbias + rt[3] * frontxbias) * rearybias
						+ (lb[3] * rearxbias + rb[3] * frontxbias) * frontybias));

					// ����alphablend����
					if (pixel[3] < 255)
					{
						if (yincolor1 ^ xincolor1[j])
						{//���ֱ���ɫ
							if (*((DWORD*)&pixel) == ALPHABLEND::lastcolor2)//�������
							{
								surfData[index] = ALPHABLEND::lastresult2;
							}
							else
							{
								byte *dest = (byte*)&(surfData[index]);
								//byte *src = pixel;

								dest[2] = ALPHABLEND::alphablendtable2[pixel[2]][pixel[3]];
								dest[1] = ALPHABLEND::alphablendtable2[pixel[1]][pixel[3]];
								dest[0] = ALPHABLEND::alphablendtable2[pixel[0]][pixel[3]];

								ALPHABLEND::lastcolor2 = *((DWORD*)&pixel);
								ALPHABLEND::lastresult2 = surfData[index];
							}
						}
						else
						{
							if (*((DWORD*)&pixel) == ALPHABLEND::lastcolor1)//�������
							{
								surfData[index] = ALPHABLEND::lastresult1;
							}
							else
							{
								byte *dest = (byte*)&(surfData[index]);
								//byte *src = pixel;

								dest[2] = ALPHABLEND::alphablendtable1[pixel[2]][pixel[3]];
								dest[1] = ALPHABLEND::alphablendtable1[pixel[1]][pixel[3]];
								dest[0] = ALPHABLEND::alphablendtable1[pixel[0]][pixel[3]];

								ALPHABLEND::lastcolor1 = *((DWORD*)&pixel);
								ALPHABLEND::lastresult1 = surfData[index];
							}
						}
					}
					else
						surfData[index] = *((DWORD*)&pixel);
				}
			}
		}
		else
		{
			for (int i = 0; i < surfSize.y; i++)
			{
				float fronty = (i + surfbias.y + 0.5f) / zoom - 0.5f;
				float frontybias = fronty - (int)fronty;
				/*if (fronty < 0)//���Ӳ�����
				frontybias = 0;*/
				float rearybias = 1 - frontybias;

				int fy = (int)fronty;
				int ry = fy + 1;
				if (ry > height - 1)//��������Խ��
					ry -= 1;
				DWORD* datafy = (DWORD*)data[fy];
				DWORD* datary = (DWORD*)data[ry];

				for (int j = 0; j < surfSize.x; j++)
				{
					// ��ӦԭͼƬ��λ��
					float frontx = xsto[j];
					float frontxbias = frontx - (int)frontx;
					/*if (frontx < 0)
					frontxbias = 0;*/
					float rearxbias = 1 - frontxbias;

					byte *lt, *rt, *lb, *rb;
					// ��Ӧԭͼ��Χ4�����±�
					int fx = (int)frontx;
					int rx = fx + 1;
					if (rx > width - 1)//��������Խ��
						rx -= 1;
					// ��Ӧԭͼ��Χ4����
					lt = (byte*)&datafy[fx];
					rt = (byte*)&datafy[rx];
					lb = (byte*)&datary[fx];
					rb = (byte*)&datary[rx];

					// 2�β�ֵ-��ȥɫ�  >90%��ʱ�仨����4������
					byte pixel[4];
					float auxrxry = rearxbias*rearybias, auxfxry = frontxbias*rearybias
						, auxrxfy = rearxbias*frontybias, auxfxfy = frontxbias*frontybias;
					pixel[0] = (byte)((lt[0] * auxrxry + rt[0] * auxfxry
						+ lb[0] * auxrxfy + rb[0] * auxfxfy));
					pixel[1] = (byte)((lt[1] * auxrxry + rt[1] * auxfxry
						+ lb[1] * auxrxfy + rb[1] * auxfxfy));
					pixel[2] = (byte)((lt[2] * auxrxry + rt[2] * auxfxry
						+ lb[2] * auxrxfy + rb[2] * auxfxfy));
					pixel[3] = (byte)((lt[3] * auxrxry + rt[3] * auxfxry
						+ lb[3] * auxrxfy + rb[3] * auxfxfy));

					// copypixel����
					surfData[i*pitch + j] = *((DWORD*)&pixel);
				}
			}
		}
		delete[] xsto;
		delete[] xincolor1;

		return true;
	}

	/*
	 * ͼƬ����
	 */
	inline void TestInc()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				byte *ppix = (byte*)&((DWORD*)data[i])[j];
				ppix[0]++;
				ppix[1]++;
				ppix[2]++;
			}
		}
	}
	bool MYCALL1 Gray();

	bool MYCALL1 Inverse();
	bool MYCALL1 InverseAlpha();
	bool MYCALL1 InverseAll();

	bool MYCALL1 LOSE_R();
	bool MYCALL1 LOSE_G();
	bool MYCALL1 LOSE_B();

	bool MYCALL1 RGB2HSV();

	// ��Ϣ
	inline bool MYCALL1 GetPixel(int x, int y, DWORD *dest)
	{
		if (x < 0 || y < 0 || x >= width || y >= height)
			return false;

		if (!data)
			return false;

		if (!data[y])
			return false;

		*dest = ((DWORD*)data[y])[x];

		return true;
	}

	// ״̬
	void MYCALL1 Clear();
	inline bool MYCALL1 isEmpty()
	{
		return (bool)!data;
	}
	inline bool MYCALL1 isNotEmpty()
	{
		return (data != NULL);
	}
};


string GetFMTStr(D3DFORMAT);

class PicPack {
private:
	BMP *pBmp;
	PicInfo myPicInfo;				//ͼƬ��Ϣ
	D3DXIMAGE_INFO D3DPicInfo;		//�Զ�ͼƬ��Ϣ,TODO������
	WCHAR strFileName[MAX_PATH];	//�ļ���

	bool bSaved;
	bool bHasDirectory;

	// ��surface�������Ϣ
	POINT src;
	float zoom;

	// ��Ϣ
	WCHAR picInfoStr[256];
public:
	PicPack();
	~PicPack();

	inline bool isEmpty()
	{
		return pBmp->isEmpty();
	}
	inline bool isNotEmpty()
	{
		return pBmp->isNotEmpty();
	}
	inline float GetZoom()
	{
		return zoom;
	}
	inline void SetZoom(float z)
	{
		zoom = z;
	}
	inline POINT GetBase()
	{
		return src;
	}
	inline void SetSrc(POINT point)
	{
		src = point;
	}
	inline BMP* GetPBMP()
	{
		return pBmp;
	}
	WCHAR *GetFileName();

	HRESULT LoadFile(LPDIRECT3DDEVICE9 pDevice, WCHAR[]);
	bool SaveFile(LPDIRECT3DDEVICE9 pDevice, WCHAR[]);

	WCHAR *GetPicInfoStrW();
};

class PicPackList {
private:
	PicPack **data;
	int size;
	int count;

	int cur;
public:
	PicPack *pLivePicPack;
	BMP *pLiveBMP;

	PicPackList();

	bool isEmpty()
	{
		return count == 0;
	}
	int GetCurPos();
	int GetCount();
	PicPack *SetPicPack(int idx);
	PicPack *SetTailPicPack();
	PicPack *SetPrev();
	PicPack *SetNext();
	PicPack *Drop();
	bool Add(PicPack *newpp);
};

class SurferBase {
protected:
	BMP *pBmp;								//BMPָ�롤����
	UINT *pBufferW, *pBufferH;				//[INT64]Ŀ��surface�ߴ硤����
	bool bHasPic;
	bool bNeedRenew;

	// ��ʱ
	LARGE_INTEGER wheeltick, lastwheeltick;	//���ּ�ʱ
	LARGE_INTEGER freq;						//Ƶ��

	// ����
	POINT ptBase;							//[INT64]����dragzoom�����ĵ�
	double ptScaleX;						//�������ŵı�����
	double ptScaleY;
	
public:
	// ӳ�����
	float			zoom;					//��Ŵ���
	float			lastZoom;				//����һ���Ŵ���
	INT64			zoomW, zoomH;			//�Ŵ�zoom����surface�ĳߴ磨��clip�����������������ʱ����������Ҫ�ֶ����¡�(��Ҫ�ֶ����³��ϣ����ţ�����ͼƬ)
	// ӳ��״̬
	float			actualZoomX, actualZoomY;//ʵ��x,y�����ϵķŴ�������ʱֻ�����鿴��

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ����
	POINT			surfBase;				//[INT64]��Ŵ�zoom���ļ���surface��Դ��ڿͻ�����λ�ã����Ͻǣ�
	// ӳ��״̬
	POINT			oldSurfSize;			//[INT64]�洢���һ��surfRenewʱ�ĳߴ�
	POINT			surfSize;				//[INT64]��surface�ߴ�
	bool			bPicClipped;			//��ǰͼƬ�Ƿ�clip(bPicClipped��bOutClientֻ���ض������м��㣬��Ҫȷ��������)
	bool			bOutClient;				//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩

	// "BMP" ӳ�� "SURFACE" ѡ��
	bool			bClip;					//��surface�Ƿ�clip����ȥ�ͻ���������

	SurferBase();

	inline POINT	GetBase() const;
	inline float	GetZoom() const;
	inline INT64	GetZoomWidth() const;
	inline INT64	GetZoomHeight() const;
	inline void		SetClip(bool clip);
	// surfaceˢ��ǰ��������λ���Ų����Ĳ���׼��
	inline void		SetBasePoint(POINT ptB);
	inline void		SurfHomePR();		//surface��ԭ��
	inline void		SurfLocatePR(LONG x, LONG y);	//surface��λ
	inline void		SurfLocatePR(POINT base);
	inline void		SurfCenterPR(int bufferW, int bufferH);
	inline void		SurfMovePR(int dx, int dy);
	//��3����������һ������������ű��ʵ�ֵ��������zoomW,zoomH�ȣ�ʵ��������Ҫ��һ������������һ��������ʹ�õ�����ı��ʣ�
	inline void		SurfSetZoomPR(float zoom);// ����surface���ű���
	//��������������zoom�����������޸�
	void			SurfAdjustZoom_DragPR(int wParam);
	void			SurfAdjustZoom_WheelPR(int wParam);
protected:
	inline void		CalcBindState();
	inline void		CalcSurfSize();
	inline void		PostSurfPosChange();
	inline void		PostZoomChange();
};

inline POINT SurferBase::GetBase() const
{
	return surfBase;
}

inline float SurferBase::GetZoom() const
{
	return zoom;
}

inline INT64 SurferBase::GetZoomWidth() const
{
	return zoomW;
}

inline INT64 SurferBase::GetZoomHeight() const
{
	return zoomH;
}

inline void SurferBase::SetClip(bool clip)
{
	if (bClip != clip)
	{
		bClip = clip;

		CalcSurfSize();
	}
}

inline void SurferBase::SetBasePoint(POINT ptB)
{
	ptBase = ptB;

	POINT surf2cursor;
	surf2cursor.x = -surfBase.x + ptBase.x;
	surf2cursor.y = -surfBase.y + ptBase.y;

	if (zoomW != 0)
		ptScaleX = surf2cursor.x / (double)zoomW;
	else
		ptScaleX = 0;
	if (zoomH != 0)
		ptScaleY = surf2cursor.y / (double)zoomH;
	else
		ptScaleY = 0;
}

inline void SurferBase::SurfHomePR()
{
	surfBase.x = 0;
	surfBase.y = 0;

	PostSurfPosChange();
}

inline void SurferBase::SurfLocatePR(LONG x, LONG y)
{
	surfBase.x = x;
	surfBase.y = y;

	PostSurfPosChange();
}

inline void SurferBase::SurfLocatePR(POINT base)
{
	surfBase = base;

	PostSurfPosChange();
}

inline void SurferBase::SurfCenterPR(int bufferW, int bufferH)
{
	surfBase.x = (LONG)(bufferW / 2 - zoomW / 2);
	surfBase.y = (LONG)(bufferH / 2 - zoomH / 2);

	PostSurfPosChange();
}

inline void SurferBase::SurfMovePR(int dx, int dy)
{
	surfBase.x += dx;
	surfBase.y += dy;

	PostSurfPosChange();
}

inline void SurferBase::SurfSetZoomPR(float zoom)
{
	this->zoom = zoom;
	PostZoomChange();
}

inline void SurferBase::CalcBindState()
{
	bHasPic = (pBmp != NULL)/* && pBmpW && pBmpH*/;
}

inline void SurferBase::CalcSurfSize()
{
	if (pBufferW != NULL && pBufferH != NULL)
	{
		SETPOINT(surfSize, (LONG)zoomW, (LONG)zoomH);
		if (bClip)
		{
			// surface���clip
			if (surfBase.x < 0)
				surfSize.x += surfBase.x;
			// surface�ϲ�clip
			if (surfBase.y < 0)
				surfSize.y += surfBase.y;
			// surface�Ҳ�clip
			if (surfBase.x + (LONG)zoomW > (INT64)*pBufferW)
				surfSize.x -= surfBase.x + zoomW - (INT64)*pBufferW;
			// surface�²�clip
			if (surfBase.y + (LONG)zoomH > (INT64)*pBufferH)
				surfSize.y -= surfBase.y + zoomH - (INT64)*pBufferH;
		}
	}
}

inline void SurferBase::PostSurfPosChange()
{
	bNeedRenew = true;
	if (pBufferW != NULL && pBufferH != NULL)
	{
		bPicClipped = (
			surfBase.x < 0 || surfBase.y < 0			//surface�����ϲ���bClip
			|| surfBase.x + zoomW  >(INT64)*pBufferW		//surface�Ҳ���bClip
			|| surfBase.y + zoomH  >(INT64)*pBufferH		//surface�²���bClip
			);
		bOutClient = (
			surfBase.x < -zoomW || surfBase.y < -zoomH	//���򳬳�surface�һ���
			|| surfBase.x >= (INT64)*pBufferW				//�������²���surface�����
			|| surfBase.y >= (INT64)*pBufferH
			);

		CalcSurfSize();
	}
}

inline void	SurferBase::PostZoomChange()
{
	bNeedRenew = true;
	if (bHasPic)
	{
		zoomW = (int)(zoom*pBmp->width);
		zoomH = (int)(zoom*pBmp->height);
		actualZoomX = (float)zoomW / pBmp->width;
		actualZoomY = (float)zoomH / pBmp->height;
	}

	PostSurfPosChange();
}

class Surfer :public SurferBase {
protected:
	LPDIRECT3DSURFACE9 surf;				//surface�����ɵ�ͼƬ
	LPDIRECT3DDEVICE9 pDevice;				//�豸ָ�롤����
	bool bHasDevice, bHasBoth;				//�Ƿ�߱�����ֵ���������

public:
	// ��Ϣ
	//int surfrenewcount;					//surface���¼���
	float renewTime;						//surface���º�ʱ
	UINT8 sampleSchema;						//��������
	WCHAR strSurfInfo[512];					//��Ϣ�ַ���

	/******************************************************************************
	surfBase�����ɱ������߼��Ͽɷ��룬��Ϊ����BMP����Ϣ��
	surfBase��pBmp����bClip,zoom��Ǩ�ƹ�������SurfRenew֮ǰ�������:zoomW��zoomH��lastZoom&��Ϣ
	*******************************************************************************/

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ֱ�Ӳ���
	POINT			surfDest;				//��surface������backbuffer, backbuffer����ʼ��
	RECT			rcSurf;					//��surfaceҪ������backbuffer������

	// "BMP" ӳ�� "SURFACE" ״̬
	bool			bSurfFailed;			//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp����¡���ʱֻ�����鿴����Ҫ�ֶ����¡�
	bool			bSurfClipped;			//��ǰsurface�Ƿ�bClip

	// �����Ϣ
	POINT			cursorPixel;			//��굱ǰλ�ö�Ӧԭͼ������
	bool			bCursorOnPic;			//����Ƿ���ͼƬ��
	DWORD			cursorColor;			//��굱ǰλ�ö�Ӧԭͼ����ɫ
	UINT8			cursorPos;				//��ǰ���λ�ã����࣬�����ĸ�����ͼƬ���հס������⣩

private:
	inline void		CalcBindState();		// ���������־�������״̬��
	inline void		CalcMapInfo();			// ��bHasDevice������ӳ�䵽Ŀ��surface����

public:
	Surfer();
	~Surfer();

	inline bool		IsSurfNull() const
	{
		return surf == NULL;
	}
	inline bool		IsSurfNotNull() const
	{
		return surf != NULL;
	}

// �豸
	bool			BindDevice(LPDIRECT3DDEVICE9 device);//�����豸
	bool			BindBuf(UINT * pBufW, UINT *pBufH);//����Ŀ��surface�ߴ�

// ͼƬ
	bool			BindPic(PicPack *ppic, bool renew = true);//����ͼƬ����������NULL������������/�л�ͼƬ��
	bool			DeBindPic(PicPack *ppic);			//���ͼƬ
	void			Clear();							// ���,�ͷ���ͼƬ������

//surface���ţ�ǿ������ˢ�²�����
	// ��bHasBoth�����ݵ���ֵ���ţ����趨λ����������ӳ�����������surface�Ƿ�ˢ��
	inline bool		SurfZoomRenew(float zoom = -1, int dx = 0, int dy = 0);
	// ��bHasBoth���������趨�����ű���ֵ���ţ�����ӳ�����������surface�Ƿ�ˢ��
	inline bool		SurfZoomRenew(POINT *client2cursor, bool stable = false, bool acce = false);

//surfaceˢ��
	// ��bHasBoth��surfaceˢ�£�����surface�Ƿ�ˢ��
	inline bool		SurfRenew(bool acce = false);

	// ��surf����Ⱦ
	inline HRESULT	Render() const;

//surface���Ʋ���
	// ��bHasBoth���Զ�����ק�¼�����(����bClip��surfaceλ�õ�״̬�Զ�����surface�Ƿ����)������surface�Ƿ�ˢ��
	inline bool		OnMove_Custom(POINT cursorbias);
	// ��bHasBoth���Զ��崰�������¼�����(����bClip��surfaceλ�õ�״̬�Զ�����surface�Ƿ����)������surface�Ƿ�ˢ��
	inline bool		OnWinsize_Custom();//��OnMove_Custom���
	bool			SurfSuit(int w, int h);

// ��Ϣ
	//��¼��ǰ״̬��Ϣ�������ַ�����
	void			SetInfoStr();
	//���µ�ǰ��Ϣ����ǰ���λ�ö�ӦͼƬ�����أ�����Ϣ�ַ���
	void			GetCurInfo(POINT *cursor, RECT *rcClient);
	//�����Ϣ�ַ���
	inline const WCHAR *GetInfoStr() const
	{
		return strSurfInfo;
	}
	//�����һ������surface�Ĳ����㷨
	char*			GetSampleSchemaStr();
};

inline bool Surfer::SurfZoomRenew(float zoom, int dx, int dy)
{
	if (bHasBoth)
	{
		if (lastZoom == zoom && dx == 0 && dy == 0)
			return false;
		if (zoom < 0)
			return false;

		//#1������ʼ��
		SurfMovePR(dx, dy);

		//#2����zoomֵ
		SurfSetZoomPR(zoom);

		//���±�־������
		SetClip(true);
		lastZoom = zoom;

		//#3����surface
		return SurfRenew(false);
	}
	else
		return false;
}

inline bool Surfer::SurfZoomRenew(POINT * client2cursor, bool stable, bool acce)
{
	if (bHasBoth)
	{
		if (lastZoom == zoom)
			return false;

		//������ʼ��#1
		if (client2cursor == NULL)//�������û�ṩ��׼�㣬����ptBase��Ϊ��׼��
			client2cursor = &ptBase;
		if (client2cursor != NULL)
		{
			if (stable)
			{
				surfBase.x = ROUND(client2cursor->x - zoomW*0.5);
				surfBase.y = ROUND(client2cursor->y - zoomH*0.5);
			}
			else
			{
				surfBase.x = ROUND(client2cursor->x - zoomW*ptScaleX);
				surfBase.y = ROUND(client2cursor->y - zoomH*ptScaleY);
			}
		}

		//����ڷŴ�׶Σ����ڿͻ���������ȫ����surface�������surface��ʾ����
		//��ֹСͼƬ�Ŵ�Ѹ��ƫ�봰�ڿͻ���
		if (zoomW <= (INT64)*pBufferW
			&& zoomH <= (INT64)*pBufferH
			&& zoom > lastZoom
			&& zoom < ZOOM_MAXCONTROLED)
		{
			if (surfBase.x < 0)
				surfBase.x = 0;
			if (surfBase.y < 0)
				surfBase.y = 0;
			if (surfBase.x + zoomW >(INT64)*pBufferW)
				surfBase.x = (INT64)*pBufferW - zoomW;
			if (surfBase.y + zoomH >(INT64)*pBufferH)
				surfBase.y = (INT64)*pBufferH - zoomH;
		}

		//#2���±�־������
		//bClip = (zoomW > MIN_FORCECLIP_WIDTH || zoomH > MIN_FORCECLIP_HEIGHT);
		SetClip(true);
		PostSurfPosChange();
		lastZoom = zoom;

		//#3����surface
		return SurfRenew(acce);
	}
	else
		return false;
}

inline bool Surfer::SurfRenew(bool acce)
{
	bool bRenewed = false;
	if (bHasBoth)
	{
		LARGE_INTEGER start, end;
		QueryPerformanceCounter(&start);

		POINT surfBias = { 0, 0 };	//ʵ��surface��㣨���Ͻǣ���Լ���surface��λ�ã�ƫ�����ڶ�λͼƬ����������surface���أ�
		bSurfClipped = false;
		if (bClip)
		{
			// ����surfBias		��bClip���surface����ڼ���surface��ƫ��
			// ����bSurfClipped	��ʵ��surface�Ƿ�clip
			// surface�����clip
			if (surfBase.x < 0)
			{
				surfBias.x = -surfBase.x;

				bSurfClipped = true;
			}
			// surface�ϲ���clip
			if (surfBase.y < 0)
			{
				surfBias.y = -surfBase.y;

				bSurfClipped = true;
			}
			// surface�Ҳ���clip
			if (surfBase.x + zoomW >(INT64)*pBufferW)
			{
				bSurfClipped = true;
			}
			// surface�²���clip
			if (surfBase.y + zoomH > (INT64)*pBufferH)
			{
				bSurfClipped = true;
			}
		}


		// ����surface
		if (!(bOutClient && bClip))//surfaceֻ�ڿͻ����Ⲣclipʱ�Ų�����
		{
			POINT destBase;//ʵ��surface�����Ŀ��buffer��λ�ã�����͸��ɫ�鶨λ��
			if (bClip)
			{
				destBase.x = surfBase.x < 0 ? 0 : surfBase.x;
				destBase.y = surfBase.y < 0 ? 0 : surfBase.y;
			}
			else
			{
				destBase.x = surfBase.x;
				destBase.y = surfBase.y;
			}

			// ���surface��ԭ���ߴ��б仯�������´���
			if (oldSurfSize.x != surfSize.x || oldSurfSize.y != surfSize.y || !surf)
			{
				// ���ԭ����
				SAFE_RELEASE(surf);
				// �½�surface
				if (FAILED(pDevice->CreateOffscreenPlainSurface(
					surfSize.x, surfSize.y
					, D3DFMT_A8R8G8B8
					, D3DPOOL_SYSTEMMEM
					, &surf
					, NULL)))
				{
					bSurfFailed = true;
					return false;//D3DPOOL_DEFAULT, D3DPOOL_SYSTEMMEM
				}
			}

			// ����surface����
			D3DLOCKED_RECT lockedRect;
			FAILED_RETURN_FALSE(surf->LockRect(&lockedRect, NULL, NULL));
			DWORD *surfData = (DWORD*)lockedRect.pBits;

			// ����
			if (acce)
			{
				if (zoom > NN_MIN || surfSize.x > MIN_NORMALRENEW_WIDTH || surfSize.y > MIN_NORMALRENEW_HEIGHT)
				{
					pBmp->Sample_NN(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_NN;
				}
				else
				{
					pBmp->Sample_Single(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_SINGLE;
				}
			}
			else
			{
				// ����ӳ����Ե�ѡ��
				if (zoom > NN_MIN)
				{
					pBmp->Sample_NN(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_NN;
				}
				else if (zoom >= 1)
				{
					//Сƽ��������
					pBmp->Sample_Single(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_SINGLE;
				}
				else if (zoom > 0.5f
					|| pBmp->width > MAXSAMPLE_WIDTH_UPPERLIMIT || pBmp->height > MAXSAMPLE_HEIGHT_UPPERLIMIT)
				{
					//˫����
					pBmp->Sample_BiLinear(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_BILINEAR;
				}
				else
				{
					//����ز���
					pBmp->Sample_Area(surfData, lockedRect.Pitch / 4, surfSize, zoom, 0, surfBias, destBase);
					sampleSchema = SAMPLE_SCHEMA_MAX;
				}
			}
			surf->UnlockRect();

			bRenewed = true;//���ø��±�־
			bSurfFailed = false;
			oldSurfSize = surfSize;

			// bRenewed������Ϣ��ȡ
			CalcMapInfo();//����surface������backbuffer����

			QueryPerformanceCounter(&end);
			renewTime = (float)(end.QuadPart - start.QuadPart) / freq.QuadPart;
		}
	}

	if (bRenewed)
		bNeedRenew = false;

	return bRenewed;
}

inline HRESULT Surfer::Render() const
{
	if (surf && !bOutClient && bHasDevice)
	{
		// ͼƬ������surface�봰�ڿͻ����н�������Ⱦ
		// ���backbuffer
		LPDIRECT3DSURFACE9 backbuffer = NULL;
		HRESULT hr = pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		if (FAILED(hr))
			return hr;
		// ������backbuffer
		hr = pDevice->UpdateSurface(surf, &rcSurf, backbuffer, &surfDest);//ͳһ�Ŀ���
		if (FAILED(hr))
			return hr;

		SAFE_RELEASE(backbuffer);

		return S_OK;
	}
	else
		return E_FAIL;
}

inline bool Surfer::OnMove_Custom(POINT cursorbias)
{
	if (bHasBoth)
	{
		bool oldPicClipped = bPicClipped;
		bool oldOutClient = bOutClient;

		SurfMovePR(cursorbias.x, cursorbias.y);

		bool bRenewed = false;
		if (bClip)
		{
			//�ƶ�ʱ�����bClip��־������ͼƬС��һ���ߴ磬��ȡ��clip��
			//�����ǰsurface��clip������������surface�������϶��ٶȡ�
			if (zoomW <= MAX_NOCLIP_WIDTH_DRAG && zoomH <= MAX_NOCLIP_HEIGHT_DRAG)
			{
				SetClip(false);
				if (bSurfClipped)
				{
					SurfRenew(false);//ȡ��clip��ֱ����������surface
					bRenewed = true;
				}
			}
			//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��bSurfClipped״̬����0��
			//����ʼ���ڿͻ����⣨ǰ��bOutClient״̬����1�����Ͳ�����
			else if ((oldPicClipped || bPicClipped) && (!oldOutClient || !bOutClient))
			{
				SurfRenew(true);
				bRenewed = true;
			}
		}
		//����û����surface�������surfaceӳ����Ϣ������surface����
		if (!bRenewed)
		{
			CalcMapInfo();//����surface������backbuffer����
		}

		return bRenewed;
	}
	else
		return false;
}

inline bool Surfer::OnWinsize_Custom()
{
	if (bHasBoth)
	{
		bool oldPicClipped = bPicClipped;
		bool oldOutClient = bOutClient;

		PostSurfPosChange();

		bool bRenewed = false;
		if (bClip)
		{
			//�ƶ�ʱ�����bClip��־������ͼƬС��һ���ߴ磬��ȡ��clip��
			//�����ǰsurface��clip������������surface�������϶��ٶȡ�
			if (zoomW <= MAX_NOCLIP_WIDTH_DRAG && zoomH <= MAX_NOCLIP_HEIGHT_DRAG)
			{
				SetClip(false);
				if (bSurfClipped)
				{
					SurfRenew(false);
					bRenewed = true;
				}
			}
			//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��bSurfClipped״̬����0��
			//����ʼ���ڿͻ����⣨ǰ��bOutClient״̬����1�������߲�����bClip��־���Ͳ�����
			else if ((oldPicClipped || bPicClipped) && (!oldOutClient || !bOutClient))
			{
				SurfRenew(true);
				bRenewed = true;
			}
		}
		//����û����surface�������surfaceӳ����Ϣ
		if (!bRenewed)
		{
			CalcMapInfo();//����surface������backbuffer����
		}

		return bRenewed;
	}
	else
		return false;
}