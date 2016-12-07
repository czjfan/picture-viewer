#pragma once

//windows include
#include "time.h"
#include "shellapi.h"
#include <Psapi.h>
#include <iostream>
#include <windows.h>
#include <Commdlg.h>
#include <io.h>

//local include
#include "resource.h"
#include "generalgeo.h"
#include "BMP.h"
#include "Pic.h"
#include "inputer.h"
#include "d3dwnd.h"

//local dx include
//#include "d3dgui.h"
//(using fast font render-CD3DFont)
#include "d3dfont.h"

//dx include
#include <d3d9.h>
#include <d3dx9tex.h>
//#include <DXUT.h>
//#include <DXUTgui.h>

//lib
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//#pragma comment(lib,"DXUT.lib")
//#pragma comment(lib,"DXUTOpt.lib")
//use to handle compile error 
///NODEFAULTLIB:"libcmt.lib" 

using std::cout;
using std::endl;

//�û���Ϣ
//#define WM_RESETDEVICE					(WM_USER + 1)
#define WM_SURFFORCERENEW				(WM_USER + 2)

//GUI ϵͳ
#define BUTTON_ID_1						1

//������
#define CMDRECT_ALPHA					120
#define CMDRECT_COLOR					D3DCOLOR_ARGB(CMDRECT_ALPHA, 25, 220, 255)
#define CMDRECT_COLOR2					D3DCOLOR_ARGB(CMDRECT_ALPHA, 23, 24, 36) 
#define CMDRECT_COLOR3					0xB0CFF5DB 
#define CMDRECT_COLOR_USING				CMDRECT_COLOR2

//������Ϣ
#define WINDOWPOSX_INIT					180
#define WINDOWPOSY_INIT					100
#define WINDOWWIDTH_INIT				800
#define WINDOWHEIGHT_INIT				520

//���ڿ���
#define SIZE_NEAR_PIXEL					5	//���ڵ�����С�����ӽ���Ե�ĳ̶�

//����״̬����
#define KEYSTATEMASK_DOWN				0x8000

//��λת��
#define B_TO_MB							1048576.0f

//֡��
#define ICONIC_SLEEP					50
#define NORMAL_FPS						66
#define NORMAL_FRAMETIME				(1000.0f/NORMAL_FPS)
#define STATIC_FPS_NORMAL				16		//��ֹ״̬����֡��
#define STATIC_FRAMETIME_NORMAL			(1000.0f/STATIC_FPS_NORMAL)

//��־
#define FLAGDELAY_ZOOM					100		//����΢���Ϊ��zoomЧ��
#define FLAGDELAY_SIZE					1		//wm_sizeʱ��������ѭ�����������size��־�����ֵ��ʾsize�������־������Ӧ����
#define FLAGDELAY_SURFREFRESH			160		//����΢���Ϊ���ܹ����������־����
#define FLAGDELAY_DRAG					100		//����΢���Ϊ��dragЧ��
#define FLAGDELAY_DRAGZOOM				100		//����΢���Ϊ��dragzoomЧ��

//����ģʽ
#define MODE_PIC						0
#define MODE_CMD						1

//����ģʽ
#define WINMODE_NORMALWINDOWS			0
#define WINMODE_ROUND					1
#define WINMODE_INIT					WINMODE_ROUND

//������ʾ
//���봰�ڱ�Ե����
#define TEXTMARGIN_SIDE					3
#define TEXTMARGIN_TOP					0
#define TEXTMARGIN_BOTTOM				2
#define TEXTMARGIN_ROW					2

//��ɫ
#define COLOR_BKG0						D3DCOLOR_ARGB(200, 238, 243, 249)
#define COLOR_BKG1						D3DCOLOR_ARGB(200, 255, 255, 255)
#define COLOR_BKGNIGHT					D3DCOLOR_ARGB(200, 18, 12, 20)
#define COLOR_BKG_INIT					COLOR_BKG0
#define COLOR_CMD						D3DCOLOR_ARGB(255, 20, 100, 255) 
#define COLOR_CMD2						D3DCOLOR_ARGB(240, 240, 200, 23)
#define COLOR_CMD_INIT					COLOR_CMD2
#define COLOR_TEXT0						D3DCOLOR_ARGB(160, 255, 255, 255)
#define COLOR_TEXT1						D3DCOLOR_ARGB(180, 100, 0, 255)

/*
 * ������Ϣ&��־
 */
