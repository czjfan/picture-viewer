#include "stdafx.h"
#include "BMP.h"
#include "generalgeo.h"

DWORD ALPHABLEND::lastcolor1 = 0;
DWORD ALPHABLEND::lastresult1 = 0;
DWORD ALPHABLEND::lastcolor2 = 0;
DWORD ALPHABLEND::lastresult2 = 0;

byte **ALPHABLEND::alphablendtable1 = NULL;
byte **ALPHABLEND::alphablendtable2 = NULL;

byte ***ALPHABLEND::alphablendcube1 = NULL;

BMP::BMP()
{
	//set_new_handler(newhandlerfunc);
	width = 0;
	height = 0;

	data = NULL;
	bAlpha = false;
}

BMP::~BMP()
{
	if (data)//�ͷ�
	{
		for (int i = 0; i < height; i++)
		{
			SAFE_DELETE_LIST(data[i]);
		}
		SAFE_DELETE_LIST(data);
	}
}

void BMP::SetAlpha(bool alpha)
{
	bAlpha = alpha;
}

bool BMP::Load(LPDIRECT3DSURFACE9 surf)
{
	if (!surf)
		return false;

	//surf����
	D3DSURFACE_DESC surfaceDesc;
	surf->GetDesc(&surfaceDesc);

	//���surf����
	int oldheight = height;//�����ͷ�ԭͼ
	width = surfaceDesc.Width;
	height = surfaceDesc.Height;
	if (width <= 0 || height <= 0)
		return false;

	//���ԭͼ
	if (data)
	{
		for (int i = 0; i < oldheight; i++)
		{
			SAFE_DELETE_LIST(data[i]);
		}
		DELETE_LIST(data);
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
	int pitch = lockedRect.Pitch / 4;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			((DWORD*)data[i])[j] = surfData[i*pitch + j];
		}
	}

	//����surface����
	surf->UnlockRect();

	return true;
}

bool BMP::UpLoad(LPDIRECT3DSURFACE9 surf)
{
	if (!surf || isEmpty())
		return false;

	//surf����
	D3DSURFACE_DESC surfaceDesc;
	surf->GetDesc(&surfaceDesc);

	//����surface����
	D3DLOCKED_RECT lockedRect;
	surf->LockRect(&lockedRect, NULL, NULL);
	DWORD *surfData = (DWORD*)lockedRect.pBits;

	//����
	int index = 0;
	int wrange = min((int)surfaceDesc.Width, width);//��Ҫ����������
	int hrange = min((int)surfaceDesc.Height, height);
	for (int i = 0; i < hrange; i++)
	{
		for (int j = 0; j < wrange; j++)
		{
			index = i*lockedRect.Pitch / 4 + j;

			surfData[index] = ((DWORD*)data[i])[j];
		}
	}
	surf->UnlockRect();

	return true;
}

bool MYCALL1 BMP::Gray()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			byte *ppix = (byte*)&((DWORD*)data[i])[j];
			byte gray = RGB2GRAY(ppix[0], ppix[1], ppix[2]);
			ppix[0] = ppix[1] = ppix[2] = gray;
		}
	}
	return true;
}

bool MYCALL1 BMP::Inverse()
{
	for (int i = 0; i < height; i++)
	{
		byte *ppix = data[i];
		for (int j = 0; j < width; j++)
		{
			/*ppix[0] = ~ppix[0];
			ppix[1] = ~ppix[1];*/
			*(UINT16*)ppix = ~(*(UINT16*)ppix);
			ppix[2] = ~ppix[2];
			ppix += 4;
		}
	}
	return true;
}

bool MYCALL1 BMP::InverseAlpha()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			byte *ppix = (byte*)&((DWORD*)data[i])[j];
			ppix[3] = 255 - ppix[3];
		}
	}
	return true;
}

