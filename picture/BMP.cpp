#include "stdafx.h"
#include "BMP.h"

BMP::BMP()
{
	//set_new_handler(newhandlerfunc);

	data = NULL;
	alphaon = false;
}

bool BMP::Load(LPDIRECT3DSURFACE9 surf)
{
	if (!surf)
		return false;

	//surf����
	D3DSURFACE_DESC surfaceDesc;
	surf->GetDesc(&surfaceDesc);

	//���surf����
	int /*oldwidth = width,*/ oldheight = height;
	width = surfaceDesc.Width;
	height = surfaceDesc.Height;
	if (width <= 0 || height <= 0)
		return false;


	//���ԭͼ
	if (data)
	{
		for (int i = 0; i < oldheight; i++)
		{
			if (data[i])
			{
				delete[] data[i];
				data[i] = NULL;
			}
		}

		delete[] data;
		data = NULL;
	}

	//������BMPͼ
	try
	{
		data = new byte*[height];
		for (int i = 0; i < height; i++)
		{
			data[i] = new byte[4 * width];
		}
	}
	catch (bad_alloc &memExp)
	{
		//ʧ���Ժ�ҪôabortҪô�ط���
		cerr << memExp.what() << endl;
		data = NULL;

		return false;
	}

	//����surface����
	D3DLOCKED_RECT lockedRect;
	surf->LockRect(&lockedRect, NULL, NULL);
	DWORD *surfData = (DWORD*)lockedRect.pBits;

	//����
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i*lockedRect.Pitch / 4 + j;

			((DWORD*)data[i])[j] = surfData[index];
		}
	}
	surf->UnlockRect();

	return true;
}

/*
����λͼ����ƥ�����
*/
bool BMP::CreateSurf(LPDIRECT3DDEVICE9 device, LPDIRECT3DSURFACE9 *surf, D3DCOLOR color, int zoom)
{
	if (width <= 0 || height <= 0
		|| !data)
		return false;
	if (!device)
		return false;
	if (zoom <= 0)
		return false;

	HRESULT hr;

	//���ԭ����
	if (*surf)
	{
		(*surf)->Release();
		(*surf) = NULL;
	}
	//�½�ƥ�����
	hr = device->CreateOffscreenPlainSurface(
		width*zoom, height*zoom
		, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM
		, surf
		, NULL);

	if (FAILED(hr))
	{
		//MessageBoxW(NULL, L"CreateOffscreenPlainSurface in BMP FAILED!", L"", 0);
		return false;
	}

	//����surface����
	D3DLOCKED_RECT lockedRect;
	(*surf)->LockRect(&lockedRect, NULL, NULL);
	DWORD *surfData = (DWORD*)lockedRect.pBits;

	//����
	DWORD realcolor = 0;
	byte alpha = 0;
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i*lockedRect.Pitch / 4 * zoom + j*zoom;

			for (int m = 0; m < zoom; m++)
			{
				for (int n = 0; n < zoom; n++)
				{
					alpha = (byte)(((DWORD*)data[i])[j] >> 24);
					realcolor = 0;
					realcolor += ((((DWORD*)data[i])[j] & 0xFF)*alpha + (color & 0xFF)*(255 - alpha)) / 255;
					realcolor += (((((DWORD*)data[i])[j] >> 8 & 0xFF)*alpha + (color >> 8 & 0xFF)*(255 - alpha)) / 255)<<8;
					realcolor += (((((DWORD*)data[i])[j] >> 16 & 0xFF)*alpha + (color >> 16 & 0xFF)*(255 - alpha)) / 255)<<16;
					realcolor += ((DWORD*)data[i])[j] & 0xFF000000;

					//if (((DWORD*)data[i])[j] == 0)
					//	realcolor = 0;
					surfData[index + m*lockedRect.Pitch / 4 + n] = realcolor;
					//surfData[index + m*lockedRect.Pitch / 4 + n] = ((DWORD*)data[i])[j];
				}
			}
		}
	}
	(*surf)->UnlockRect();

	return true;
}