HWND mainwnd;							//�����ھ��
RECT wndrect, clientrect;				//���������򣬿ͻ�����
POINT clientsize;						//�ͻ�����С
POINT cursor, lastpos;					//��ǰ������꣨ȫ�֣���lastpos�����϶�ʱ��¼��һλ�ã���lastcursor���ɻ���
int wbias, hbias;						//������ͻ�����Ȳ�
int wlimit, hlimit;						//���ڳ�����λ
RECT originwndrect;						//ȫ���ָ�
//������Ϣ
HANDLE hprocess;						//���̾��
float memoryin, memoryout;				//�ڴ�ռ����
//ѡ���־
UINT8 mode;								//ģʽ
UINT8 winmode;							//����ģʽ
D3DCOLOR backcolor;						//����ɫ

bool sizeEnable;						//�Ƿ�����ı䴰�ڴ�С
bool easymoveEnable;					//�Ƿ������Ҽ��϶�����
bool flagshow;							//�Ƿ���ʾ״̬��־��Ϣ		
bool infoshow;							//�Ƿ���ʾ������Ϣ
bool fpslimit;							//�Ƿ�����֡��
bool screencoloron;						//�Ƿ��ȡ��Ļ��ɫ
//״̬��־
//dragging, onzoom, draggingzoom����ȷ��֡�ʿ��Ʋ��ԣ���ȷ���ػ��¼�WM_SURFFORCERENEW��Ӧ
//WM_SIZE�¼�ʱ��������ѭ�������Բ���Ҫ�ж�onsize��ȷ��֡��
//bool LMBDown;							//�������Ƿ���
bool purewnd;
bool iswindowedfullscreen;
bool ondrag;							//�Ƿ�ʼ���϶�ͼ��״̬
bool dragging;							//�Ƿ����϶�ͼ��״̬��ȷ��֡�ʣ�
bool ondragzoom;						//�Ƿ�ʼ���϶��Ŵ�ͼ��״̬
bool draggingzoom;						//�Ƿ����϶��Ŵ�ͼ��״̬��ȷ��֡�ʣ�
bool onzoom;							//�Ƿ��ڷŴ�ȷ��֡�ʣ�
bool onsize;							//�Ƿ������Ŵ���
bool surfrefresh;						//surface�Ƿ���ˢ�£��������־��ʱֻ�����鿴��
bool needforcerenew;					//��Ҫǿ�Ƹ���surface��־�������ж��Ƿ���WM_SURFFORCERENEW��

short mousestate;						//��ǰ���λ�ã���������������ͣ��ޱ߿򴰿�ʹ�ã�
DWORD screencolor;						//��굱ǰλ�ö�Ӧ��Ļ����ɫ

/*
 * ��ʱ
 */
time_t loopcount;						//��ѭ������(PEEKMESSAGE����)
LARGE_INTEGER frequency, stime, etime;	//�߾��ȼ�ʱ
time_t nowtime, lasttime;				//�뼶��ʱ
float fps, avgfps, cvgfps;				//֡��
short staticfps; float staticframetime;
time_t zoomtick, sizetick\
	, surfrefreshtick, dragtick, dragzoomtick;	//��־��ʱ
ULONGLONG fpscount;						//����֡�ʴ��������ڼ�������֡��cvgfps��
float frametime;						//֡ʱ��

/*
 * D3D
 */
D3DWnd *pd3dwnd;						//D3D��װ��
LPDIRECT3DDEVICE9 maindevice;			//���豸 linked with "D3DWnd"
UINT *pbufferw, *pbufferh;				//�豸�������ߴ� linked with "D3DWnd's backbuffer"
//LPD3DXSPRITE  m_sprite;				//��֪����ô��!


/**************************************************************
һ��PicPack�洢һ��ͼƬ�ļ���������Ϣ
��BMP��mainbmp��Ϊ��ǰ��ԾͼƬ��һ������ԭ����PicPack�С��ǵ�ǰ������ͼƬ
���޸�Surfer���ڰ󶨲�ͬBMP���л���
***************************************************************/
Surfer surfer;							//ͼƬ��ʾ�� linked with "D3D Device" and BMP
/*
 * ͼƬ��
 */
//PicPack *piclist;
PicPack mainpicpack;					//��ͼƬ��
BMP *mainbmp;							//��ǰͼƬ linked with "PicPack"����������
WCHAR strFileName[MAX_PATH];			//�ļ���

/*
 * ͼƬ�����
 */
//_finddata_t filegroup;
//UINT32 picnum;
//UINT32 piccount;
//UINT32 piclistlen;
//PicPack *piclist;

/*
 * ��Ϣ��ʾ
 */