bool MYCALL1 BMP::InverseAll()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			byte *ppix = (byte*)&((DWORD*)data[i])[j];
			ppix[0] = 255 - ppix[0];
			ppix[1] = 255 - ppix[1];
			ppix[2] = 255 - ppix[2];
			ppix[3] = 255 - ppix[3];
		}
	}
	return true;
}

bool MYCALL1 BMP::LOSE_R()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			((byte*)&((DWORD*)data[i])[j])[2] = 0;
		}
	}
	return true;
}

bool MYCALL1 BMP::LOSE_G()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			((byte*)&((DWORD*)data[i])[j])[1] = 0;
		}
	}
	return true;
}

bool MYCALL1 BMP::LOSE_B()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			((byte*)&((DWORD*)data[i])[j])[0] = 0;
		}
	}
	return true;
}

bool MYCALL1 BMP::RGB2HSV()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			byte *ppix = (byte*)&((DWORD*)data[i])[j];

			byte r = ppix[2];
			byte g = ppix[1];
			byte b = ppix[0];
			byte max = 0;
			byte min = 1;
			if (g > b)
			{
				max = 1;
				min = 0;
			}
			if (r > ppix[max])
				max = 2;
			else if (r < ppix[min])
				min = 2;
			byte minvalue = ppix[min];
			byte maxvalue = ppix[max];

			ppix[0] = maxvalue;//v
			ppix[1] = (maxvalue == 0 ? 0 : ROUND_BYTE(255.0f*(maxvalue - minvalue) / maxvalue));//s

			if (maxvalue == minvalue)
				ppix[2] = 0;
			else
			{
				if (max == 2)
				{
					ppix[2] = ROUND_BYTE(255.0f * (60 * (g - b) / (maxvalue - minvalue)) / 360);
				}
				else if (max == 1)
				{
					ppix[2] = ROUND_BYTE(255.0f * (120 + 60 * (b - r) / (maxvalue - minvalue)) / 360);
				}
				else
				{
					ppix[2] = ROUND_BYTE(255.0f * (240 + 60 * (r - g) / (maxvalue - minvalue)) / 360);
				}
			}
		}
	}
	return true;
}

void MYCALL1 BMP::Clear()
{
	if (data)//�ͷŶ�ά�ռ�
	{
		for (int i = 0; i < height; i++)
		{
			SAFE_DELETE_LIST(data[i]);
		}

		DELETE_LIST(data);
	}
	width = 0;
	height = 0;
}

SurferBase::SurferBase()
{
	pBmp = NULL;
	pBufferW = NULL;
	pBufferH = NULL;
	bHasPic = false;
	bNeedRenew = false;

	QueryPerformanceFrequency(&freq);
	wheeltick.QuadPart = 0;
	lastwheeltick.QuadPart = 0;

	bClip = true;
	zoom = 1.0f;
	lastZoom = 1.0f;
	actualZoomX = 1.0f;
	actualZoomY = 1.0f;
	zoomW = 0;
	zoomH = 0;

	ZEROPOINT(surfBase);
	ZEROPOINT(basePoint);

	ZEROPOINT(surfSize);
	bPicClipped = false;
	bOutClient = false;
}

void SurferBase::SurfAdjustZoom_DragPR(int wParam)
{
	float adds = 0;

	//��ȡ��ʱ�����
	adds = ZOOMFACTOR_DRAG*zoom;

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

	zoom += adds;
	//zoom������
	if (zoom > ZOOM_MAX)
		zoom = ZOOM_MAX;
	if (zoom < ZOOM_MIN)
		zoom = ZOOM_MIN;

	PostZoomChange();
}

