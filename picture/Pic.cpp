#include "stdafx.h"
#include "Pic.h"

bool PicInfo::GetFile(WCHAR file[])
{
	ifstream in(file, ios::_Nocreate | ios::binary | 0x02);
	if (!in.is_open())
		return false;

	in.seekg(0, ios::end);
	bytecount = in.tellg();

	return true;
}

bool PicPack::Read(LPDIRECT3DDEVICE9 dev, WCHAR file[])
{
	if (!dev)
	{
		return false;
	}

	//��ȡͼ����Ϣ
	HRESULT hr;
	ZeroMemory(&info, sizeof(D3DXIMAGE_INFO));
	D3DXGetImageInfoFromFile(file, &info);
	//��ȡ������Ϣ
	ZeroMemory(&info0, sizeof(PicInfo));
	if (!info0.GetFile(file))
	{
		return false;
	}

	//����
	LPDIRECT3DSURFACE9 tempsurf = NULL;
	/*if (tempsurf)
		tempsurf->Release();
	tempsurf = NULL;*/
	//������ͼ��ƥ�����
	hr = dev->CreateOffscreenPlainSurface(
		(LONG)info.Width, (LONG)info.Height, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM, &tempsurf, NULL);
	if (FAILED(hr))
	{
		//MessageBoxW(mainwnd, L"CreateOffscreenPlainSurface FAILED!", L"", 0);
		return false;
	}
	//װ��ͼ��
	hr = D3DXLoadSurfaceFromFileW(
		tempsurf, NULL, NULL, file
		, NULL, D3DX_FILTER_NONE, 0x00000000, NULL);
	if (FAILED(hr))
	{
		//MessageBoxW(mainwnd, L"D3DXLoadSurfaceFromFileW FAILED!", L"", 0);
		return false;
	}
	//����BMP
	pic.Load(tempsurf);
	//zoomw = (int)(realzoom*mainbmp.width);
	//zoomh = (int)(realzoom*mainbmp.height);
	//���surface
	tempsurf->Release();
	tempsurf = NULL;

	//��ͼƬ����
	//CenterPic();
	//����Ϊsurface
	//RefreshSurf();
	//��Ⱦ
	//Render();
	//���棨���ԣ�
	//D3DXSaveSurfaceToFile(L"E:\\1.bmp", D3DXIFF_BMP, mainsurf, NULL, NULL);

	//SetWindowTextW(mainwnd, file);

	return true;
}
