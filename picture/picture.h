#pragma once

// windows include
#include "time.h"
#include "shellapi.h"
#include <Psapi.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <Commdlg.h>
#include <io.h>

// local include
#include "resource.h"
#include "generalgeo.h"
#include "BMP.h"
#include "picfile.h"

// local dx include
#include "d3dfont.h"//(use fast pFontPic render-CD3DFont)
#include "d3dwnd.h"
#include "gui.h"

// dx include
#include <d3d9.h>
#include <d3dx9tex.h>
//#include <DXUT.h>
//#include <DXUTgui.h>

// lib
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//#pragma comment(lib,"DXUT.lib")
//#pragma comment(lib,"DXUTOpt.lib")
//use to handle compile error 
///NODEFAULTLIB:"libcmt.lib" 

using std::cout;
using std::endl;

// �û���Ϣ
#define WM_RESETDEVICE					(WM_USER + 1)	// ����D3D�豸��Ϣ
#define WM_SURFFORCERENEW				(WM_USER + 2)	// surface������Ϣ
#define WM_TOGGLEFULLSCREEN				(WM_USER + 3)	// �л�ȫ����Ϣ
#define WM_MOUSEWHEELEND				(WM_USER + 4)	// FLAGDELAY_WHEEL�趨�ӳ�ʱ�䣬���ֽ����󾭹����ʱ��󣬷���һ�������ֽ������¼�
#define WM_SIZEEND						(WM_USER + 5)	// FLAGDELAY_SIZE

// ������
#define CMDRECT_ALPHA					180
#define CMDRECT_COLOR					D3DCOLOR_ARGB(CMDRECT_ALPHA, 25, 220, 255)
#define CMDRECT_COLOR2					D3DCOLOR_ARGB(CMDRECT_ALPHA, 23, 24, 36) 
#define CMDRECT_COLOR3					0xB0CFF5DB 
#define CMDRECT_COLOR_USING				CMDRECT_COLOR

// ���ڳߴ�
#define WINDOWPOSX_INIT					CW_USEDEFAULT //Ĭ��
#define WINDOWPOSY_INIT					100
#define WINDOWWIDTH_INIT				656
#define WINDOWHEIGHT_INIT				518

// ���ڿ���
#define SIZE_NEAR_PIXEL					5	//���ڵ�����С�����ӽ���Ե�ĳ̶�

// ��λת��
#define B_TO_MB							1048576.0f

// ʱ��
#define ICONIC_SLEEP					50
#define NORMAL_FPS						60
#define NORMAL_FRAMETIME				(1000.0f/NORMAL_FPS)
#define STATIC_FPS_NORMAL				NORMAL_FPS		//��ֹ״̬����֡��
#define STATIC_FPS_SILENT				16
#define STATIC_FPS_SLEEP				0

// ��־
#define FLAGDELAY_WHEEL					400		//����΢���Ϊ��zoomЧ��
#define FLAGDELAY_SIZE					1		//wm_sizeʱ��������ѭ�����������size��־�����ֵ��ʾsize�������־������Ӧ����
#define FLAGDELAY_SURFREFRESH			160		//����΢���Ϊ���ܹ����������־����
#define FLAGDELAY_DRAG					100		//����΢���Ϊ��dragЧ��
#define FLAGDELAY_DRAGZOOM				100		//����΢���Ϊ��dragzoomЧ��

// ����ģʽ
#define MODE_PIC						0
#define MODE_CMD						1		//TODO������Ϊģʽ����Ϊ״̬
//#define MODE_PICVIEWER					2	//ͼƬ���ģʽ
//#define MODE_VIDEO						3	//��Ƶ����ģʽ

// ����ģʽ
#define WINMODE_NORMALWINDOWS			0
#define WINMODE_ROUND					1
#define WINMODE_INIT					WINMODE_NORMALWINDOWS