void SurferBase::SurfAdjustZoom_WheelPR(int wParam)
{
	float adds = 0;
	QueryPerformanceCounter(&wheeltick);

	//�仯�������ʱ��͵�ǰ�Ŵ���
	adds = (sqrtf(freq.QuadPart / (float)(wheeltick.QuadPart - lastwheeltick.QuadPart))
		*(ZOOMFACTOR_WHEEL*zoom));
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

	zoom += adds;
	//zoom������
	if (zoom > ZOOM_MAX)
		zoom = ZOOM_MAX;
	if (zoom < ZOOM_MIN)
		zoom = ZOOM_MIN;
	/*if (zoom*min(*pBmpW, *pBmpH) < 1)
	zoom = 1.00001f / min(*pBmpW, *pBmpH);//����surface�ߴ�>0*/

	PostZoomChange();
}

inline void Surfer::CalcBindState()
{
	bHasPic = (pBmp != NULL);
	bHasDevice = pDevice && pBufferW && pBufferH;
	bHasBoth = bHasDevice && bHasPic;
}

inline void	Surfer::CalcMapInfo()
{
	if (bHasDevice)
	{
		//����rcSurf��surfDest
		//surfDest��surface������backbuffer��㣬����surfaceʱ�����������Ҫʵʱ���㣨��surf�ƶ����ǲ���Ҫˢ�£�
		if (surfBase.x < 0)
			surfDest.x = 0;
		else
			surfDest.x = surfBase.x;
		if (surfBase.y < 0)
			surfDest.y = 0;
		else
			surfDest.y = surfBase.y;

		if (bClip)
		{
			rcSurf.left = 0;
			rcSurf.top = 0;
			rcSurf.right = min((int)*pBufferW - surfDest.x, surfSize.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
			rcSurf.bottom = min((int)*pBufferH - surfDest.y, surfSize.y);
		}
		else
		{
			rcSurf.left = max(-surfBase.x, 0);
			rcSurf.top = max(-surfBase.y, 0);
			rcSurf.right = min(zoomW, rcSurf.left + (int)*pBufferW - surfDest.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
			rcSurf.bottom = min(zoomH, rcSurf.top + (int)*pBufferH - surfDest.y);
		}
	}
}

Surfer::Surfer() {
	surf = NULL;
	pDevice = NULL;
	bHasDevice = false;
	bHasBoth = false;

	//surfrenewcount = 0;
	renewTime = -0.001f;
	sampleSchema = SAMPLE_SCHEMA_UNKNOWN;
	strSurfInfo[0] = (WCHAR)'\0';

	bSurfFailed = true;
	bSurfClipped = false;

	ZEROPOINT(surfDest);
	ZERORECT(rcSurf);


	SETPOINT(cursorPixel, -1, -1);
	bCursorOnPic = false;
	cursorColor = 0;
	cursorPos = CURSORPOS_BLANK;
	
	// alphablend���ʼ��
	ALPHABLEND::InitAlphBlendTable(TRANSPARENTBACK_FILLDENSITY, TRANSPARENTBACK_HOLLOWDENSITY);

	// ��Ϣ�ַ�����ʼ��
	SetInfoStr();
}

Surfer::~Surfer()
{
	SAFE_RELEASE(surf);
	SAFE_RELEASE(pDevice);
}

bool Surfer::BindDevice(LPDIRECT3DDEVICE9 device)
{
	SAFE_RELEASE(pDevice);//�������
	pDevice = device;
	if(pDevice)
		pDevice->AddRef();//�������ü���

	LPDIRECT3DSURFACE9 pSurf;
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_LEFT, &pSurf);
	CalcBindState();

	return pDevice != NULL;
}

bool Surfer::BindBuf(UINT * pBufW, UINT * pBufH)
{
	pBufferW = pBufW;
	pBufferH = pBufH;

	CalcBindState();

	return pBufferW && pBufferH;
}

bool Surfer::BindPic(PicPack * ppic, bool renew)
{
	if (ppic)
	{
		// ����ָ��
		pBmp = ppic->GetPBMP();
		/*pBmpW = &pBmp->width;
		pBmpH = &pBmp->height;*/

		// ��������״̬
		CalcBindState();

		// ��Ϣ����
		SurfLocatePR(ppic->GetBase());
		SurfSetZoomPR(ppic->GetZoom());
		lastZoom = zoom;

		if (renew)
			SurfRenew(true);
		SetInfoStr();// ������Ϣ�ַ���
	}
	else
	{
		Clear();
	}

	return pBmp != NULL;
}

bool Surfer::DeBindPic(PicPack *ppic)
{
	if (ppic)
	{
		ppic->SetSrc(surfBase);
		ppic->SetZoom(zoom);

		CalcBindState();

		return true;
	}
	else
		return false;
}

void Surfer::Clear()
{
	SAFE_RELEASE(surf);//�ͷ�surface
	pBmp = NULL;

	zoomW = 0;
	zoomH = 0;

	actualZoomX = 0;
	actualZoomY = 0;
	bSurfFailed = true;
	bSurfClipped = false;

	ZEROPOINT(surfSize)
	bPicClipped = false;
	bOutClient = false;

	ZEROPOINT(cursorPixel)
	cursorColor = 0;
	cursorPos = CURSORPOS_BLANK;//���±�־

	CalcBindState();
	SetInfoStr();
}

bool Surfer::SurfSuit(int w, int h)
{
	if (bHasBoth)
	{
		if (w < 1 || h < 1)
			return false;

		return SurfZoomRenew(min((float)w / pBmp->width, (float)h / pBmp->height), -surfBase.x, -surfBase.y);
	}
	else
		return false;
}

void Surfer::SetInfoStr()
{
	WCHAR subinfo[256] = { 0 };
	strSurfInfo[0] = L'\0';

	// buffer�ߴ硢surface�ߴ�
	// ���ű���
	// surface��ʼ��
	swprintf_s(subinfo, L"SURFACE: %d �� %d     TIME: %.3fms (%S)\n\
		ZOOM: %.4f\n\
		BASE: %d, %d\n"
		, surfSize.x, surfSize.y, renewTime*1000.0f, GetSampleSchemaStr()
		, zoom
		, surfBase.x, surfBase.y);
	wcscat_s(strSurfInfo, subinfo);

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
	wcscat_s(strSurfInfo, subinfo);

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
	wcscat_s(strSurfInfo, subinfo);

	// ������Ϣ
	//"intended surface: %d�� %d\n
	//	clipsurface base: %d, %d\n
	//	PZ: X %.4f Y %.4f\n"
	//	, zoomW, zoomH
	//	, surfDest.x, surfDest.y
	//	, actualZoomX, actualZoomY
}

void Surfer::GetCurInfo(POINT *cursor, RECT *rcClient)
{
	if (bHasPic)
	{
		if (!cursor || !rcClient)
			return;

		// ��õ�ǰ���λ�ã����ࣩ
		POINT surf2cursor = { cursor->x - rcClient->left - surfBase.x
			, cursor->y - rcClient->top - surfBase.y };
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
		cursorPixel.x = (LONG)((surf2cursor.x + 0.5f) / zoom);
		cursorPixel.y = (LONG)((surf2cursor.y + 0.5f) / zoom);

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

char* Surfer::GetSampleSchemaStr()
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

void ALPHABLEND::InitAlphBlendTable(byte color1, byte color2)
{
	if (alphablendtable1)
	{
		for (int i = 0; i < 256; i++)
		{
			if (alphablendtable1[i])
				delete[] alphablendtable1[i];
		}
		delete[] alphablendtable1;
	}
	if (alphablendtable2)
	{
		for (int i = 0; i < 256; i++)
		{
			if (alphablendtable2[i])
				delete[] alphablendtable2[i];
		}
		delete[] alphablendtable2;
	}

	alphablendtable1 = new byte*[256];
	alphablendtable2 = new byte*[256];
	for (int i = 0; i < 256; i++)
	{
		alphablendtable1[i] = new byte[256];
		alphablendtable2[i] = new byte[256];
		for (int j = 0; j < 256; j++)//i��Դͨ��ֵ��j��alpha��color1��color2��Ŀ��ͨ��ֵ
		{

			alphablendtable1[i][j] = (i*j + (255 - j)*color1) / 255;
			alphablendtable2[i][j] = (i*j + (255 - j)*color2) / 255;
		}
	}
	lastcolor1 = 0;
	lastresult1 = color1;
	lastcolor2 = 0;
	lastresult2 = color2;
}

void ALPHABLEND::InitAlphBlendTCube()
{
	const int alphalevels = 20;//alpha��Ϊ���ټ������ǵ�����alpha��float��ʽ��

	alphablendcube1 = new byte**[alphalevels];
	for (int i = 0; i < alphalevels; i++)//i,j:src��dest��k:alpha
	{
		alphablendcube1[i] = new byte*[256];
		for (int j = 0; j < 256; j++)
		{
			alphablendcube1[i][j] = new byte[256];

			for (int k = 0; k < 256; k++)
			{
				alphablendcube1[i][j][k]
					= (byte)((i*j + (float)(alphalevels - 1 - i)*k) / (alphalevels - 1));
			}
		}
	}
}

PicPack::PicPack() 
{
	pBmp = new BMP;

	ZeroMemory(&myPicInfo, sizeof(myPicInfo));
	ZeroMemory(&D3DPicInfo, sizeof(D3DPicInfo));
	strFileName[0] = L'\0';

	ZEROPOINT(src)
	zoom = 1.0f;

	bSaved = false;
	bHasDirectory = false;
}

PicPack::~PicPack()
{
	SAFE_DELETE(pBmp);
	pBmp = NULL;
}

WCHAR * PicPack::GetFileName()
{
	return strFileName;
}

HRESULT PicPack::LoadFile(LPDIRECT3DDEVICE9 pDevice, WCHAR file[])
{
	if (!pDevice)
		return E_FAIL;

	// ��ȡͼƬ��Ϣ
	HRESULT hr;
	ZeroMemory(&D3DPicInfo, sizeof(D3DXIMAGE_INFO));
	D3DXGetImageInfoFromFileW(file, &D3DPicInfo);
	/*if (D3DPicInfo.Width == 0 || D3DPicInfo.Height == 0)
	{
	return false;
	}*/
	//��ȡ������Ϣ
	if(!myPicInfo.ReadFile(file))
		return E_FAIL;

	// ������ͼƬƥ�����
	LPDIRECT3DSURFACE9 tempsurf = NULL;
	hr = pDevice->CreateOffscreenPlainSurface(
		(LONG)myPicInfo.width, (LONG)myPicInfo.height, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM, &tempsurf, NULL);
	FAILED_RETURN(hr);
	// װ��ͼƬ
	hr = D3DXLoadSurfaceFromFileW(
		tempsurf, NULL, NULL, file
		, NULL, D3DX_FILTER_NONE, 0x00000000, NULL);
	FAILED_RETURN(hr);
	//const DWORD TT = S_FALSE;
	//S_OK;

	// ����BMP
	if (!pBmp->Load(tempsurf)) return E_FAIL;//��tempsurface����ͼƬ
	pBmp->SetAlpha(myPicInfo.channels == 4);//����alpha

	// �ļ���
	wcscpy_s(strFileName, file);

	// ״̬����
	bHasDirectory = true;
	bSaved = true;

	// ���surface
	SAFE_RELEASE(tempsurf);

	return S_OK;
}

bool PicPack::SaveFile(LPDIRECT3DDEVICE9 pDevice, WCHAR file[])
{
	if (!pDevice || pBmp->isEmpty())
	{
		return false;
	}
	HRESULT hr;

	LPDIRECT3DSURFACE9 tempsurf = NULL;
	hr = pDevice->CreateOffscreenPlainSurface(
		pBmp->width, pBmp->height
		, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM
		, &tempsurf
		, NULL);
	FAILED_RETURN_FALSE(hr);

	FALSE_RETURN(pBmp->UpLoad(tempsurf));//��tempsurface����ͼƬ
	hr = D3DXSaveSurfaceToFile(file, D3DPicInfo.ImageFileFormat, tempsurf, NULL, NULL);//���浽�ļ�

	// �ļ���
	wcscpy_s(strFileName, file);

	// ״̬����
	bSaved = true;

	return !FAILED(hr);
}

WCHAR * PicPack::GetPicInfoStrW()
{
	//ͼƬ�ߴ硢��ʽ
	swprintf_s(picInfoStr, _T("SIZE: %d �� %d\n\
		FORMAT: %S\n\
		FILE: %lld Bytes  [%.3fMB]\n\
		DEPTH: %d\n\
		CHANNEL: %d\n\
		ALPHA: %d")
		, pBmp->width, pBmp->height
		, GetFMTStr(D3DPicInfo.Format).c_str()
		, myPicInfo.bytecount, myPicInfo.bytecount / 1048576.0f
		, myPicInfo.generaldepth
		, myPicInfo.channels
		, pBmp->bAlpha);
	return picInfoStr;
}

string GetFMTStr(D3DFORMAT fmt)
{
	switch (fmt)
	{
	case D3DFMT_R8G8B8:
		return "D3DFMT_R8G8B8";
	case D3DFMT_A8R8G8B8:
		return "D3DFMT_A8R8G8B8";
	case D3DFMT_X8R8G8B8:
		return "D3DFMT_X8R8G8B8";
	case D3DFMT_R5G6B5:
		return "D3DFMT_R5G6B5";
	case D3DFMT_X1R5G5B5:
		return "D3DFMT_X1R5G5B5";
	case D3DFMT_A1R5G5B5:
		return "D3DFMT_A1R5G5B5";
	case D3DFMT_A4R4G4B4:
		return "D3DFMT_A4R4G4B4";
	case D3DFMT_R3G3B2:
		return "D3DFMT_R3G3B2";
	case D3DFMT_A8:
		return "D3DFMT_A8";
	case D3DFMT_A8R3G3B2:
		return "D3DFMT_A8R3G3B2";
	case D3DFMT_X4R4G4B4:
		return "D3DFMT_X4R4G4B4";
	case D3DFMT_A2B10G10R10:
		return "D3DFMT_A2B10G10R10";
	case D3DFMT_A8B8G8R8:
		return "D3DFMT_A8B8G8R8";
	case D3DFMT_X8B8G8R8:
		return "D3DFMT_X8B8G8R8";
	case D3DFMT_G16R16:
		return "D3DFMT_G16R16";
	case D3DFMT_A2R10G10B10:
		return "D3DFMT_A2R10G10B10";
	case D3DFMT_A16B16G16R16:
		return "D3DFMT_A16B16G16R16";
	case D3DFMT_A8P8:
		return "D3DFMT_A8P8";
	case D3DFMT_P8:
		return "D3DFMT_P8";
	case D3DFMT_L8:
		return "D3DFMT_L8";
	case D3DFMT_A8L8:
		return "D3DFMT_A8L8";
	case D3DFMT_A4L4:
		return "D3DFMT_A4L4";
	case D3DFMT_V8U8:
		return "D3DFMT_V8U8";
	case D3DFMT_L6V5U5:
		return "D3DFMT_L6V5U5";
	case D3DFMT_X8L8V8U8:
		return "D3DFMT_X8L8V8U8";
	case D3DFMT_Q8W8V8U8:
		return "D3DFMT_Q8W8V8U8";
	case D3DFMT_V16U16:
		return "D3DFMT_V16U16";
	case D3DFMT_A2W10V10U10:
		return "D3DFMT_A2W10V10U10";
	default:
		return "UNKNOWN";
	}
}

PicPackList::PicPackList()
{
	size = PICPACKLIST_SIZE_INIT;
	data = MALLOC(PicPack*, size);
	count = 0;
	cur = 0;

	pLivePicPack = NULL;
	pLiveBMP = NULL;
}

int PicPackList::GetCurPos()
{
	return cur;
}

int PicPackList::GetCount()
{
	return count;
}

PicPack * PicPackList::SetPicPack(int idx)
{
	if (count < 1)
		return NULL;

	if (idx <= count && idx > 0)
	{
		cur = idx;

		pLivePicPack = data[cur - 1];
		pLiveBMP = pLivePicPack->GetPBMP();

		return data[cur - 1];
	}
	else
	{
		cur = 0;
		return NULL;
	}
}

PicPack * PicPackList::SetTailPicPack()
{
	if (count < 1)
		return NULL;

	cur = count;

	pLivePicPack = data[cur - 1];
	pLiveBMP = pLivePicPack->GetPBMP();

	return data[cur - 1];
}

PicPack * PicPackList::SetPrev()
{
	if (count < 1)
		return NULL;

	int oldcur = cur;
	cur--;
	if (cur < 1)
		cur = count;

	if (cur <= count && cur > 0)
	{
		pLivePicPack = data[cur - 1];
		pLiveBMP = pLivePicPack->GetPBMP();

		return data[cur - 1];
	}
	else
	{
		cur = oldcur;

		return NULL;
	}
}

PicPack * PicPackList::SetNext()
{
	if (count < 1)
		return NULL;

	int oldcur = cur;
	cur++;
	if (cur > count)
		cur = 1;

	if (cur <= count && cur > 0)
	{
		pLivePicPack = data[cur - 1];
		pLiveBMP = pLivePicPack->GetPBMP();

		return data[cur - 1];
	}
	else
	{
		cur = oldcur;

		return NULL;
	}
}

PicPack * PicPackList::Drop()
{
	if (count < 1 || cur>count || cur <= 0)
		return NULL;

	delete data[cur - 1];
	for (int i = cur; i <= count - 1; i++)
	{
		data[i - 1] = data[i];
	}
	if (cur == count)
		cur--;
	count--;

	if (count >= 1 && cur <= count && cur > 0)//ɾ������ͼƬ
	{
		pLivePicPack = data[cur - 1];
		pLiveBMP = pLivePicPack->GetPBMP();

		return data[cur - 1];
	}
	else//ɾ�����б���ûͼƬ��
	{
		cur = 0;

		pLivePicPack = NULL;
		pLiveBMP = NULL;

		return NULL;
	}
}

//void PicPackList::Add()
//{
//	if (count >= size)
//	{
//		size += 2;
//		data = (PicPack**)realloc(data, size*sizeof(PicPack*));
//	}
//
//	count++;
//	data[count - 1] = new PicPack;
//}

bool PicPackList::Add(PicPack * newpp)
{
	if (newpp)
	{
		if (count >= size)
		{
			size += PICPACKLIST_SIZE_INCREMENT;
			REALLOC(data, PicPack*, size)
		}
		count++;
		data[count - 1] = newpp;
		return true;
	}
	else
	{
		return false;
	}
}

COLOR_F3::COLOR_F3()
{
	r = g = b = 0;
}

COLOR_F3::COLOR_F3(COLORTYPE f)
{
	r = g = b = f; 
}

void COLOR_F3::Normalize()
{
	if (a > 255.0)
		a = 255.0;
	if (r > 255.0)
		r = 255.0;
	if (g > 255.0)
		g = 255.0;
	if (b > 255.0)
		b = 255.0;

	a = (float)(byte)(a + 0.5f);
	r = (float)(byte)(r + 0.5f);
	g = (float)(byte)(g + 0.5f);
	b = (float)(byte)(b + 0.5f);
}
