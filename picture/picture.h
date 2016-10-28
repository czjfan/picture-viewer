#pragma once

#include "resource.h"
#include "shellapi.h"
#include <windows.h>
#include <Commdlg.h>
#include "time.h"
#include <io.h>

#include <d3d9.h>
#include <d3dx9tex.h>

#include "BMP.h"
#include "Pic.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define PICLISTLEN_INIT				2
#define SIZE_NEAR_PIXEL				8

//�ڷŴ���С��1������£��ⶨ��surface����С�����³ߴ磬��ǿ��clip
#define MAX_FORCECLIP_WIDTH			1000
#define MAX_FORCECLIP_HEIGHT		MAX_FORCECLIP_WIDTH

#define MAX_FORCECLIP_WIDTH_DRAG	2000
#define MAX_FORCECLIP_HEIGHT_DRAG	MAX_FORCECLIP_WIDTH_DRAG

//֡��
#define NORMAL_FPS					70
#define MIN_FPS						16

//��־
#define ZOOMFLAG_DELAY				400
#define SIZEFLAG_DELAY				1
#define MOVEFLAG_DELAY				1

//zoom
#define MAX_ZOOM					200
#define MIN_ZOOM					0.001
#define MIN_DELTAZOOM				0.001

//����ģʽ
#define MODE_PIC					0
#define MODE_CMD					1

//���λ��
#define CURSORPOS_PIC				0
#define CURSORPOS_BLANK				1
#define CURSORPOS_OUTWINDOW			2

//������ʾ
#define TEXTMARGIN_SIDE				3
#define TEXTMARGIN_TOP				0
#define TEXTMARGIN_BOTTOM			2
#define TEXTMARGIN_ROW				2

//��ɫ
#define COLOR_BKG0					D3DCOLOR_ARGB(200, 238, 243, 249)
#define COLOR_BKG1					D3DCOLOR_ARGB(200, 255, 255, 255)
#define COLOR_TEXT0					D3DCOLOR_ARGB(160, 255, 255, 255)
#define COLOR_TEXT1					D3DCOLOR_ARGB(180, 90, 0, 255)
#define COLOR_BKG					COLOR_BKG0

const string cursorposshow[3] = { "PIC", "BLANK", "OUT WINDOW"};
const string clipshow[2] = {""};


//
//������Ϣ&״̬
//
HWND mainwnd;							//�����ھ��
RECT wndrect, clientrect;				//���������򣬿ͻ�����
POINT clientsize;						//�ͻ�����С
POINT cursor, lastcursor, lastpos;		//��ǰ������꣨ȫ�֣���lastpos�����϶�ʱ��¼��һλ�ã���lastcursor���ɻ���
int wbias, hbias;						//������ͻ�����Ȳ�

int wlimit, hlimit;						//���ڳ�����λ
D3DCOLOR backcolor;						//����ɫ
time_t loopcount;						//��ѭ������
UINT8 mode;								//ģʽ
bool ondrag;							//�Ƿ����϶�ͼ��״̬
bool onzoom;							//�Ƿ��ڷŴ�
bool onsize;
bool onmove;
bool surffailed;						//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp�����
bool sizeEnable;						//�Ƿ�����ı䴰�ڴ�С
bool flagshow;
bool infoshow;

long oldstyle;

POINT curpixel;							//��굱ǰλ�ö�Ӧԭͼ������
DWORD pixelcolor;						//��굱ǰλ�ö�Ӧԭͼ����ɫ
DWORD screencolor;						//��굱ǰλ�ö�Ӧ��Ļ����ɫ
UINT8 cursorpos;						//��ǰ���λ�ã����ࣩ

short mousestate;						//��ǰ���λ�ã���������������ͣ�

//
//��ʱ
//
LARGE_INTEGER frequency, stime, etime;	//
time_t nowtime, lasttime;				//�뼶��ʱ
LARGE_INTEGER lastwheeltick, wheeltick;	//���ּ�ʱ
time_t zoomtick, sizetick, movetick;	//��־��ʱ
float fps;								//֡��

//
//D3D
//
LPDIRECT3D9	lpD3D;
LPDIRECT3DDEVICE9 maindevice;
D3DPRESENT_PARAMETERS d3dpp;
D3DDISPLAYMODE displaymode;				//��ʾģʽ
D3DCAPS9 caps;							//�豸����
int vertexprocessing;					//vertexprocessing��ʽ

//
//����������Ϣ
//
WCHAR strFileName[MAX_PATH];
D3DSURFACE_DESC surfDesc;
LPDIRECT3DSURFACE9 mainsurf, tempsurf;
bool clip;								//surface�����Ƿ�clip����ȥ�ͻ���������
float zoom;								//ֱ�ӲٿصķŴ���
float oldzoom;							//��һ���Ŵ���
float realzoom;							//ʵ��ʹ�õķŴ���
float actualzoomx, actualzoomy;			//ʵ��x,y�����ϵķŴ���
int zoomw, zoomh;						//�Ŵ�realzoom�����ⶨsurface�ĳߴ�
POINT surfsrc;							//�Ŵ�realzoom�����ⶨsurface�����Ͻǵ����ڿͻ���������
POINT surfbase;							//surface������backbuffer����ʼ��
RECT surfrect;
bool outsideclient;						//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩
bool surfclipped;						//��ǰsurface�Ƿ�clip
bool picclipped;						//��ǰͼƬ�Ƿ�clip


//_finddata_t filegroup;
//UINT32 picnum;
UINT32 piccount;
UINT32 piclistlen;
PicPack *piclist;
PicPack *mainpack;
//
//λͼ��
//
//����λͼ���surface��Ϣ��(surfsrc)
PicInfo imginfo0;
D3DXIMAGE_INFO imginfo;
BMP mainbmp;

//
//��Ϣ��ʾ
//
LPD3DXFONT font;
LPD3DXFONT font2;
RECT textrect;
RECT textrect2;
WCHAR infowstr[1024];

//
//��ʼ�� & ����
//
inline short GetSizeType(CPoint);
inline void SetCursor();					//������������С�����������ʽ
inline void Get2WndRect();					//��ô��ںͿͻ�����С
bool Init();
bool D3DInit();
bool InitDevice();
bool OnDropFile(WCHAR file[]);
bool OnLoadFile(LPWSTR file);
inline void DelayFlag();
inline void ClearFlag();

//
//ͼ��
//
bool FitWnd();
void CenterPic();
void Togglezoom(WPARAM wParam);
bool Zoom(float oldzoom, float zoom);
inline bool RefreshSurf();
inline void DisplayStatusChange(bool winchange = true, bool surfchange = true);			//��ʾ״̬�ı�Ĵ���refreshsurf��ͼƬλ�ã����ڸı�֮������
inline void SurfStatusChange();
inline void CalcSurfMapInfo();				//���ݵ�ǰclip��־��surfsrc�����ڿͻ����ߴ磬����surface������backbuffer����ʼ�������
inline void CalcClipped();					
inline void GetCurInfo();					//��ȡ��ǰ���λ�ö�ӦͼƬ������
inline void GetCurPos();					//��ȡ��ǰ���λ��(����)
inline bool GetCurColor();					//����GetCurPos�õ������أ���ȡ��Ӧ����ɫ
inline bool NoPic();
inline void Clear();

//
//D3D
//
inline bool InfoRender();
inline bool LoadBackbuffer();
inline bool Render();
bool ResetDevice();
void OnLostDevice();
bool OnResetDevice();

//
//����
//
string GetFMT(D3DFORMAT);
inline int Widthof(const RECT &rect);
inline int Heightof(const RECT &rect);
inline bool inside(const POINT &point, const RECT &rect);