bool BMP::CreateSurf_Clip(LPDIRECT3DDEVICE9 device, LPDIRECT3DSURFACE9 * surf
	, POINT surfsrc, POINT clientrectsize, POINT &surfbase, bool &surfclipped, float zoom, bool clip
	, D3DCOLOR color, short rotate)
{
	//�쳣���
	if (width <= 0 || height <= 0
		|| !data)
		return false;
	if (clientrectsize.x <= 0 || clientrectsize.y <= 0)
		return false;
	if (!device)
		return false;
	if (zoom <= 0)
		return false;

	HRESULT hr;

	//���ԭ����
	if (*surf)
	{
		(*surf)->Release();
		(*surf) = NULL;
	}

	//�Ŵ���ⶨsurface�ĳߴ�
	UINT32 zoomw = (UINT32)(width*zoom);
	UINT32 zoomh = (UINT32)(height*zoom);

	//Ҫ������surfaceʵ�ʴ�С
	POINT surfsize = { (LONG)zoomw, (LONG)zoomh };
	//ʵ��surface��㣨���Ͻǣ����ⶨsurface������
	POINT surfbias = { 0, 0 };

	surfclipped = false;
	if (clip)
	{
		//surface�������������ж�(�봰�ڿͻ����޽���)
		if (surfsrc.x > (LONG)zoomw || surfsrc.y > (LONG)zoomh//�������򳬳��Ŵ�surface�һ���
			|| surfsrc.x + clientrectsize.x <= 0 || surfsrc.y + clientrectsize.y <= 0//�������²���surface�����
			)
		{
			//������surface����
			/*hr = device->CreateOffscreenPlainSurface(
				1, 1
				, D3DFMT_A8R8G8B8
				, D3DPOOL_SYSTEMMEM
				, surf
				, NULL);*/
			return false;
		}

		//clip���surface�ߴ�,clip���surface�����clipǰƫ�ƣ�ƫ�����ڸ���bmp����surface���أ�
		//���޸�clip���surface������backbuffer����ʼ��
		if (surfsrc.x > 0)//surface�����clip
		{
			surfsize.x -= surfsrc.x;
			surfbias.x += surfsrc.x;
			surfbase.x = 0;

			surfclipped = true;
		}
		else
		{
			surfbase.x = -surfsrc.x;
		}
		if (surfsrc.y > 0)//surface�ϲ���clip
		{
			surfsize.y -= surfsrc.y;
			surfbias.y += surfsrc.y;
			surfbase.y = 0;

			surfclipped = true;
		}
		else
		{
			surfbase.y = -surfsrc.y;
		}
		if ((LONG)zoomw - surfsrc.x > clientrectsize.x)//surface�Ҳ���clip
		{
			surfsize.x -= zoomw - surfsrc.x - clientrectsize.x;

			surfclipped = true;
		}
		if ((LONG)zoomh - surfsrc.y > clientrectsize.y)//surface�²���clip
		{
			surfsize.y -= zoomh - surfsrc.y - clientrectsize.y;

			surfclipped = true;
		}
	}

	//�½�surface
	hr = device->CreateOffscreenPlainSurface(
		surfsize.x, surfsize.y
		, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM
		, surf
		, NULL);

	if (FAILED(hr))//����surfaceʧ��
	{
		//MessageBoxW(NULL, L"CreateOffscreenPlainSurface in BMP FAILED!", L"", 0);
		return false;
	}

	//����surface����
	D3DLOCKED_RECT lockedRect;
	(*surf)->LockRect(&lockedRect, NULL, NULL);
	DWORD *surfData = (DWORD*)lockedRect.pBits;

	//����
	DWORD realcolor = 0;
	byte alpha = 0;
	int index = 0;
	int zoomint = (int)zoom;

	if (zoom < 1)
	{
		COLORTYPE xpixspan = (COLORTYPE)width / zoomw;//������x������
		COLORTYPE ypixspan = (COLORTYPE)height / zoomh;//������y������
		COLORTYPE spanarea = xpixspan*ypixspan;//��������� x*y
		COLORTYPE *xbase = new COLORTYPE[zoomw + 1];//������������ʼ�㣬x����
		COLORTYPE *ybase = new COLORTYPE[zoomh + 1];//������������ʼ�㣬y����
		for (UINT16 i = 0; i < zoomw + 1; i++)
			xbase[i] = (COLORTYPE)i*width / zoomw;
		for (UINT16 i = 0; i < zoomh + 1; i++)
			ybase[i] = (COLORTYPE)i*height / zoomh;

		COLOR_F3 rcolor = 0;//��ɫ�ۼ���
		for (int i = 0; i < surfsize.y; i++)
		{
			int starty = (int)ybase[i + surfbias.y];//������������ʼ�㣬y����
			int endy = ceilf2i(ybase[i + surfbias.y + 1]);//���������Ͻ����㣬y����
			for (int j = 0; j < surfsize.x; j++)
			{
				index = i*lockedRect.Pitch / 4 + j;

				int startx = (int)xbase[j + surfbias.x];//������������ʼ�㣬x����
				int endx = ceilf2i(xbase[j + surfbias.x + 1]);//���������Ͻ����㣬x����

				rcolor = 0;
				COLORTYPE floatsy, floatey;//��ȷ����յ�

				//��Χ��������ɫ�ۼ�
				for (int m = starty; m < endy; m++)
				{
					if (m == (int)ybase[i + surfbias.y])
						floatsy = ybase[i + surfbias.y];
					else
						floatsy = (float)m;
					if (ybase[i + surfbias.y + 1] - m < 1)
						floatey = ybase[i + surfbias.y]+ ypixspan;
					else
						floatey = (float)m + 1;

					for (int n = startx; n < endx; n++)
					{
						COLORTYPE floatsx, floatex;
						if (n == (int)xbase[j + surfbias.x])
							floatsx = xbase[j + surfbias.x];
						else
							floatsx = (float)n;
						if (xbase[j + surfbias.x + 1] - n < 1)
							floatex = xbase[j + surfbias.x]+ xpixspan;
						else
							floatex = (float)n + 1;

						DWORD bmppixel = ((DWORD*)data[m])[n];
						//if (endx - startx == 1 && endy - starty == 1)//��ֹ�Ŵ�󣬴��������ɫ�����ȣ�����������
						//	rcolor.Add(bmppixel);
						//else
							rcolor.Add(bmppixel, (floatey - floatsy) * (floatex - floatsx), spanarea);
					}
				}

				/*DWORD bmppixel = rcolor.GetDW();
				alpha = (byte)(bmppixel >> 24);
				realcolor = ((bmppixel & 0xFF)*alpha + (color & 0xFF)*(255 - alpha)) / 255;
				realcolor += (((bmppixel >> 8 & 0xFF)*alpha + (color >> 8 & 0xFF)*(255 - alpha)) / 255) << 8;
				realcolor += (((bmppixel >> 16 & 0xFF)*alpha + (color >> 16 & 0xFF)*(255 - alpha)) / 255) << 16;
				realcolor += (bmppixel & 0xFF000000);*/

				//rcolor.Normalize();
				surfData[index] = rcolor.AlphaBlend(color);
			}
		}
		delete[] xbase;
		delete[] ybase;
	}
	else
	{
		for (int i = 0; i < surfsize.y; i++)
		{
			for (int j = 0; j < surfsize.x; j++)
			{
				index = i*lockedRect.Pitch / 4 + j;

				DWORD bmppixel = ((DWORD*)data[(i + surfbias.y) / zoomint])[(j + surfbias.x) / zoomint];//��ȡbmp��Ϣʱ����surfaceƫ��
				alpha = (byte)(bmppixel >> 24);
				realcolor = ((bmppixel & 0xFF)*alpha + (color & 0xFF)*(255 - alpha)) / 255;
				realcolor += (((bmppixel >> 8 & 0xFF)*alpha + (color >> 8 & 0xFF)*(255 - alpha)) / 255) << 8;
				realcolor += (((bmppixel >> 16 & 0xFF)*alpha + (color >> 16 & 0xFF)*(255 - alpha)) / 255) << 16;
				realcolor += (bmppixel & 0xFF000000);
				//realcolor = bmppixel*alpha + color*(255 - alpha);

				surfData[index] = realcolor;
			}
		}
	}

	(*surf)->UnlockRect();

	return true;
}