// �ı���ʾ
// ���봰�ڱ�Ե����
#define TEXTMARGIN_SIDE					3
#define TEXTMARGIN_TOP					0
#define TEXTMARGIN_BOTTOM				2
#define TEXTMARGIN_ROW					2
// ��ɫ
#define COLOR_BKG0						D3DCOLOR_ARGB(200, 238, 243, 249)
#define COLOR_BKG1						D3DCOLOR_ARGB(200, 255, 255, 255)
#define COLOR_BKGNIGHT					D3DCOLOR_ARGB(200, 18, 12, 20)
#define COLOR_BKG_INIT					COLOR_BKG0
#define COLOR_CMD						D3DCOLOR_ARGB(255, 20, 100, 255) 
#define COLOR_CMD2						D3DCOLOR_ARGB(240, 240, 200, 23)
#define COLOR_CMD3						D3DCOLOR_ARGB(250, 230, 40, 60)
#define COLOR_CMD_INIT					COLOR_CMD3
#define COLOR_TEXT0						D3DCOLOR_ARGB(250, 255, 255, 255)
#define COLOR_TEXT1						D3DCOLOR_ARGB(220, 110, 0, 255)
#define COLOR_TEXT2						D3DCOLOR_ARGB(220, 250, 200, 10)
#define COLOR_TEXT3						D3DCOLOR_ARGB(230, 120, 122, 122)
#define COLOR_TEXT4						D3DCOLOR_ARGB(230, 200, 200, 160)

#define COLOR_BLOCKALPHA				255
#define COLOR_BLOCKBACK					COLOR_BKG0

// D3D
//ɫ�����
#define COLORBLOCK_X					160
#define COLORBLOCK_Y					-1
#define COLORBLOCK_RADIUS				16

// GUI ϵͳ
#define BUTTON_ID_1						1
#define BUTTON_ID_2						2
#define BUTTON_ID_3						3
#define INPUT_IN_1						21


/*
 * ������Ϣ&��־
 */
// ����״̬
HWND hWndMain;							// �����ھ��
RECT rcWnd, rcClient;					// ���������򣬿ͻ�����
POINT cursor, lastCursor;				// ��ǰ������꣨ȫ�֣���lastCursor�����϶�ʱ��¼��һ���λ�á�
int WinDiffW, WinDiffH;					// ������ͻ����߿����أ�
int WinStdW, WinStdH;					// ���ڱ�׼�ߴ磨���أ�
RECT rcOriginalWnd;						// ����ȫ���ָ�����¼ȫ��֮ǰ��������

// ������Ϣ
HANDLE hProcess;						// ���̾��
float memoryIn, memoryOut;				// �ڴ�ռ����
DWORD screenColor;						// ��굱ǰλ�ö�Ӧ��Ļ����ɫ

// ѡ���־
UINT8 mode;								// ģʽ
UINT8 winMode;							// ����ģʽ
D3DCOLOR BackgroundColor;				// ����ɫ
bool bSizeEnable;						// �Ƿ�����ı䴰�ڴ�С�����δ���ģʽʱ��Ч��
bool bFlagsShow;						// �Ƿ���ʾ״̬��־��Ϣ		
bool bInfoShow;							// �Ƿ���ʾ������Ϣ
bool bFpsLimited;						// �Ƿ�����֡��

bool bColorblockOn;						// ��ʾ��ǰ��ɫ��
bool bScreenColorOn;					// �Ƿ��ȡ��Ļ��ɫ

// ״̬��־
bool bLMBDown;							// �������Ƿ���
bool bPureWnd;							// �Ƿ��Ǵ�����״̬���ޱ�������
bool bWindowedFullscreen;				// �Ƿ���αȫ��״̬
bool bIconic;							// �Ƿ�����С��
bool bOnZoom;							// �Ƿ��ڷŴ�ȷ��֡�ʣ�
bool bOnSize;							// �Ƿ������Ŵ���
bool bOnDrag;							// �Ƿ�ʼ���϶�ͼƬ״̬
bool bDragging;							// �Ƿ����϶�ͼƬ״̬��ȷ��֡�ʣ�
bool bOnDragzoom;						// �Ƿ�ʼ���϶��Ŵ�ͼƬ״̬
bool bDragzooming;						// �Ƿ����϶��Ŵ�ͼƬ״̬��ȷ��֡�ʣ�
bool bSurfRenew;						// surface�Ƿ���ˢ�£��������־��ʱֻ�����鿴��
bool bNeedForceRenew;					// ��Ҫǿ�Ƹ���surface��־�������ж��Ƿ���WM_SURFFORCERENEW��

// ���ñ�־ :
#define SF_SR(RETVAL)	{ bSurfRenew = RETVAL;if(bSurfRenew) surfRenewTick = GetTickCount(); }//bSurfRenew
#define SF_SRFR(RETVAL) { bSurfRenew = RETVAL;if(bSurfRenew) surfRenewTick = GetTickCount(); bNeedForceRenew |= bSurfRenew;}//bSurfRenew&bNeedForceRenew
#define SF_DR()			{ bDragging = true;dragTick = GetTickCount(); }//bDragging
#define SF_OZ()			{ bOnZoom = true;zoomTick = GetTickCount(); }//bOnZoom

/*
 * ��ʱ
 */
LONGLONG nLoops;			// ��ѭ������
float fps, avgFps, cvgFps;				// ֡��
ULONGLONG fpsCount;						// ����֡�ʴ��������ڼ�������֡��cvgFps��
float frameTime;						// ֡ʱ��
float procTime;							// ����ʱ��
short staticFps; float staticFrameTime;	// ֡������
float normalFps; float normalFrameTime;
time_t zoomTick, sizeTick\
	, surfRenewTick, dragTick\
	, dragzoomTick, decorateTick;		// ��־��ʱ

/*
 * D3D
 */
D3DWnd *pD3DWnd;						// D3D��װ��
LPDIRECT3DDEVICE9 mainDevice;			// ���豸 linked with "D3DWnd"
UINT *pBufferW, *pBufferH;				// �豸�������ߴ� linked with "D3DWnd's backbuffer"
//LPD3DXSPRITE  m_sprite;				// ��֪����ô��!

LPD3DXMESH decorate;
D3DLIGHT9 light;
D3DMATERIAL9 material;
D3DXMATRIX matWorld;


Surfer surfer;							// ͼƬ��ʾ�� linked with "D3D Device" and BMP
/*
 * ͼƬ��
 */
//_finddata_t filegroup;
bool bPicOn;
PicPackList picList;					// ͼƬ�б�
PicPack *pLivePicpack;					// ��ͼƬ��
BMP *pLiveBmp;							// ��ǰͼƬ linked with "PicPack"����������

/*
 * ��Ϣ��ʾ
 */

// ��ɫ��
//LPDIRECT3DVERTEXBUFFER9 colorblock;
//LPDIRECT3DVERTEXBUFFER9 colorblockback;

// ������Ϣ		@��װ����WndDev
LPD3DXFONT pFontPic;					// ��ʾ����
LPD3DXFONT pFontFlags;
LPD3DXFONT pFontPicState;
//LPCD3DFont d3dfont1;					// ���ٵ�Ч��һ������������
RECT rcPic;								// ͼƬ��Ϣ����
RECT rcSurface;							// surface��Ϣ����
RECT rcFlag;							// ״̬��Ϣ����
RECT rcPicState;						// ͼƬ״̬��ʾ����
WCHAR picInfoStr[256];					// ��ʾ��Ϣ���棬���洢������Ϣ:�����٣����ʶ࣬����ʹ�û��棩

/*
 * ����
 */
void BeginDragWindow_Inner(int cursorx, int cursory);// �Զ������촰��
void BeginDragWindow_Custom(int cursorx, int cursory);// �Զ������촰��
void KeyDownProc(WPARAM wParam);		// WM_KEYDOWN��Ϣ������
void SetWindowMode(UINT8 wmode);		// ���ô���ģʽ
inline void MaintainWindowStyle();		// ά�ִ���ģʽ���������δ��ڣ�
inline void OnWinChange();				// ��ô��ںͿͻ�����С
inline void RefreshTextRect();			// ����������ʾ����
bool FullScreen_Windowed(bool tofull, bool restore = true);
bool PureWindow(bool topure);
void SetStaticFps(float sfps);
void ToggleNight();

