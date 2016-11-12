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

#define MYHT_WND					33

//ͼƬ��
#define PICLISTLEN_INIT				2
#define SIZE_NEAR_PIXEL				5

//����״̬����
#define KEYSTATEMASK_DOWN			0X8000

//�ڷŴ���С��1������£������surface����С�����³ߴ磬��ǿ��clip
#define MAX_FORCECLIP_WIDTH			2000
#define MAX_FORCECLIP_HEIGHT		MAX_FORCECLIP_WIDTH

#define MAX_FORCECLIP_WIDTH_DRAG	3000
#define MAX_FORCECLIP_HEIGHT_DRAG	MAX_FORCECLIP_WIDTH_DRAG

//֡��
#define ICONIC_SLEEP				10
#define NORMAL_FPS					70
#define WINDOW_FPS					16
#define MIN_FPS						16

//��־
#define FLAGDELAY_ZOOM				400//onzoom����ѭ������ѭ��Ƶ�����ã�Ҫ����
#define FLAGDELAY_SIZE				1
#define FLAGDELAY_MOVE				1
#define FLAGDELAY_SURFREFRESH		100
#define FLAGDELAY_DRAG				100

//zoom
#define MAX_ZOOM					200
#define MIN_ZOOM					0.001f
#define MIN_DELTAZOOM				0.001f

#define DELTAZOOM1					0.006f
#define DELTAZOOM2					0.003f

//����ģʽ
#define MODE_PIC					0
#define MODE_CMD					1

//����ģʽ
#define WINMODE_NORMALWINDOWS		0
#define WINMODE_ROUND				1

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

const string cursorposshow[3] = { "PIC", "BLANK", "OUTSIDE CLIENT"};
const string clipshow[2] = {""};
const WCHAR yesno1[2] = { L'��', L'��' };
const WCHAR yesno2[2] = { L'��', L'��' };//��

//
//������Ϣ&״̬
//
HWND mainwnd;							//�����ھ��
RECT wndrect, clientrect;				//���������򣬿ͻ�����
POINT clientsize;						//�ͻ�����С
POINT cursor, lastcursor, lastpos;		//��ǰ������꣨ȫ�֣���lastpos�����϶�ʱ��¼��һλ�ã���lastcursor���ɻ���
int wbias, hbias;						//������ͻ�����Ȳ�
int wlimit, hlimit;						//���ڳ�����λ

//״̬��־
bool ondrag;							//�Ƿ����϶�ͼ��״̬
bool dragging;							//�Ƿ����϶�ͼ��״̬
bool onzoom;							//�Ƿ��ڷŴ�
bool onsize;							//�Ƿ������Ŵ���
bool onmove;							//�Ƿ����ƶ�����
bool surfrefresh;						//surface�Ƿ���ˢ��
bool picclipped;						//��ǰͼƬ�Ƿ�clip
//ѡ���־
UINT8 mode;								//ģʽ
UINT8 winmode;							//����ģʽ
D3DCOLOR backcolor;						//����ɫ
bool sizeEnable;						//�Ƿ�����ı䴰�ڴ�С
bool easymoveEnable;
bool flagshow;							//�Ƿ���ʾ״̬��־��Ϣ		
bool infoshow;							//�Ƿ���ʾ������Ϣ
bool fpslimit;
bool screencoloron;						//�Ƿ��ȡ��Ļ��ɫ


POINT picpixel;							//��굱ǰλ�ö�Ӧԭͼ������
DWORD picpixelcolor;					//��굱ǰλ�ö�Ӧԭͼ����ɫ
DWORD screencolor;						//��굱ǰλ�ö�Ӧ��Ļ����ɫ
UINT8 cursorpos;						//��ǰ���λ�ã����࣬�����ĸ�����ͼƬ���հס������⣩

short mousestate;						//��ǰ���λ�ã���������������ͣ�

//
//��ʱ
//
time_t loopcount;						//��ѭ������
LARGE_INTEGER frequency, stime, etime;	//�߾��ȼ�ʱ
time_t nowtime, lasttime;				//�뼶��ʱ
LARGE_INTEGER lastwheeltick, wheeltick;	//���ּ�ʱ
time_t zoomtick, sizetick, movetick\
	, surfrefreshtick, dragtick;		//��־��ʱ
float fps, avgfps, cvgfps, showfps, showavgfps;						//֡��
ULONGLONG fpscount;

//
//D3D
//
LPDIRECT3D9	lpD3D;
LPDIRECT3DDEVICE9 maindevice;			//���豸
D3DPRESENT_PARAMETERS d3dpp;
D3DDISPLAYMODE displaymode;				//��ʾģʽ
D3DCAPS9 caps;							//�豸����
int vertexprocessing;					//vertexprocessing��ʽ

