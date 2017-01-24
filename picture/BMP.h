#pragma once 

#include<new>
#include<iostream>
#include<stdlib.h>
#include<winnt.h>

#include "Pic.h"
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

#define RGB2GRAY(R, G, B)				((R)*38 + (G)*75 + (B)*15) >> 7

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

// ͼ������
#define MAX_ZOOM						200
#define MAX_CONTROLEDZOOM				0.5f
#define MIN_ZOOM						0.0f
#define MIN_DELTAZOOM					0.000001f

#define DELTAZOOMNORMAL					0.25f
#define DELTAZOOM1						0.002f
#define DELTAZOOM2						0.002f

#define ZOOMOUT_SHRINK					0.80f

// ��������
#define SAMPLE_SCHEMA_UNKNOWN			0
#define SAMPLE_SCHEMA_NN				1
#define SAMPLE_SCHEMA_BILINEAR			2
#define SAMPLE_SCHEMA_CUBE				3
#define SAMPLE_SCHEMA_SINGLE			4
#define SAMPLE_SCHEMA_MAX				5

#define NN_MIN							3.0f	//��������Ŵ���ʹ��NN����
#define MAXSAMPLE_WIDTH_UPPERLIMIT		20000	//��������ߴ�ͼ��Ĭ�ϲ�����������MAXSAMPLE
#define MAXSAMPLE_HEIGHT_UPPERLIMIT		MAXSAMPLE_WIDTH_UPPERLIMIT

#define MIN_NORMALRENEW_WIDTH			500		//ͼ��С������ߴ磬�ڼ���renewʱ������ʹ��SingleSample(̫�����Ź��̿����п���)
#define MIN_NORMALRENEW_HEIGHT			MIN_NORMALRENEW_WIDTH

#define MIN_FORCECLIP_WIDTH				1500	//ͼ���������ߴ磬������clip�����������ٶ�
#define MIN_FORCECLIP_HEIGHT			MIN_FORCECLIP_WIDTH

#define MAX_NOCLIP_WIDTH_DRAG			3000	//ͼ��С������ߴ磬�϶�ͼ��ʱȡ��clip�������϶��ٶ�
#define MAX_NOCLIP_HEIGHT_DRAG			MAX_NOCLIP_WIDTH_DRAG

// ͼƬ�б�
#define PICPACKLIST_SIZE_INIT			3
#define PICPACKLIST_SIZE_INCREMENT		2

// ����״̬
#define KEYSTATEMASK_DOWN				0x8000
#define ISKEYDOWN(KEY)					(GetAsyncKeyState(KEY) & KEYSTATEMASK_DOWN)

// ���λ��
#define CURSORPOS_PIC					0
#define CURSORPOS_BLANK					1
#define CURSORPOS_OUTWINDOW				2