//������			@��װ����WndDev
//D3DXMATRIXA16 matView;				//�ӽǾ������������ӽǣ�����ʱ�������棩
//D3DXMATRIXA16 proj;					//ͶӰ���󣬣�����ʱ�������棩
D3DXVECTOR3 eye;						//�۾�λ�ã��۲�λ�ã�
D3DXVECTOR3 at;							//����Ŀ��
D3DXVECTOR3 up;							//��Ļ���ϵ���������
LPD3DXMESH cmdlight;					//������#1-��ά����
//LPDIRECT3DVERTEXBUFFER9 cmdlightrhw;	//������#2-��ά�������ͻ�������,ȱ�������¶�λʱ��Ҫ�ؽ�ģ�ͣ����ǹ̶�������λ�ã�
Inputer_W inputer0;						//������������

//������Ϣ		@��װ����WndDev
LPD3DXFONT font;						//��ʾ����
LPD3DXFONT font2;
LPD3DXFONT fontcmd;
//LPCD3DFont d3dfont1;					//���ٵ�Ч��һ������������
RECT textrect;							//��ʾ��������
RECT textrect0;							//������Ϣ����
RECT textrect2;							//״̬��Ϣ����
RECT cmdrect;							//��������ʾ����
WCHAR picinfostr[256];					//��ʾ��Ϣ���棬���洢������Ϣ-�����٣����ʶࣩ

/*
 * ����
 */
//inline void WinSizeProc();			//WM_SIZE��Ϣ������
//inline void MouseMoveProc();			//WM_MOUSEMOVE��Ϣ������
void ClickDragWindow_Custom(int cursorx, int cursory);
void KeyDownProc(WPARAM wparam);		//WM_KEYDOWN��Ϣ������
inline void SetWindowMode(UINT8 wmode);	//����Բ�Ǿ��δ���
inline void MaintainWindowStyle();
inline void OnWinChange();				//��ô��ںͿͻ�����С
inline void RefreshTextRect();			//����������ʾ����
void FitWnd(int bmpwith, int bmpheight);//���ñ�׼���ڳߴ�
bool FullScreen_Windowed(bool tofull);
bool PureWindow(bool topure);
//���
inline short GetSizeType(CPoint);		//���ݵ�ǰ���꣬���������ͣ��ı䴰�ڴ�С��
void CustomWinSizeProc();				//WM_LBUTTONDOWN��Ϣ���Զ������촰�ڵĴ�����
inline void SetCursor_Custom();				//������������С�����������ʽ
//��ʼ��
bool Init();							//��ʼ��
//D3D��ʼ��
bool D3DInit();							//D3D��ʼ��
//�ļ�
void OpenFileWin();						//���ļ�����
void SaveFileWin();
bool OnSaveFile(WCHAR file[]);
bool OnLoadFile(WCHAR file[]);	//����ͼƬ
bool OnWininitFile(LPWSTR file);//��ʼ��ʱ����ͼƬ
//��־
inline void DelayFlag();		//�ӳٱ�־������
void ClearFlag();				//�����Ծ��־
//����
void BeginDragPic();			//��ʼͼƬ�϶�
void EndDragPic();				//����ͼƬ�϶�
void BeginDragZoomPic();		//��ʼͼƬ�϶�
void EndDragZoomPic();			//����ͼƬ�϶�
//����ģʽ
inline void EnterCMDMode();		//����������ģʽ
inline void ExitCMDMode();		//�˳�������ģʽ���ص�����ģʽ��

/*
 * ͼ��
 */
//mainbmp
void MYCALL1 Clear();					//�����ǰͼƬ
inline bool MYCALL1 NoPic();			//�ж�ͼƬ��surface����

/*
 * D3D
 */
inline void MYCALL1 SetRenderState();	//������Ⱦ״̬
inline void MYCALL1 SetView();			//�����ӽǣ�����ά����Ļ��ƣ�
inline bool MYCALL1 InfoRender();		//��Ⱦ������Ϣ
inline void MYCALL1 Render();			//��Ⱦ
inline bool MYCALL1 ResetDevice();		//�����豸
inline void MYCALL1 OnLostDevice();		//�豸��ʧ����
inline bool MYCALL1 OnResetDevice(int clientw, int clienth);	//�豸���ö���
inline bool MYCALL1 OnResetDevice();	//�豸���ö���

//GUI ϵͳ
//CD3DGUISystem *g_gui;
//int fontID1;

//�ص�����
void CALLBACK CMDProc(WCHAR *wstr);		//�����лص�
//void CALLBACK GUICallback(int id, int state);
//void CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);//��ʱ����������cpuռ����!