//
//����������Ϣ
//
WCHAR strFileName[MAX_PATH];			//�ļ���
D3DSURFACE_DESC surfDesc;				//��surface��Ϣ
LPDIRECT3DSURFACE9 mainsurf, tempsurf;	//��surface������ʱsurface

float zoom;								//ֱ�ӲٿصķŴ���
float oldzoom;							//��һ���Ŵ���
float realzoom;							//ʵ��ʹ�õķŴ���
float actualzoomx, actualzoomy;			//ʵ��x,y�����ϵķŴ���
bool surffailed;						//surface�Ƿ񴴽�ʧ�ܣ�����mainbmp�����
bool outsideclient;						//surface�ʹ��ڿͻ����Ƿ��޽������ڿͻ����⣩
bool clip;								//surface�����Ƿ�clip����ȥ�ͻ���������
bool surfclipped;						//��ǰsurface�Ƿ�clip
int zoomw, zoomh;						//�Ŵ�realzoom�������surface�ĳߴ�
POINT surfsrc;							//�Ŵ�realzoom���ļ���surface�����Ͻǵ����ڿͻ���������
POINT surfbase;							//surface������backbuffer, backbuffer����ʼ��
RECT surfrect;							//surfaceҪ������backbuffer������


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
PicInfo imginfo0;						//ͼƬ��Ϣ
D3DXIMAGE_INFO imginfo;
BMP mainbmp;							//��ǰͼƬ

//
//��Ϣ��ʾ
//
LPD3DXFONT font;						//��ʾ����
LPD3DXFONT font2;
RECT textrect;							//��ʾ��������
RECT textrect2;
WCHAR infowstr[512];					//��ʾ��Ϣ����

//
//��ʼ�� & ����
//
//����
void SetRoundWnd();
inline void Get2WndRect();				//��ô��ںͿͻ�����С
//���
inline short GetSizeType(CPoint);		//���ݵ�ǰ���꣬���������ͣ��ı䴰�ڴ�С��
inline void SetCursor();				//������������С�����������ʽ
//��ʼ��
bool Init();							//��ʼ��
//D3D��ʼ��
bool D3DInit();							//D3D��ʼ��
bool InitDevice();						//�����豸��������Ⱦ״̬
//�ļ�
bool OnDropFile(WCHAR file[]);			//����ͼƬ��ͳһ�������ͼƬ
bool OnLoadFile(LPWSTR file);			//��ʼ��ʱ����ͼƬ
//��־
inline void DelayFlag();				//�ӳٱ�־������
inline void ClearFlag();				//�����Ծ��־
//����
void BeginDragListen();
void EndDragListen();

//
//ͼ��
//
bool FitWnd();							//���ñ�׼���ڳߴ�
void CenterPic();						//��ͼƬ���У����ڣ�
void SurfAdjustZoom(WPARAM wParam);		//���ݵ���ֵ�������ű���
void SurfMove(int dx, int dy);									//surface�ƶ�������surface���£��ⲿ��������	
void SurfZoom(float oldzoom, float zoom, POINT cursor);			//����
void SurfRotate(POINT center, POINT corsor, POINT cursor2);		//��ת
void SurfManipulate(POINT center, POINT corsor, POINT cursor2);	//��ת��+ƫ�ƣ����ɲٿ�
inline bool SurfRenew();				//��������surface
inline void SurfStatusChange();			//surface״̬�ı䣬RefreshSurf��ִ��
inline void CalcSurfMapInfo();			//���ݵ�ǰclip��־��surfsrc�����ڿͻ����ߴ磬����surface������backbuffer����ʼ�������

inline void CalcClipped();				//���㵱ǰͼƬ(����Ŵ�zoom����δ��clip��surface)clip״̬
inline void Clear();					//�����ǰͼƬ
inline bool NoPic();					//�ж�ͼƬ��surface����
inline bool NoSurf();					//�ж�surface����
//��ȡ��Ϣ
inline void GetCurInfo();				//��ȡ��ǰ���λ�ö�ӦͼƬ������
inline void GetCurPos();				//��ȡ��ǰ���λ��(����)
inline bool GetCurColor();				//����GetCurPos�õ������أ���ȡ��Ӧ����ɫ

//
//D3D
//
inline bool InfoRender();				//��Ⱦ������Ϣ
inline bool LoadBackbuffer();			//����ǰsurface����backbuffer����Ⱦ֮ǰִ��
inline bool Render();					//��Ⱦ
bool ResetDevice();						//�����豸
void OnLostDevice();					//�豸��ʧ����
bool OnResetDevice();					//�豸���ö���

//
//����
//
string GetFMT(D3DFORMAT);				//���ͼƬ��ʽ�ַ���
inline int Widthof(const RECT &rect);	//��RECT���
inline int Heightof(const RECT &rect);	//��RECT�߶�
inline bool inside(const POINT &point, const RECT &rect);//�жϵ���RECT��