// ��ɫ
#define BACKCOLOR_INIT					0xFF888888
#define SETALPHA(C, A)					((C & 0xFFFFFF) | (A << 24))

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
	inline DWORD AlphaBlend(DWORD blender) {
		byte alpha = (byte)a;
		DWORD res = 0;
		//res += (byte)round(a) << 24;
		res += (DWORD)((r*alpha + ((blender >> 16) & 0xFF)*(255 - alpha)) / 255) << 16;
		res += (DWORD)((g*alpha + ((blender >> 8) & 0xFF)*(255 - alpha)) / 255) << 8;
		res += (DWORD)((b*alpha + (blender & 0xFF)*(255 - alpha)) / 255);

		return res;
	}
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

	//ʾ��
	/*static inline DWORD AlphaBlend1(COLOR_F3 src) {
		byte alpha = roundf2b(src.a);
		DWORD res = 0;
		res += (DWORD)alphablendtable1[(byte)(src.r)][alpha] << 16;
		res += (DWORD)alphablendtable1[(byte)(src.g)][alpha] << 8;
		res += (DWORD)alphablendtable1[(byte)(src.b)][alpha];

		return res;
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
	bool alphaon;

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
	surfbias:surf���ԭͼƫ�ƣ�����ԭͼѡ����
	backbase:surf���client���ڣ����ڸ���͸���������ƣ�

	˫���Բ�ֵ�����ֲ��ԣ���ɫ���ɫ���Copy_Single����ϸ����
	****************************************************/
	// �������ʧ��������СЧ���Ϻã���С�ٶ�������
	inline bool MYCALL1 Copy_MaxSample(DWORD *surfData, int pitch, const POINT &surfsize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		UINT32 zoomw = (UINT32)(width*zoom);
		UINT32 zoomh = (UINT32)(height*zoom);

		COLORTYPE xpixspan = (COLORTYPE)width / zoomw;//������x������
		COLORTYPE ypixspan = (COLORTYPE)height / zoomh;//������y������
		COLORTYPE spanarea = xpixspan*ypixspan;//��������� x*y
		float baseweight = 1.0f / spanarea;
		COLORTYPE *xbase = new COLORTYPE[surfsize.x + 1];//������������ʼ�����飬x����
		COLORTYPE *ybase = new COLORTYPE[surfsize.y + 1];//������������ʼ�����飬y����
		bool *xincolor1 = new bool[surfsize.x + 1];

		// �������������ֵ����,�޶���Ҫ������
		float actualzx = (float)width / zoomw;
		//float actualzy = (float)height / zoomh;
		for (int i = 0; i < surfsize.x + 1; i++)
		{
			xbase[i] = (COLORTYPE)(i + surfbias.x)*actualzx;
			xincolor1[i] = ((backbase.x + i) & TRANSPARENTBACK_MASK) > 0;
		}
		for (int i = 0; i < surfsize.y + 1; i++)
			ybase[i] = (COLORTYPE)(i + surfbias.y)*height / zoomh;//height / zoomh

		int index;
		COLOR_F3 rcolor = 0;//��ɫ�ۼ���
		for (int i = 0; i < surfsize.y; i++)
		{
			int starty = (int)ybase[i];//������������ʼ�㣬y����
			int endy = ceilf2i(ybase[i + 1]);//���������Ͻ����㣬y����
			bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

			for (int j = 0; j < surfsize.x; j++)
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
	inline bool MYCALL1 Copy_Single(DWORD *surfData, int pitch, const POINT &surfsize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		const float auxz = 0.3f / zoom;//����
		float auxfront = 0.5f - auxz, auxrear = 0.5f + auxz;//����

		float *xsto = new float[surfsize.x];//����,��ֹ���ڶ���ѭ����x�ظ�����
		bool *xincolor1 = new bool[surfsize.x];
		for (int j = 0; j < surfsize.x; j++)//��ǰ����x����ı���
		{
			xsto[j] = (j + surfbias.x + 0.5f) / zoom - 0.5f;
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (alphaon)// ��alphaͨ��
		{
			for (int i = 0; i < surfsize.y; i++)
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

				for (int j = 0; j < surfsize.x; j++)
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
			for (int i = 0; i < surfsize.y; i++)
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

				for (int j = 0; j < surfsize.x; j++)
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
	inline bool MYCALL1 Copy_NN(DWORD *surfData, int pitch, const POINT &surfsize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		int *bxarray = new int[surfsize.x];
		bool *xincolor1 = new bool[surfsize.x];
		for (int j = 0; j < surfsize.x; j++)//����
		{
			bxarray[j] = (int)(min((j + surfbias.x + 0.5f) / zoom, width - 1));
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (alphaon)
		{
			for (int i = 0; i < surfsize.y; i++)
			{
				int by = (int)(min((i + surfbias.y + 0.5f) / zoom, height - 1));
				DWORD *databy = (DWORD*)data[by];

				bool yincolor1 = ((backbase.y + i) & TRANSPARENTBACK_MASK) > 0;

				for (int j = 0; j < surfsize.x; j++)
				{
					index = i*pitch + j;

					int bx = bxarray[j];
					DWORD bmppixel = databy[bx];//��ȡbmp��Ϣʱ����surfaceƫ��

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
			for (int i = 0; i < surfsize.y; i++)
			{
				int by = (int)(min((i + surfbias.y + 0.5f) / zoom, height - 1));
				DWORD *databy = (DWORD*)data[by];
				for (int j = 0; j < surfsize.x; j++)
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
	inline bool MYCALL1 Copy_BiLinear(DWORD *surfData, int pitch, const POINT &surfsize
		, float zoom, float rotate = 0, const POINT surfbias = { 0,0 }, const POINT backbase = { 0,0 }) 
	{
		int index;

		float *xsto = new float[surfsize.x];//����,��ֹ���ڶ���ѭ����x�ظ�����
		bool *xincolor1 = new bool[surfsize.x];
		for (int j = 0; j < surfsize.x; j++)//��ǰ����x����ı������ռ�����
		{
			xsto[j] = (j + surfbias.x + 0.5f) / zoom - 0.5f;
			xincolor1[j] = ((backbase.x + j) & TRANSPARENTBACK_MASK) > 0;
		}
		if (alphaon)
		{
			for (int i = 0; i < surfsize.y; i++)
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

				for (int j = 0; j < surfsize.x; j++)
				{
					index = i*pitch + j;

					// ��Ӧԭͼ����λ��
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
			for (int i = 0; i < surfsize.y; i++)
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

				for (int j = 0; j < surfsize.x; j++)
				{
					// ��Ӧԭͼ����λ��
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
	BMP bmp;
	PicInfo myimginfo;				//ͼƬ��Ϣ
	D3DXIMAGE_INFO d3dimginfo;		//�Զ�ͼƬ��Ϣ
	WCHAR strFileName[MAX_PATH];	//�ļ���

	// ��surface�������Ϣ
	POINT src;
	float zoom;

	// ��Ϣ
	WCHAR picinfostr[MAX_PATH];
public:
	PicPack();
	~PicPack();

	inline bool isEmpty()
	{
		return bmp.isEmpty();
	}
	inline bool isNotEmpty()
	{
		return bmp.isNotEmpty();
	}
	inline float GetZoom()
	{
		return zoom;
	}
	inline void SetZoom(float z)
	{
		zoom = z;
	}
	inline POINT GetSrc()
	{
		return src;
	}
	inline void SetSrc(POINT point)
	{
		src = point;
	}
	inline BMP* GetPBMP()
	{
		return &bmp;
	}
	WCHAR *GetFileName();

	HRESULT LoadFile(LPDIRECT3DDEVICE9 pdev, WCHAR[]);
	bool SaveFile(LPDIRECT3DDEVICE9 pdev, WCHAR[]);

	WCHAR *GetPicInfoStrW();
};

class PicPackList {
private:
	PicPack **plist;
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
	PicPack *SetPicPack();
	PicPack *GetLast();
	PicPack *GetNext();
	PicPack *Drop();
	bool Add(PicPack *newpp);
};

/*********************************************
*********************************************/
struct Surfer {
public:
	LPDIRECT3DSURFACE9 surf;				//surface
	LPDIRECT3DDEVICE9 pdev;					//�豸ָ�롤����
	BMP *pbmp;								//BMPָ�롤���١�����
	int *pbmpw, *pbmph;						//BMP�ߴ硤����
	UINT *pbufw, *pbufh;					//Ŀ��surface�ߴ硤����
	bool haspic, hasdevice, hasboth;

	//int surfrenewcount;					//surface���¼���
	float renewtime;						//surface���º�ʱ
	UINT8 sampleschema;						//��������
	WCHAR surferinfostr[512];				//��Ϣ�ַ���

	LARGE_INTEGER wheeltick, lastwheeltick;	//���ּ�ʱ
	LARGE_INTEGER freq;						//Ƶ��

	/******************************************************************************
	surfsrc�����ɱ�������BMP��ʾ��Ŀ��SURFACE����������"ѡ��"��ͬ���á��߼��Ͽɷ��룬��Ϊ���ڵ�BMP��Ϣ��
	���������ڲ�ά��
	surfsrc��bmp����clip,zoom��Ǩ�ƹ�������SurfRenew֮ǰ�������:��zoomw��zoomh��oldsurfzoom��&��Ϣ
	*******************************************************************************/

	// "BMP" ӳ�� "SURFACE" ѡ��
	bool clip;								//surface�Ƿ�clip����ȥ�ͻ���������
	D3DCOLOR backcolor;						//����ɫ����ʱû�ã�
	// "BMP" ӳ�� "SURFACE" ����
	float surfzoom;							//ֱ�ӲٿصķŴ���
	float oldsurfzoom;						//��һ���Ŵ���
	int zoomw, zoomh;						//�Ŵ�surfzoom�������surface�ĳߴ磬��������ʱ��������

	// "BMP" ӳ�� "SURFACE" ״̬
	float actualzoomx, actualzoomy;			//ʵ��x,y�����ϵķŴ����������־��ʱֻ�����鿴��
	bool surffailed;						//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp����¡������־��ʱֻ�����鿴��
	bool surfclipped;						//��ǰsurface�Ƿ�clip

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ����
	POINT surfsrc;							//�Ŵ�surfzoom���ļ���surface�����Ͻǵ����ڿͻ���������
	POINT basepoint;						//����dragzoom�����ĵ�
	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ֱ�Ӳ���
	POINT surfbase;							//surface������backbuffer, backbuffer����ʼ��
	RECT surfrect;							//surfaceҪ������backbuffer������

	// "ԴSURFACE" ӳ�� "Ŀ��SURFACE" ״̬
	POINT surfsize;							//surface�ߴ�
	bool picclipped;						//��ǰͼƬ�Ƿ�clip
	bool outsideclient;						//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩
// ��Ϣ
	POINT picpixel;							//��굱ǰλ�ö�Ӧԭͼ������
	bool pixelcolorvalid;
	DWORD picpixelcolor;					//��굱ǰλ�ö�Ӧԭͼ����ɫ
	UINT8 cursorpos;						//��ǰ���λ�ã����࣬�����ĸ�����ͼƬ���հס������⣩

	void CalcBindSituation();
public:
	Surfer();
	~Surfer();

	inline POINT GetSrc()
	{
		return surfsrc;
	}
	inline float GetZoom()
	{
		return surfzoom;
	}
// �豸
	bool BindDevice(LPDIRECT3DDEVICE9 device);//�����豸
	bool BindBuf(UINT * bufw, UINT *bufh);	//����Ŀ��surface�ߴ�
	void SetBackcolor(D3DCOLOR bc);			

// ͼƬ
	bool BindPic(PicPack *ppic, bool renew = true, bool debind = true);//����ͼƬ
	bool DeBindPic(PicPack *ppic);						//���ͼƬ
	void Refresh();//��haspic�������������ͼƬ�ؼ���Ϣ(zoomw,zoomh)��ͼ��ߴ�仯ʱ������¡�
	void Clear();							// ���,�ͷ���ͼƬ������
	
// surface��λ����
	void SetBasePoint(POINT bp);
	inline void SurfHoming()
	{
		surfsrc.x = 0;
		surfsrc.y = 0;
	}											//surface��ԭ��
	inline void SurfLocate(LONG x, LONG y)		//surface��λ
	{
		surfsrc.x = x;
		surfsrc.y = y;
	}
	inline void SurfCenter(int bufferw, int bufferh)
	{
		if (zoomw <= bufferw
			&& zoomh <= bufferh)
		{
			surfsrc.x = (LONG)(zoomw / 2 - bufferw / 2);
			surfsrc.y = (LONG)(zoomh / 2 - bufferh / 2);
		}
		else
		{
			surfsrc.x = 0;
			surfsrc.y = 0;
		}
	}					//surface���У����Ŀ��surface��
	inline void SurfMove(int dx, int dy)
	{
		surfsrc.x -= dx;
		surfsrc.y -= dy;
	}								//surface�ƶ�������surface���£��ⲿ��������
	inline void SurfSetZoom(float z)
	{
		surfzoom = z;
	}
	// ����surface���ű���	
	inline void SurfAdjustZoom_normal1(int wParam)
	{
		float adds = 0;
		QueryPerformanceCounter(&wheeltick);

		adds = (float)((DELTAZOOMNORMAL*surfzoom));//�����ٶ���ʱ��ͷŴ����й�
		lastwheeltick.QuadPart = wheeltick.QuadPart;

		adds *= wParam / 120.0f;
		if (adds < 0)
			adds *= ZOOMOUT_SHRINK;//�Ŵ���Сһ�»�

		surfzoom += adds;
		//zoom������
		if (surfzoom > MAX_ZOOM)
			surfzoom = MAX_ZOOM;
		if (surfzoom < 0)
			surfzoom = 0;
	}
	inline void SurfAdjustZoom_wheel(int wParam)
	{
		float adds = 0;
		QueryPerformanceCounter(&wheeltick);

		if (surfzoom < 1)
			adds = (float)(freq.QuadPart / (wheeltick.QuadPart - lastwheeltick.QuadPart)
				*(DELTAZOOM1*surfzoom)) + MIN_DELTAZOOM;//�����ٶ���ʱ��ͷŴ����й�
		else
			adds = (float)(freq.QuadPart / (wheeltick.QuadPart - lastwheeltick.QuadPart)
				*(DELTAZOOM2*surfzoom)) + MIN_DELTAZOOM;//�����ٶ���ʱ��ͷŴ����й�
		lastwheeltick.QuadPart = wheeltick.QuadPart;

		adds *= wParam / 120.0f;
		if (adds < 0)
			adds *= ZOOMOUT_SHRINK;//�Ŵ���Сһ�»�

		surfzoom += adds;
		//zoom������
		if (surfzoom > MAX_ZOOM)
			surfzoom = MAX_ZOOM;
		if (surfzoom < 0)
			surfzoom = 0;
		/*if (surfzoom*min(*pbmpw, *pbmph) < 1)
			surfzoom = 1.00001f / min(*pbmpw, *pbmph);//����surface�ߴ�>0*/
	}

	// ��hasboth�����ݵ���ֵ�������ű��ʣ�����ӳ�����
	inline void SurfZoomRenew(float zoom = -1, int dx = 0, int dy = 0)
	{
		if (hasboth)
		{
			if (oldsurfzoom == zoom && dx == 0 && dy == 0)
				return;

			surfsrc.x += dx;
			surfsrc.y += dy;

			if (zoom >= 0)
				surfzoom = zoom;
			zoomw = (int)(surfzoom**pbmpw);
			zoomh = (int)(surfzoom**pbmph);
			actualzoomx = (float)zoomw / *pbmpw;
			actualzoomy = (float)zoomh / *pbmph;

			//����clip��־
			clip = true;

			oldsurfzoom = surfzoom;

			SurfRenew(false);
		}
	}
	inline void SurfZoomRenew(POINT client2cursor, bool stable = false, bool acce = false)
	{
		if (hasboth)
		{
			if (oldsurfzoom == surfzoom)
				return;

			int oldzoomw = zoomw, oldzoomh = zoomh;
			zoomw = (int)(surfzoom**pbmpw);
			zoomh = (int)(surfzoom**pbmph);

			actualzoomx = (float)zoomw / *pbmpw;
			actualzoomy = (float)zoomh / *pbmph;

			UINT oldswidth = (UINT)(*pbmpw*oldsurfzoom)
				, oldsheight = (UINT)(*pbmph*oldsurfzoom);

			//������ʼ��#1
			if (ISKEYDOWN(VK_MENU) || stable)
			{
				surfsrc.x = (LONG)(zoomw / 2) - oldzoomw / 2 + surfsrc.x;
				surfsrc.y = (LONG)(zoomh / 2) - oldzoomh / 2 + surfsrc.y;
			}
			else
			{
				POINT surf2cursor;
				surf2cursor.x = surfsrc.x + client2cursor.x;
				surf2cursor.y = surfsrc.y + client2cursor.y;

				float tempf = (float)surf2cursor.x*surfzoom / oldsurfzoom;//ƽ����ʽ
				if (tempf > 0)
					surfsrc.x = (LONG)(tempf + 0.5f) - client2cursor.x;
				else
					surfsrc.x = (LONG)(tempf - 0.5f) - client2cursor.x;
				tempf = (float)surf2cursor.y*surfzoom / oldsurfzoom;//ƽ����ʽ
				if (tempf > 0)
					surfsrc.y = (LONG)(tempf + 0.5f) - client2cursor.y;
				else
					surfsrc.y = (LONG)(tempf - 0.5f) - client2cursor.y;
			}

			//����ڷŴ�׶Σ����ڿͻ���������ȫ����surface�������surface��ʾ����
			//��ֹСͼƬ�Ŵ�Ѹ��ƫ�봰�ڿͻ���
			if (zoomw <= (int)*pbufw
				&& zoomh <= (int)*pbufh
				&& surfzoom > oldsurfzoom
				&& surfzoom < MAX_CONTROLEDZOOM)
			{
				if (surfsrc.x > 0)
					surfsrc.x = 0;
				if (surfsrc.y > 0)
					surfsrc.y = 0;
				if (surfsrc.x < (LONG)zoomw - (LONG)*pbufw)
					surfsrc.x = (LONG)zoomw - (LONG)*pbufw;
				if (surfsrc.y < (LONG)zoomh - (LONG)*pbufh)
					surfsrc.y = (LONG)zoomh - (LONG)*pbufh;
			}

			//����clip��־
			//clip = (zoomw > MIN_FORCECLIP_WIDTH || zoomh > MIN_FORCECLIP_HEIGHT);
			clip = true;

			oldsurfzoom = surfzoom;

			SurfRenew(acce);
		}
	}

	// ��hasboth��surface����
	inline bool SurfRenew(bool acce = false)
	{
		if (hasboth)
		{
			LARGE_INTEGER start, end;
			QueryPerformanceCounter(&start);

			outsideclient = (
				surfsrc.x > zoomw || surfsrc.y > zoomh	//���򳬳�surface�һ���
				|| surfsrc.x + (int)*pbufw <= 0			//�������²���surface�����
				|| surfsrc.y + (int)*pbufh <= 0
				);

			POINT oldsurfsize = surfsize;
			surfsize.x = (LONG)zoomw;
			surfsize.y = (LONG)zoomh;
			POINT surfbias = { 0, 0 };	//ʵ��surface��㣨���Ͻǣ���Լ���surface��λ�ã�ƫ�����ڶ�λͼƬ����������surface���أ�
			surfclipped = false;
			if (clip)
			{
				// ����surfsize	 ��clip���surface�ߴ�,
				// ����surfbias	 ��clip���surface����ڼ���surface��ƫ��
				// ����surfclipped ��ʵ��surface�Ƿ�clip
				// surface�����clip
				if (surfsrc.x > 0)
				{
					surfsize.x -= surfsrc.x;
					surfbias.x = surfsrc.x;

					surfclipped = true;
				}
				// surface�ϲ���clip
				if (surfsrc.y > 0)
				{
					surfsize.y -= surfsrc.y;
					surfbias.y = surfsrc.y;

					surfclipped = true;
				}
				// surface�Ҳ���clip
				if ((LONG)zoomw - surfsrc.x > (int)*pbufw)
				{
					surfsize.x -= zoomw - surfsrc.x - (int)*pbufw;

					surfclipped = true;
				}
				// surface�²���clip
				if ((LONG)zoomh - surfsrc.y > (int)*pbufh)
				{
					surfsize.y -= zoomh - surfsrc.y - (int)*pbufh;

					surfclipped = true;
				}
			}

			if (surfsrc.x > 0)
				surfbase.x = 0;
			else
				surfbase.x = -surfsrc.x;
			if (surfsrc.y > 0)
				surfbase.y = 0;
			else
				surfbase.y = -surfsrc.y;

			// ����surface
			if (!(outsideclient && clip))
			{
				POINT backbase;
				if (!clip)
				{
					backbase.x = surfbase.x - max(surfsrc.x, 0);
					backbase.y = surfbase.y - max(surfsrc.y, 0);
				}
				else
				{
					backbase.x = surfbase.x;
					backbase.y = surfbase.y;
				}

				if (oldsurfsize.x != surfsize.x || oldsurfsize.y != surfsize.y || !surf)
				{
					// ���ԭ����
					SAFE_RELEASE(surf);
					// �½�surface
					if (FAILED(pdev->CreateOffscreenPlainSurface(
						surfsize.x, surfsize.y
						, D3DFMT_A8R8G8B8
						, D3DPOOL_SYSTEMMEM
						, &surf
						, NULL)))
						return false;//D3DPOOL_DEFAULT, D3DPOOL_SYSTEMMEM
				}

				// ����surface����
				D3DLOCKED_RECT lockedRect;
				surf->LockRect(&lockedRect, NULL, NULL);
				DWORD *surfData = (DWORD*)lockedRect.pBits;

				// ����
				if (acce)
				{
					if (surfzoom > NN_MIN)
					{
						pbmp->Copy_NN(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_NN;
					}
					else if (surfsize.x > MIN_NORMALRENEW_WIDTH || surfsize.y > MIN_NORMALRENEW_HEIGHT)
					{
						pbmp->Copy_NN(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_NN;
					}
					else
					{
						pbmp->Copy_Single(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_SINGLE;
					}
				}
				else
				{
					// ����ӳ����Ե�ѡ��
					if (surfzoom > NN_MIN)
					{
						pbmp->Copy_NN(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_NN;
					}
					else if (surfzoom >= 1)
					{
						//Сƽ��������
						pbmp->Copy_Single(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_SINGLE;
					}
					else if (surfzoom > 0.5f
						|| pbmp->width > MAXSAMPLE_WIDTH_UPPERLIMIT || pbmp->height > MAXSAMPLE_HEIGHT_UPPERLIMIT)
					{
						//˫����
						pbmp->Copy_BiLinear(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_BILINEAR;
					}
					else
					{
						//����ز���
						pbmp->Copy_MaxSample(surfData, lockedRect.Pitch / 4, surfsize, surfzoom, 0, surfbias, backbase);
						sampleschema = SAMPLE_SCHEMA_MAX;
					}
				}
				surf->UnlockRect();
			}

			if (surf)
			{
				surffailed = false;

				// renew������Ϣ��ȡ
				picclipped = (
					surfsrc.x > 0 || surfsrc.y > 0			//surface�����ϲ���clip
					|| zoomw - surfsrc.x > (int)*pbufw		//surface�Ҳ���clip
					|| zoomh - surfsrc.y > (int)*pbufh		//surface�²���clip
					);
				SurfCalcMapInfo();//����surface������backbuffer����

				QueryPerformanceCounter(&end);
				renewtime = (float)(end.QuadPart - start.QuadPart) / freq.QuadPart;
				return true;
			}
			else
			{
				surffailed = true;

				return false;
			}
		}
		else
			return false;
	}
	// ��hasdevice������ӳ�䵽Ŀ��surface����
	inline void SurfCalcMapInfo()
	{
		if (hasdevice)
		{
			//����surfrect��surfbase
			//surfbase��surface������backbuffer��㣬����surfaceʱ�����������Ҫʵʱ����
			if (surfsrc.x > 0)
				surfbase.x = 0;
			else
				surfbase.x = -surfsrc.x;
			if (surfsrc.y > 0)
				surfbase.y = 0;
			else
				surfbase.y = -surfsrc.y;

			if (clip)
			{
				surfrect.left = 0;
				surfrect.top = 0;
				surfrect.right = min((int)*pbufw - surfbase.x, surfsize.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
				surfrect.bottom = min((int)*pbufh - surfbase.y, surfsize.y);
			}
			else
			{
				surfrect.left = max(surfsrc.x, 0);//�����ʼ��surfsrcĳһάС��0����ƽ������0��㣬������destpointΪ��0
				surfrect.top = max(surfsrc.y, 0);
				surfrect.right = min(zoomw, surfrect.left + (int)*pbufw - surfbase.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
				surfrect.bottom = min(zoomh, surfrect.top + (int)*pbufh - surfbase.y);
			}
		}
	}
	// ��hasboth���Զ�����ק�¼�����
	inline bool OnDrag_Custom(POINT cursorbias)
	{
		if (hasboth)
		{
			SurfMove(cursorbias.x, cursorbias.y);

			bool lastpicclipped = picclipped;
			picclipped = (
				surfsrc.x > 0 || surfsrc.y > 0				//surface�����ϲ���clip
				|| zoomw - surfsrc.x > (int)*pbufw			//surface�Ҳ���clip
				|| zoomh - surfsrc.y > (int)*pbufh			//surface�²���clip
				);
			outsideclient = (
				surfsrc.x > zoomw || surfsrc.y > zoomh		//���򳬳�surface�һ���
				|| surfsrc.x + (int)*pbufw <= 0				//�������²���surface�����
				|| surfsrc.y + (int)*pbufh <= 0
				);
			bool renew = false;
			if (clip)
			{
				//�ƶ�ʱ�����clip��־������ͼƬС��һ���ߴ磬��ȡ��clip��
				//�����ǰsurface��clip������������surface�������϶��ٶȡ�
				if (zoomw <= MAX_NOCLIP_WIDTH_DRAG && zoomh <= MAX_NOCLIP_HEIGHT_DRAG)
				{
					clip = false;
					if (surfclipped)
					{
						SurfRenew(true);
						renew = true;
					}
				}
				//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��surfclipped״̬����0��
				//����ʼ���ڿͻ����⣨ǰ��outsideclient״̬����1�������߲�����clip��־���Ͳ�����
				else if ((lastpicclipped || picclipped)
					&& !outsideclient)
				{
					SurfRenew(true);
					renew = true;
				}
			}
			//����û����surface�������surfaceӳ����Ϣ������surface����
			if (!renew)
			{
				SurfCalcMapInfo();//����surface������backbuffer����
			}

			return renew;
		}
		else
			return false;
	}
	// ��hasboth���Զ��崰�������¼�����
	inline bool OnWinsize_Custom()//��ondrag���
	{
		if (hasboth)
		{
			bool lastpicclipped = picclipped;
			picclipped = (
				surfsrc.x > 0 || surfsrc.y > 0		//surface�����ϲ���clip
				|| zoomw - surfsrc.x > (int)*pbufw	//surface�Ҳ���clip
				|| zoomh - surfsrc.y > (int)*pbufh	//surface�²���clip
				);
			outsideclient = (
				surfsrc.x > zoomw || surfsrc.y > zoomh	//���򳬳�surface�һ���
				|| surfsrc.x + (int)*pbufw <= 0			//�������²���surface�����
				|| surfsrc.y + (int)*pbufh <= 0
				);
			bool renew = false;
			if (clip)
			{
				//�ƶ�ʱ�����clip��־������ͼƬС��һ���ߴ磬��ȡ��clip��
				//�����ǰsurface��clip������������surface�������϶��ٶȡ�
				if (zoomw <= MAX_NOCLIP_WIDTH_DRAG && zoomh <= MAX_NOCLIP_HEIGHT_DRAG)
				{
					clip = false;
					if (surfclipped)
					{
						SurfRenew(true);
						renew = true;
					}
				}
				//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��surfclipped״̬����0��
				//����ʼ���ڿͻ����⣨ǰ��outsideclient״̬����1�������߲�����clip��־���Ͳ�����
				else if ((lastpicclipped || picclipped)
					&& !outsideclient)
				{
					SurfRenew(true);
					renew = true;
				}
			}
			//����û����surface�������surfaceӳ����Ϣ
			if (!renew)
			{
				SurfCalcMapInfo();//����surface������backbuffer����
			}

			return renew;
		}
		else
			return false;
	}
	// ��hasboth��
	inline void SurfSuit(int w, int h)
	{
		if (hasboth)
		{
			if (w < 1 || h < 1)
				return;

			SurfZoomRenew(min((float)w / *pbmpw, (float)h / *pbmph), -surfsrc.x, -surfsrc.y);
		}
	}

// ��surf����Ⱦ
	inline HRESULT Render()
	{
		if (surf && !outsideclient)
		{
			// ͼƬ������surface�봰�ڿͻ����н�������Ⱦ
			// ���backbuffer
			LPDIRECT3DSURFACE9 backbuffer = NULL;
			HRESULT hr = pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
			if (FAILED(hr))
				return hr;
			// ������backbuffer
			hr = pdev->UpdateSurface(surf, &surfrect, backbuffer, &surfbase);//ͳһ�Ŀ���
			if (FAILED(hr))
				return hr;

			SAFE_RELEASE(backbuffer);

			return S_OK;
		}
		else
			return E_FAIL;
	}

// ��Ϣ
	inline WCHAR *GetInfoStr()
	{
		return surferinfostr;
	}
	inline void SetInfoStr()
	{
		WCHAR subinfo[256] = { 0 };
		surferinfostr[0] = L'\0';

		// buffer�ߴ硢surface�ߴ�
		swprintf_s(subinfo, _T("SURFACE: %d �� %d     TIME: %.3fms (%S)\n\
		ZOOM: %.4f\n")
			, surfsize.x, surfsize.y, renewtime*1000.0f, GetSampleSchemaStr()
			, surfzoom);
		wcscat_s(surferinfostr, subinfo);

		// surface��ʼ�㡢clipsurface��ʼ��
		swprintf_s(subinfo, _T("BASE: %d, %d\n")
			, -surfsrc.x, -surfsrc.y);
		wcscat_s(surferinfostr, subinfo);

		// �������λ��
		if (pbmp)
		{
			if (pbmp->isNotEmpty())
				swprintf_s(subinfo, _T("PIXEL: %d, %d\n"), picpixel.x, picpixel.y);
			else
				swprintf_s(subinfo, _T("PIXEL:-, -\n"));
		}
		else
			swprintf_s(subinfo, _T("PIXEL:-, -\n"));
		wcscat_s(surferinfostr, subinfo);

		// ���������ɫ����Ļ������ɫ������ɫ
		swprintf_s(subinfo, _T("COLOR: %02X.%06X.ARGB\n")
			, (picpixelcolor >> 24), (picpixelcolor & 0xFFFFFF));
		wcscat_s(surferinfostr, subinfo);

		// ������Ϣ
		//"intended surface: %d�� %d\n
		//	clipsurface base: %d, %d\n
		//	PZ: X %.4f Y %.4f\n"
		//	, zoomw, zoomh
		//	, surfbase.x, surfbase.y
		//	, actualzoomx, actualzoomy

	}
	inline void GetCurInfo(POINT cursor, RECT clientrect)
	{
		if (haspic)
		{
			// ��õ�ǰ���λ�ã����ࣩ
			POINT surf2cursor = { surfsrc.x + cursor.x - clientrect.left
				,surfsrc.y + cursor.y - clientrect.top };
			if (OUTSIDE(cursor, clientrect))
				cursorpos = CURSORPOS_OUTWINDOW;
			else if (!surf)
				cursorpos = CURSORPOS_BLANK;
			else if (OUTSIDE2(surf2cursor, zoomw - 1, zoomh - 1))
				cursorpos = CURSORPOS_BLANK;
			else
				cursorpos = CURSORPOS_PIC;

			// ��������
			//�������actualzoom����surface��һ��ƥ�䣬��Ϊsurface����realzoom���ɣ�δ����actualzoom
			picpixel.x = (LONG)((surf2cursor.x + 0.5f) / surfzoom);
			picpixel.y = (LONG)((surf2cursor.y + 0.5f) / surfzoom);

			// ��ȡ����������ɫ
			if (cursorpos != CURSORPOS_PIC)//�������س���ͼƬ��Χ����곬�����ڷ�Χ
			{
				picpixelcolor = 0;
				pixelcolorvalid = false;
			}
			else
				pixelcolorvalid = pbmp->GetPixel(picpixel.x, picpixel.y, &picpixelcolor);

			// ������Ϣ�ַ���
			SetInfoStr();
		}
	}//��ȡ��ǰ���λ�ö�ӦͼƬ������
	inline char *GetSampleSchemaStr()
	{
		switch (sampleschema)
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