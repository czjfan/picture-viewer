#pragma once

/*
//surfer info

LPDIRECT3DSURFACE9 mainsurf;			//��surface
D3DSURFACE_DESC surfDesc;				//��surface��Ϣ

LARGE_INTEGER lastwheeltick, wheeltick;	//���ּ�ʱ
POINT basepoint;//����dragzoom
POINT piccenter;

float surfzoom;							//ʵ��ʹ�õķŴ���
float oldsurfzoom;						//��һ������surface�����õķŴ���
float actualzoomx, actualzoomy;			//ʵ��x,y�����ϵķŴ���
int zoomw, zoomh;						//�Ŵ�realzoom�������surface�ĳߴ�

bool surffailed;						//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp�����
bool clip;								//surface�Ƿ�clip����ȥ�ͻ���������
bool surfclipped;						//��ǰsurface�Ƿ�clip
bool picclipped;						//��ǰͼƬ�Ƿ�clip
bool outsideclient;						//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩
POINT surfsrc;							//�Ŵ�realzoom���ļ���surface�����Ͻǵ����ڿͻ���������
POINT surfbase;							//surface������backbuffer, backbuffer����ʼ��
RECT surfrect;							//surfaceҪ������backbuffer������

SurfParamInit()
:	mainsurf = NULL; 
	basepoint = { 0,0 };
	piccenter = { 0,0 };

	surfzoom = 1.0f;
	oldsurfzoom = 1.0f;
	actualzoomx = 1.0f;
	actualzoomy = 1.0f;
	zoomw = 0;
	zoomh = 0;

	surffailed = true;
	clip = true;
	surfclipped = false;
	picclipped = false;
	outsideclient = false;
	surfsrc = { 0,0 };
	surfbase = { 0,0 };
	surfrect = { 0,0,0,0 };

SurfPicInfoInit(int picw, int pich)
:	surfzoom => zoomw, zoomh
	surfsrc	=> piccenter

SurfHoming()
:	=> surfsrc
	zoomw, zoomh, surfsrc => piccenter

SurfLocate(LONG x, LONG y)
:	x, y => surfsrc
	zoomw, zoomh, surfsrc => piccenter
	
SurfMove(int dx, int dy)
:	dx, dy => surfsrc
	zoomw, zoomh, surfsrc => piccenter
	
SurfSetZoom(float z)
:	z => surfzoom

SurfZoom(POINT client2cursor, bool stable)
:	surfzoom, BMP => zoomw, zoomh
	zoomw, zoomh, BMP => actualzoomx&y
	zoomw, zoomh, piccenter/client2cursor => surfsrc
	zoomw, zoomh, surfsrc => piccenter
	=> clip
	=> oldsurfzoom


*/