bool BMP::GetPixel(int x, int y, DWORD *dest)
{
	if(!data)
		return false;

	if (!data[y])
		return false;

	if (x >= width || y >= height)
		return false;

	*dest = ((DWORD*)data[y])[x];

	return true;
}

void BMP::Clear()
{
	if (data)
	{
		for (int i = 0; i < height; i++)
		{
			if (data[i])
			{
				delete[] data[i];
				data[i] = NULL;
			}
		}

		delete[] data;
		data = NULL;
	}
	width = 0;
	height = 0;
}

bool BMP::Empty()
{
	if(!data)
		return true;

	return false;
}

UINT16 roundf2ui16(float f)
{
	if (f > 0)
		return (UINT16)(f + 0.5);
	else
		return (UINT16)(f - 0.5);
}

DWORD roundf2d(float f)
{
	if (f > 0)
		return (DWORD)(f + 0.5);
	else
		return (DWORD)(f - 0.5);
}

LONG roundf2l(float f)
{
	if (f > 0)
		return (LONG)(f + 0.5);
	else
		return (LONG)(f - 0.5);
}

float roundbf2f(float f)
{
	if (f > 0)
		return (float)(byte)(f + 0.5);
	else
		return (float)(byte)(f - 0.5);
}

int ceilf2i(float f)
{
	if ((int)f != f)
		return (int)(f + 1);
	else
		return (int)f;
}
