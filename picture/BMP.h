#pragma once 

#include<new>
#include<iostream>
#include<stdlib.h>
#include<winnt.h>

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
#define ZOOM_MAX						200
#define ZOOM_MIN						0.0f
#define ZOOM_MINDELTA					0.0001f

#define ZOOMFACTOR_DRAG					0.015f //����������
#define ZOOMFACTOR_WHEEL				0.025f
#define ZOOMFACTOR_ZOOMOUTSHRINK		0.7f

#define ZOOM_MAXCONTROLED				0.5f

// ��������
#define SAMPLE_SCHEMA_UNKNOWN			0
#define SAMPLE_SCHEMA_NN				1
#define SAMPLE_SCHEMA_BILINEAR			2
#define SAMPLE_SCHEMA_CUBE				3
#define SAMPLE_SCHEMA_SINGLE			4
#define SAMPLE_SCHEMA_MAX				5

#define NN_MIN							3.0f	//��������Ŵ���ʹ��NN����
#define MAXSAMPLE_WIDTH_UPPERLIMIT		20000	//��������ߴ�ͼƬĬ�ϲ�����������MAXSAMPLE
#define MAXSAMPLE_HEIGHT_UPPERLIMIT		MAXSAMPLE_WIDTH_UPPERLIMIT

#define MIN_NORMALRENEW_WIDTH			500		//ͼƬС������ߴ磬�ڼ���bRenewedʱ������ʹ��SingleSample(̫�����Ź��̿����п���)
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
	pResult[1] = (maxvalue == 0 ? 0 : ROUND_BYTE(255.0f*(maxvalue - minvalue) / maxvalue));//s

	if (maxvalue == minvalue)
		pResult[2] = 0;
	else
	{
		if (max == 2)
		{
			pResult[2] = ROUND_BYTE(255.0f * (000.0f + 60.0f * (g - b) / (maxvalue - minvalue)) / 360.0f);
		}
		else if (max == 1)
		{
			pResult[2] = ROUND_BYTE(255.0f * (120.0f + 60.0f * (b - r) / (maxvalue - minvalue)) / 360.0f);
		}
		else
		{
			pResult[2] = ROUND_BYTE(255.0f * (240.0f + 60.0f * (r - g) / (maxvalue - minvalue)) / 360.0f);
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
		pResult[0] = ROUND_BYTE(maxvalue);
		pResult[1] = ROUND_BYTE(maxvalue);
		pResult[2] = ROUND_BYTE(maxvalue);
	}
	else
	{
		float �� = color.g * color.b;
		float minvalue = maxvalue - ��;
		byte nh = ROUND_BYTE(color.r);
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

		pResult[0] = ROUND_BYTE(b + minvalue);
		pResult[1] = ROUND_BYTE(g + minvalue);
		pResult[2] = ROUND_BYTE(r + minvalue);

	}
	pResult[3] = ROUND_BYTE(color.a);

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

		pResult[0] = ROUND_BYTE(b + minvalue);
		pResult[1] = ROUND_BYTE(g + minvalue);
		pResult[2] = ROUND_BYTE(r + minvalue);

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

	˫���Բ�ֵ�����ֲ��ԣ���ɫ���ɫ���Copy_Single����ϸ����
	****************************************************/
	// �������ʧ��������СЧ���Ϻã���С�ٶ�������
	inline bool MYCALL1 Copy_MaxSample(DWORD *surfData, int pitch, const POINT &surfSize
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
	inline bool MYCALL1 Copy_Single(DWORD *surfData, int pitch, const POINT &surfSize
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
	inline bool MYCALL1 Copy_NN(DWORD *surfData, int pitch, const POINT &surfSize
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
	inline bool MYCALL1 Copy_BiLinear(DWORD *surfData, int pitch, const POINT &surfSize
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
	D3DXIMAGE_INFO D3DPicInfo;		//�Զ�ͼƬ��Ϣ
	WCHAR strFileName[MAX_PATH];	//�ļ���

	bool bSaved;
	bool bHasDirectory;

	// ��surface�������Ϣ
	POINT src;
	float zoom;

	// ��Ϣ
	WCHAR picInfoStr[MAX_PATH];
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

/*********************************************
*********************************************/
struct Surfer {
private:
	LPDIRECT3DSURFACE9 surf;				//surface
	LPDIRECT3DDEVICE9 pDevice;				//�豸ָ�롤����
	BMP *pBmp;								//BMPָ�롤����
	int *pBmpW, *pBmpH;						//BMP�ߴ硤����
	UINT *pBufferW, *pBufferH;				//Ŀ��surface�ߴ硤����
	bool bHasPic, bHasDevice, bHasBoth;		//�Ƿ�߱�����ֵ���������

public:
	// ��Ϣ
	//int surfrenewcount;					//surface���¼���
	float renewTime;						//surface���º�ʱ
	UINT8 sampleSchema;						//��������
	WCHAR surferInfoStr[512];				//��Ϣ�ַ���

	// ��ʱ
	LARGE_INTEGER wheeltick, lastwheeltick;	//���ּ�ʱ
	LARGE_INTEGER freq;						//Ƶ��

	/******************************************************************************
	surfBase�����ɱ�������BMP��ʾ��Ŀ��SURFACE����������"ѡ��"��ͬ���á��߼��Ͽɷ��룬��Ϊ���ڵ�BMP��Ϣ��
	���������ڲ�ά��
	surfBase��pBmp����bClip,zoom��Ǩ�ƹ�������SurfRenew֮ǰ�������:��zoomW��zoomH��lastSurfZoom��&��Ϣ
	*******************************************************************************/

	// "BMP" ӳ�� "SURFACE" ѡ��
	bool bClip;								//surface�Ƿ�bClip����ȥ�ͻ���������
	DWORD BackgroundColor;					//����ɫ����ʱû�ã�
	// "BMP" ӳ�� "SURFACE" ����
	float surfZoom;							//ֱ�ӲٿصķŴ���
	float lastSurfZoom;						//��һ���Ŵ���
	int zoomW, zoomH;						//�Ŵ�surfZoom�������surface�ĳߴ磬��������ʱ����������Ҫ�ֶ�ͬ�����¡�(��Ҫ�ֶ����³��ϣ����ţ�����ͼƬ)

	// "BMP" ӳ�� "SURFACE" ״̬
	float actualZoomX, actualZoomY;			//ʵ��x,y�����ϵķŴ����������־��ʱֻ�����鿴��
	bool bSurfFailed;						//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp����¡������־��ʱֻ�����鿴����Ҫ�ֶ�ͬ�����¡�
	bool bSurfClipped;						//��ǰsurface�Ƿ�bClip

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ����
	POINT surfBase;							//�Ŵ�surfZoom���ļ���surface�����Ͻǵ����ڿͻ���������
	POINT basePoint;						//����dragzoom�����ĵ�
	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ֱ�Ӳ���
	POINT surfDest;							//surface������backbuffer, backbuffer����ʼ��
	RECT rcSurf;							//surfaceҪ������backbuffer������

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ״̬
	POINT oldSurfSize;						//�洢���һ��surfRenewʱ�ĳߴ�
	POINT surfSize;							//surface�ߴ�
	bool bPicClipped;						//��ǰͼƬ�Ƿ�bClip
	bool bOutClient;						//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩
// ��Ϣ
	POINT cursorPixel;						//��굱ǰλ�ö�Ӧԭͼ������
	bool bCursorOnPic;						//����Ƿ���ͼƬ��
	DWORD cursorColor;						//��굱ǰλ�ö�Ӧԭͼ����ɫ
	UINT8 cursorPos;						//��ǰ���λ�ã����࣬�����ĸ�����ͼƬ���հס������⣩

	void CalcBindState();					// ���������־�������״̬��
	//void Refresh();							//��bPicOn�������������ͼƬ�ؼ���Ϣ(zoomW,zoomH)��ԭͼƬ�ߴ�仯ʱ��Ҫ���¡�

private:
	// ��bHasDevice������ӳ�䵽Ŀ��surface����
	inline void SurfCalcMapInfo()
	{
		if (bHasDevice)
		{
			//����rcSurf��surfDest
			//surfDest��surface������backbuffer��㣬����surfaceʱ�����������Ҫʵʱ����
			if (surfBase.x > 0)
				surfDest.x = 0;
			else
				surfDest.x = -surfBase.x;
			if (surfBase.y > 0)
				surfDest.y = 0;
			else
				surfDest.y = -surfBase.y;

			if (bClip)
			{
				rcSurf.left = 0;
				rcSurf.top = 0;
				rcSurf.right = min((int)*pBufferW - surfDest.x, surfSize.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
				rcSurf.bottom = min((int)*pBufferH - surfDest.y, surfSize.y);
			}
			else
			{
				rcSurf.left = max(surfBase.x, 0);//�����ʼ��surfBaseĳһάС��0����ƽ������0��㣬������destpointΪ��0
				rcSurf.top = max(surfBase.y, 0);
				rcSurf.right = min(zoomW, rcSurf.left + (int)*pBufferW - surfDest.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
				rcSurf.bottom = min(zoomH, rcSurf.top + (int)*pBufferH - surfDest.y);
			}
		}
	}

public:
	Surfer();
	~Surfer();

	inline bool IsSurfNull() const
	{
		return surf == NULL;
	}
	inline bool IsSurfNotNull() const
	{
		return surf != NULL;
	}
	inline POINT GetBase() const
	{
		return surfBase;
	}
	inline float GetZoom() const
	{
		return surfZoom;
	}
	inline int GetZoomWidth() const
	{
		return zoomW;
	}
	inline int GetZoomHeight() const
	{
		return zoomH;
	}
	inline void SetClip(bool clip)
	{
		if (bClip != clip)
		{
			bClip = clip;

			SETPOINT(surfSize, (LONG)zoomW, (LONG)zoomH);
			if (bClip)
			{
				// surface���clip
				if (surfBase.x > 0)
					surfSize.x -= surfBase.x;
				// surface�ϲ�clip
				if (surfBase.y > 0)
					surfSize.y -= surfBase.y;
				// surface�Ҳ�clip
				if ((LONG)zoomW - surfBase.x > (int)*pBufferW)
					surfSize.x -= zoomW - surfBase.x - (int)*pBufferW;
				// surface�²�clip
				if ((LONG)zoomH - surfBase.y > (int)*pBufferH)
					surfSize.y -= zoomH - surfBase.y - (int)*pBufferH;
			}
		}
	}

// �豸
	bool BindDevice(LPDIRECT3DDEVICE9 device);//�����豸
	bool BindBuf(UINT * pBufW, UINT *pBufH);//����Ŀ��surface�ߴ�
	void SetBackcolor(DWORD color);			//���ñ�����ɫ

// ͼƬ
	bool BindPic(PicPack *ppic, bool renew = true);//����ͼƬ����������NULL������������/�л�ͼƬ��
	bool DeBindPic(PicPack *ppic);			//���ͼƬ
	void Clear();							// ���,�ͷ���ͼƬ������
	
// surfaceˢ��ǰ��������λ���Ų����Ĳ���׼��
	void SetBasePoint(POINT color);
	void PostSurfPosChange();
	inline void SurfHomingPR()
	{
		surfBase.x = 0;
		surfBase.y = 0;

		PostSurfPosChange();
	}											//surface��ԭ��
	inline void SurfLocatePR(LONG x, LONG y)	//surface��λ
	{
		surfBase.x = x;
		surfBase.y = y;

		PostSurfPosChange();
	}
	//surface���У����Ŀ��surface��
	inline void SurfCenterPR(int bufferW, int bufferH)
	{
		surfBase.x = (LONG)(zoomW / 2 - bufferW / 2);
		surfBase.y = (LONG)(zoomH / 2 - bufferH / 2);

		PostSurfPosChange();
	}
	//surface�ƶ�������surface���£��ⲿ��������
	inline void SurfMovePR(int dx, int dy)
	{
		surfBase.x -= dx;
		surfBase.y -= dy;

		PostSurfPosChange();
	}
	//����������һ������������ű��ʵ�ֵ��ʵ��������Ҫ��һ������������һ��������ʹ�õ�����ı��ʣ�
	inline void SurfSetZoomPR(float zoom)// ����surface���ű���
	{
		surfZoom = zoom;
	}
	inline void SurfAdjustZoom_DragPR(int wParam)
	{
		float adds = 0;
		QueryPerformanceCounter(&wheeltick);

		//�仯�������ʱ��͵�ǰ�Ŵ��ʣ�����ȡ��ʱ�������
		adds = ZOOMFACTOR_DRAG*surfZoom;
		lastwheeltick.QuadPart = wheeltick.QuadPart;

		//�϶�����
		adds *= wParam;		

		//�Ŵ���С�ٶ�ƽ��
		if (adds < 0)
			adds *= ZOOMFACTOR_ZOOMOUTSHRINK;

		//��С�仯ֵ
		if (wParam > 0 && adds < ZOOM_MINDELTA)
			adds = ZOOM_MINDELTA;
		else if (wParam < 0 && adds > -ZOOM_MINDELTA)
			adds = -ZOOM_MINDELTA;

		surfZoom += adds;
		//zoom������
		if (surfZoom > ZOOM_MAX)
			surfZoom = ZOOM_MAX;
		if (surfZoom < ZOOM_MIN)
			surfZoom = ZOOM_MIN;
	}
	inline void SurfAdjustZoom_WheelPR(int wParam)
	{
		float adds = 0;
		QueryPerformanceCounter(&wheeltick);

		//�仯�������ʱ��͵�ǰ�Ŵ���
		adds = (sqrtf(freq.QuadPart / (float)(wheeltick.QuadPart - lastwheeltick.QuadPart))
			*(ZOOMFACTOR_WHEEL*surfZoom));
		lastwheeltick.QuadPart = wheeltick.QuadPart;

		//���ַ��ȱ��ʣ�������ʾ���ַ���
		adds *= wParam / 120.0f;

		//�Ŵ���С�ٶ�ƽ��
		if (adds < 0)
			adds *= ZOOMFACTOR_ZOOMOUTSHRINK;

		//��С�仯ֵ
		if (wParam > 0 && adds < ZOOM_MINDELTA)
			adds = ZOOM_MINDELTA;
		else if (wParam < 0 && adds > -ZOOM_MINDELTA)
			adds = -ZOOM_MINDELTA;

		surfZoom += adds;
		//zoom������
		if (surfZoom > ZOOM_MAX)
			surfZoom = ZOOM_MAX;
		if (surfZoom < ZOOM_MIN)
			surfZoom = ZOOM_MIN;
		/*if (surfZoom*min(*pBmpW, *pBmpH) < 1)
		surfZoom = 1.00001f / min(*pBmpW, *pBmpH);//����surface�ߴ�>0*/
	}

//surface���ţ�ǿ������ˢ�²�����
	// ��bHasBoth�����ݵ���ֵ���ţ����趨λ����������ӳ�����������surface�Ƿ�ˢ��
	inline bool SurfZoomRenew(float zoom = -1, int dx = 0, int dy = 0)
	{
		if (bHasBoth)
		{
			//zoom�׶�
			if (lastSurfZoom == zoom && dx == 0 && dy == 0)
				return false;

			surfBase.x += dx;
			surfBase.y += dy;

			if (zoom >= 0)
				surfZoom = zoom;

			zoomW = (int)(surfZoom**pBmpW);
			zoomH = (int)(surfZoom**pBmpH);
			actualZoomX = (float)zoomW / *pBmpW;
			actualZoomY = (float)zoomH / *pBmpH;

			//���±�־
			SetClip(true);
			PostSurfPosChange();

			lastSurfZoom = surfZoom;

			return SurfRenew(false);
		}
		else
			return false;
	}
	// ��bHasBoth���������趨�����ű���ֵ���ţ�����ӳ�����������surface�Ƿ�ˢ��
	inline bool SurfZoomRenew(POINT *client2cursor, bool stable = false, bool acce = false)
	{
		if (bHasBoth)
		{
			if (lastSurfZoom == surfZoom)
				return false;

			if (!client2cursor)//�������û�ṩ��׼�㣬����basePoint��Ϊ��׼��
				client2cursor = &basePoint;

			int oldzoomw = zoomW, oldzoomh = zoomH;
			zoomW = (int)(surfZoom**pBmpW);
			zoomH = (int)(surfZoom**pBmpH);
			actualZoomX = (float)zoomW / *pBmpW;
			actualZoomY = (float)zoomH / *pBmpH;

			UINT oldswidth = (UINT)(*pBmpW*lastSurfZoom)
				, oldsheight = (UINT)(*pBmpH*lastSurfZoom);

			//������ʼ��#1
			if (stable)
			{
				surfBase.x = (LONG)(zoomW / 2) - oldzoomw / 2 + surfBase.x;
				surfBase.y = (LONG)(zoomH / 2) - oldzoomh / 2 + surfBase.y;
			}
			else
			{
				POINT surf2cursor;
				surf2cursor.x = surfBase.x + client2cursor->x;
				surf2cursor.y = surfBase.y + client2cursor->y;

				float tempf = (float)surf2cursor.x*surfZoom / lastSurfZoom;//ƽ����ʽ
				if (tempf > 0)
					surfBase.x = (LONG)(tempf + 0.5f) - client2cursor->x;
				else
					surfBase.x = (LONG)(tempf - 0.5f) - client2cursor->x;
				tempf = (float)surf2cursor.y*surfZoom / lastSurfZoom;//ƽ����ʽ
				if (tempf > 0)
					surfBase.y = (LONG)(tempf + 0.5f) - client2cursor->y;
				else
					surfBase.y = (LONG)(tempf - 0.5f) - client2cursor->y;
			}

			//����ڷŴ�׶Σ����ڿͻ���������ȫ����surface�������surface��ʾ����
			//��ֹСͼƬ�Ŵ�Ѹ��ƫ�봰�ڿͻ���
			if (zoomW <= (int)*pBufferW
				&& zoomH <= (int)*pBufferH
				&& surfZoom > lastSurfZoom
				&& surfZoom < ZOOM_MAXCONTROLED)
			{
				if (surfBase.x > 0)
					surfBase.x = 0;
				if (surfBase.y > 0)
					surfBase.y = 0;
				if (surfBase.x < (LONG)zoomW - (LONG)*pBufferW)
					surfBase.x = (LONG)zoomW - (LONG)*pBufferW;
				if (surfBase.y < (LONG)zoomH - (LONG)*pBufferH)
					surfBase.y = (LONG)zoomH - (LONG)*pBufferH;
			}

			//����bClip��־
			//bClip = (zoomW > MIN_FORCECLIP_WIDTH || zoomH > MIN_FORCECLIP_HEIGHT);
			SetClip(true);
			PostSurfPosChange();

			lastSurfZoom = surfZoom;

			return SurfRenew(acce);
		}
		else
			return false;
	}

//surfaceˢ��
	// ��bHasBoth��surfaceˢ�£�����surface�Ƿ�ˢ��
	inline bool SurfRenew(bool acce = false)
	{
		bool bRenewed = false;
		if (bHasBoth)
		{
			LARGE_INTEGER start, end;
			QueryPerformanceCounter(&start);

			//POINT oldSurfSize = surfSize;
			//SETPOINT(surfSize, (LONG)zoomW, (LONG)zoomH);
			POINT surfBias = { 0, 0 };	//ʵ��surface��㣨���Ͻǣ���Լ���surface��λ�ã�ƫ�����ڶ�λͼƬ����������surface���أ�
			bSurfClipped = false;
			if (bClip)
			{
				// ����surfSize		��bClip���surface�ߴ�,
				// ����surfBias		��bClip���surface����ڼ���surface��ƫ��
				// ����bSurfClipped	��ʵ��surface�Ƿ�clip
				// surface�����clip
				if (surfBase.x > 0)
				{
					//surfSize.x -= surfBase.x;
					surfBias.x = surfBase.x;

					bSurfClipped = true;
				}
				// surface�ϲ���clip
				if (surfBase.y > 0)
				{
					//surfSize.y -= surfBase.y;
					surfBias.y = surfBase.y;

					bSurfClipped = true;
				}
				// surface�Ҳ���clip
				if ((LONG)zoomW - surfBase.x > (int)*pBufferW)
				{
					//surfSize.x -= zoomW - surfBase.x - (int)*pBufferW;

					bSurfClipped = true;
				}
				// surface�²���clip
				if ((LONG)zoomH - surfBase.y > (int)*pBufferH)
				{
					//surfSize.y -= zoomH - surfBase.y - (int)*pBufferH;

					bSurfClipped = true;
				}
			}


			// ����surface
			if (!(bOutClient && bClip))//surfaceֻ�ڿͻ����Ⲣclipʱ�Ų�����
			{
				// ����surfDest
				// ����destBase
				surfDest.x = surfBase.x > 0 ? 0 : -surfBase.x;
				surfDest.y = surfBase.y > 0 ? 0 : -surfBase.y;

				POINT destBase;//ʵ��surface�����Ŀ��buffer��λ�ã�����͸��ɫ�鶨λ��
				if (bClip)
				{
					destBase.x = surfDest.x;
					destBase.y = surfDest.y;
				}
				else
				{
					destBase.x = -surfBase.x;
					destBase.y = -surfBase.y;
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
					if (surfZoom > NN_MIN)
					{
						pBmp->Copy_NN(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_NN;
					}
					else if (surfSize.x > MIN_NORMALRENEW_WIDTH || surfSize.y > MIN_NORMALRENEW_HEIGHT)
					{
						pBmp->Copy_Single(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_SINGLE;
					}
					else
					{
						pBmp->Copy_Single(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_SINGLE;
					}
				}
				else
				{
					// ����ӳ����Ե�ѡ��
					if (surfZoom > NN_MIN)
					{
						pBmp->Copy_NN(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_NN;
					}
					else if (surfZoom >= 1)
					{
						//Сƽ��������
						pBmp->Copy_Single(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_SINGLE;
					}
					else if (surfZoom > 0.5f
						|| pBmp->width > MAXSAMPLE_WIDTH_UPPERLIMIT || pBmp->height > MAXSAMPLE_HEIGHT_UPPERLIMIT)
					{
						//˫����
						pBmp->Copy_BiLinear(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_BILINEAR;
					}
					else
					{
						//����ز���
						pBmp->Copy_MaxSample(surfData, lockedRect.Pitch / 4, surfSize, surfZoom, 0, surfBias, destBase);
						sampleSchema = SAMPLE_SCHEMA_MAX;
					}
				}
				surf->UnlockRect();

				bRenewed = true;//���ø��±�־
				oldSurfSize = surfSize;

				bSurfFailed = false;

				// bRenewed������Ϣ��ȡ
				SurfCalcMapInfo();//����surface������backbuffer����

				QueryPerformanceCounter(&end);
				renewTime = (float)(end.QuadPart - start.QuadPart) / freq.QuadPart;
			}
		}
		
		return bRenewed;
	}

//surface���Ʋ���
	// ��bHasBoth���Զ�����ק�¼�����(����bClip��surfaceλ�õ�״̬�Զ�����surface�Ƿ����)������surface�Ƿ�ˢ��
	inline bool OnMove_Custom(POINT cursorbias)
	{
		if (bHasBoth)
		{
			bool oldPicClipped = bPicClipped;
			bool oldOutClient = bOutClient;

			SurfMovePR(cursorbias.x, cursorbias.y);

			bool bRenewed = false;
			if (bClip)
			{
				//�ƶ�ʱ�����bClip��־������ͼƬС��һ���ߴ磬��ȡ��bClip��
				//�����ǰsurface��clip������������surface�������϶��ٶȡ�
				if (zoomW <= MAX_NOCLIP_WIDTH_DRAG && zoomH <= MAX_NOCLIP_HEIGHT_DRAG)
				{
					SetClip(false);
					if (bSurfClipped)
					{
						SurfRenew(true);
						bRenewed = true;
					}
				}
				//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��bSurfClipped״̬����0��
				//����ʼ���ڿͻ����⣨ǰ��bOutClient״̬����1�����Ͳ�����
				else if ((oldPicClipped || bPicClipped)	&& (!oldOutClient || !bOutClient))
				{
					SurfRenew(true);
					bRenewed = true;
				}
			}
			//����û����surface�������surfaceӳ����Ϣ������surface����
			if (!bRenewed)
			{
				SurfCalcMapInfo();//����surface������backbuffer����
			}

			return bRenewed;
		}
		else
			return false;
	}
	// ��bHasBoth���Զ��崰�������¼�����(����bClip��surfaceλ�õ�״̬�Զ�����surface�Ƿ����)������surface�Ƿ�ˢ��
	inline bool OnWinsize_Custom()//��OnMove_Custom���
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
						SurfRenew(true);
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
				SurfCalcMapInfo();//����surface������backbuffer����
			}

			return bRenewed;
		}
		else
			return false;
	}
	// ��bHasBoth��
	inline bool SurfSuit(int w, int h)
	{
		if (bHasBoth)
		{
			if (w < 1 || h < 1)
				return false;

			return SurfZoomRenew(min((float)w / *pBmpW, (float)h / *pBmpH), -surfBase.x, -surfBase.y);
		}
		else
			return false;
	}

// ��surf����Ⱦ
	inline HRESULT Render() const
	{
		if (surf && !bOutClient)
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

// ��Ϣ
	//�����Ϣ�ַ���
	inline const WCHAR *GetInfoStr() const
	{
		return surferInfoStr;
	}
	//��¼��ǰ״̬��Ϣ�������ַ�����
	inline void SetInfoStr()
	{
		WCHAR subinfo[256] = { 0 };
		surferInfoStr[0] = L'\0';

		// buffer�ߴ硢surface�ߴ�
		// ���ű���
		// surface��ʼ��
		swprintf_s(subinfo, L"SURFACE: %d �� %d     TIME: %.3fms (%S)\n\
		ZOOM: %.4f\n\
		BASE: %d, %d\n"
			, surfSize.x, surfSize.y, renewTime*1000.0f, GetSampleSchemaStr()
			, surfZoom
			, -surfBase.x, -surfBase.y);
		wcscat_s(surferInfoStr, subinfo);

		// �������λ��
		if (pBmp)
		{
			if (pBmp->isNotEmpty())
				swprintf_s(subinfo, L"PIXEL: %d, %d\n", cursorPixel.x, cursorPixel.y);
			else
				swprintf_s(subinfo, L"PIXEL:-, -\n");
		}
		else
			swprintf_s(subinfo, L"PIXEL:-, -\n");
		wcscat_s(surferInfoStr, subinfo);

		// ���������ɫ����Ļ������ɫ������ɫ
		COLOR_F3 hsvc = RGB2HSV_F3(cursorColor);
		if (bCursorOnPic)
			swprintf_s(subinfo, L"COLOR: #%02X.%06X.ARGB\n\
			              %d, %d, %d, %d.ARGB\n\
			              %.1f, %.2f, %.0f.HSV\n"
				, (cursorColor >> 24), (cursorColor & 0xFFFFFF)
				, (cursorColor >> 24), (cursorColor >> 16) & 0xFF, (cursorColor >> 8) & 0xFF, cursorColor & 0xFF
				, hsvc.r, hsvc.g, hsvc.b);
		else
			swprintf_s(subinfo, L"COLOR: ??.??????.ARGB\n");
		wcscat_s(surferInfoStr, subinfo);

		// ������Ϣ
		//"intended surface: %d�� %d\n
		//	clipsurface base: %d, %d\n
		//	PZ: X %.4f Y %.4f\n"
		//	, zoomW, zoomH
		//	, surfDest.x, surfDest.y
		//	, actualZoomX, actualZoomY

	}
	//���µ�ǰ��Ϣ����ǰ���λ�ö�ӦͼƬ�����أ�����Ϣ�ַ���
	inline void GetCurInfo(POINT *cursor, RECT *rcClient)
	{
		if (bHasPic)
		{
			if (!cursor || !rcClient)
				return;

			// ��õ�ǰ���λ�ã����ࣩ
			POINT surf2cursor = { surfBase.x + cursor->x - rcClient->left
				,surfBase.y + cursor->y - rcClient->top };
			if (OUTSIDE(*cursor, *rcClient))
				cursorPos = CURSORPOS_OUTWINDOW;
			else if (!surf)
				cursorPos = CURSORPOS_BLANK;
			else if (OUTSIDE2(surf2cursor, zoomW - 1, zoomH - 1))
				cursorPos = CURSORPOS_BLANK;
			else
				cursorPos = CURSORPOS_PIC;

			// ��������
			//�������actualzoom����surface��һ��ƥ�䣬��Ϊsurface����realzoom���ɣ�δ����actualzoom
			cursorPixel.x = (LONG)((surf2cursor.x + 0.5f) / surfZoom);
			cursorPixel.y = (LONG)((surf2cursor.y + 0.5f) / surfZoom);

			// ��ȡ����������ɫ
			if (cursorPos != CURSORPOS_PIC)//�������س���ͼƬ��Χ����곬�����ڷ�Χ
			{
				cursorColor = 0;
				bCursorOnPic = false;
			}
			else
				bCursorOnPic = pBmp->GetPixel(cursorPixel.x, cursorPixel.y, &cursorColor);

			// ������Ϣ�ַ���
			SetInfoStr();
		}
	}
	//�����һ������surface�Ĳ����㷨
	inline char *GetSampleSchemaStr()
	{
		switch (sampleSchema)
		{
		case SAMPLE_SCHEMA_UNKNOWN:
			return "unknown";
			break;
		case SAMPLE_SCHEMA_NN:
			return "nn";
			break;
		case SAMPLE_SCHEMA_BILINEAR:
			return "bi linear";
			break;
		case SAMPLE_SCHEMA_CUBE:
			return "cube";
			break;
		case SAMPLE_SCHEMA_SINGLE:
			return "single";
			break;
		case SAMPLE_SCHEMA_MAX:
			return "max";
			break;
		default:
			return "unknown";
			break;
		}
	}
};