// ���
inline short GetSizeType(CPoint point);	// ���ݵ�ǰ���꣬���������ͣ��ı䴰�ڴ�С��
inline void SetCursor_Custom();			// ������������С�����������ʽ
// ��ʼ��
bool Init();							// ��ʼ��
// D3D��ʼ��
bool D3DInit();							// D3D��ʼ��
// �ļ�
void OnSave();
void OnSaveAs();
bool OnWininitFile(LPWSTR cmdline);		// ��ʼ��ʱ����ͼƬ
void OpenFileWin();						// ���ļ�����
void SaveFileWin(WCHAR file[] = L"");	// �����ļ�����

bool SaveFile(WCHAR file[]);			// ����ͼƬ
bool LoadFile(WCHAR file[]);			// ����ͼƬ

// ��־
inline void DelayFlag();				// �ӳٱ�־������
void ClearFlag();						// �����Ծ��־
// ״̬�л�
inline void BeginDragPic();				// ��ʼͼƬ�϶�
inline void EndDragPic();				// ����ͼƬ�϶�
inline void BeginDragZoomPic();			// ��ʼͼƬ�϶��Ŵ�
inline void EndDragZoomPic();			// ����ͼƬ�϶��Ŵ�
// ģʽ
inline void EnterCMDMode();				// ����������ģʽ
inline void ExitCMDMode();				// �˳�������ģʽ���ص�����ģʽ��

/*
 * ͼƬ��ͼƬ�б�PicPackListһϵ�в������ʹ���surface��PicPack�İ󶨣�
 */
void InitNonPic();						// ��ʼ����ͼƬ״̬�Ĳ���
bool SetPic(short picidx = 1);			// ����ָ���������ͼƬ
bool SetNewPic();						// ��λ�����һ��ͼ��������
bool SetTailPic();						// ��λ�����һ��ͼƬ
bool SetPrevPic();						// ��λ����һͼƬ
bool SetNextPic();						// ��λ����һͼƬ
void Drop();							// �����ǰͼƬ

inline bool HasPic()					// ���㵱ǰ�Ƿ�ͼƬ�ǿ�
{
	if (pLiveBmp)
		if (pLiveBmp->isNotEmpty())
			return true;
	return false;
}
void SetSurface(PicPack *newpicpack, PicPack *oldpicpack, bool renew = true);	// ����ͼƬ֮������surface
void SetPicInfo();						// ���õ�ǰͼƬ��Ϣ������pLivePicpack

//ͼƬ��surface�������ɣ�
void PicMove(int dx, int dy);
void PicRestore();
void PicClipWindow();
void PicFitWnd();// ���ñ�׼���ڳߴ�
void PicDock();
void PicCenter();
void PicFit();

/*
 * D3D
 */
inline void SetRenderState();			// ������Ⱦ״̬
inline void SetView();
inline void SetLight();
inline bool InfoRender();				// ��Ⱦ������Ϣ
inline void Render();					// ��Ⱦ
// �豸����
inline bool ResetDevice();				// �����豸
inline void OnLostDevice();				// �豸��ʧ����
inline bool OnResetDevice();			// �豸���ö���
inline bool OnResetDevice2(int clientw, int clienth);	// �豸���ö���
inline void PostResetDevice();			// �豸���ú���

// GUI ϵͳ
CD3DGUISystem *g_gui;
int fontID1;

// �ص�����
void CALLBACK CMDProc(WCHAR *wstr);		// �����лص�
void CALLBACK GUICallback(int ID, WPARAM wp, LPARAM lp = NULL);// GUI�ص�
//MMRESULT TimerID_main;
//void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);//��ʱ������