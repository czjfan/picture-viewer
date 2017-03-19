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
#include "Pic.h"

// local dx include
//(using fast font render-CD3DFont)
#include "d3dfont.h"
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
#define WM_RESETDEVICE					(WM_USER + 1)
#define WM_SURFFORCERENEW				(WM_USER + 2)
#define WM_TOGGLEFULLSCREEN				(WM_USER + 3)
#define WM_MOUSEWHEELEND				(WM_USER + 4)

// ������
#define CMDRECT_ALPHA					180
#define CMDRECT_COLOR					D3DCOLOR_ARGB(CMDRECT_ALPHA, 25, 220, 255)
#define CMDRECT_COLOR2					D3DCOLOR_ARGB(CMDRECT_ALPHA, 23, 24, 36) 
#define CMDRECT_COLOR3					0xB0CFF5DB 
#define CMDRECT_COLOR_USING				CMDRECT_COLOR

// ���ڳߴ�
#define WINDOWPOSX_INIT					180
#define WINDOWPOSY_INIT					100
#define WINDOWWIDTH_INIT				800
#define WINDOWHEIGHT_INIT				520

// ���ڿ���
#define SIZE_NEAR_PIXEL					5	//���ڵ�����С�����ӽ���Ե�ĳ̶�

// ��λת��
#define B_TO_MB							1048576.0f

// ֡��
#define ICONIC_SLEEP					50
#define NORMAL_FPS						60
#define NORMAL_FRAMETIME				(1000.0f/NORMAL_FPS)
#define STATIC_FPS_NORMAL				NORMAL_FPS		//��ֹ״̬����֡��
#define STATIC_FPS_SILENT				16
#define STATIC_FPS_SLEEP				0

// ��־
#define FLAGDELAY_ZOOM					500		//����΢���Ϊ��zoomЧ��
#define FLAGDELAY_SIZE					1		//wm_sizeʱ��������ѭ�����������size��־�����ֵ��ʾsize�������־������Ӧ����
#define FLAGDELAY_SURFREFRESH			160		//����΢���Ϊ���ܹ����������־����
#define FLAGDELAY_DRAG					100		//����΢���Ϊ��dragЧ��
#define FLAGDELAY_DRAGZOOM				100		//����΢���Ϊ��dragzoomЧ��

// ����ģʽ
#define MODE_PIC						0
#define MODE_CMD						1
//#define MODE_INTRO						2
//#define MODE_PICVIEWER					3
//#define MODE_VIDEO						4

// ����ģʽ
#define WINMODE_NORMALWINDOWS			0
#define WINMODE_ROUND					1
#define WINMODE_INIT					WINMODE_ROUND

// ������ʾ
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
#define COLORBLOCK_X					160
#define COLORBLOCK_Y					-1
#define COLORBLOCK_RADIUS				16

// GUI ϵͳ
#define BUTTON_ID_1						1
#define BUTTON_ID_2						2
#define BUTTON_ID_3						3
#define BUTTON_ID_4						4
#define BUTTON_ID_5						5
#define BUTTON_ID_6						6
#define STATIC_ID_1						11
#define STATIC_ID_2						12
#define INPUT_IN_1						21


/*
 * ������Ϣ&��־
 */
// ������Ϣ
HWND mainwnd;							// �����ھ��
RECT wndrect, clientrect;				// ���������򣬿ͻ�����
POINT clientsize;						// �ͻ�����С
POINT cursor, lastcursor;				// ��ǰ������꣨ȫ�֣���lastcursor�����϶�ʱ��¼��һλ�ã���lastcursor���ɻ���
int wbias, hbias;						// ������ͻ�����Ȳ�
int wlimit, hlimit;						// ���ڳ�����λ
RECT originwndrect;						// ȫ���ָ�
// ������Ϣ
HANDLE hprocess;						// ���̾��
float memoryin, memoryout;				// �ڴ�ռ����
DWORD screencolor;						// ��굱ǰλ�ö�Ӧ��Ļ����ɫ
// ѡ���־
UINT8 mode;								// ģʽ
UINT8 winmode;							// ����ģʽ
bool colorblockon;						// ��ʾ��ǰ��ɫ��
D3DCOLOR backcolor;						// ����ɫ
bool sizeEnable;						// �Ƿ�����ı䴰�ڴ�С
bool easymoveEnable;					// �Ƿ������Ҽ��϶�����
bool flagshow;							// �Ƿ���ʾ״̬��־��Ϣ		
bool infoshow;							// �Ƿ���ʾ������Ϣ
bool debuginfoshow;
bool fpslimit;							// �Ƿ�����֡��
bool screencoloron;						// �Ƿ��ȡ��Ļ��ɫ
// ״̬��־
bool LMBdown;							// �������Ƿ���
bool purewnd;							// �Ƿ��Ǵ�����״̬���ޱ�������
bool iswindowedfullscreen;				// �Ƿ���αȫ��״̬
bool isiconic;							// �Ƿ�����С��
bool ondrag;							// �Ƿ�ʼ���϶�ͼ��״̬
bool dragging;							// �Ƿ����϶�ͼ��״̬��ȷ��֡�ʣ�
bool ondragzoom;						// �Ƿ�ʼ���϶��Ŵ�ͼ��״̬
bool draggingzoom;						// �Ƿ����϶��Ŵ�ͼ��״̬��ȷ��֡�ʣ�
bool onzoom;							// �Ƿ��ڷŴ�ȷ��֡�ʣ�
bool onsize;							// �Ƿ������Ŵ���
bool surfrenew;							// surface�Ƿ���ˢ�£��������־��ʱֻ�����鿴��
bool needforcerenew;					// ��Ҫǿ�Ƹ���surface��־�������ж��Ƿ���WM_SURFFORCERENEW��

/*
 * ��ʱ
 */
time_t loopcount, lastloopcount;		// ��ѭ������
float fps, avgfps, cvgfps;				// ֡��
ULONGLONG fpscount;						// ����֡�ʴ��������ڼ�������֡��cvgfps��
float frametime;						// ֡ʱ��
short staticfps; float staticframetime;	// ֡������
float normalfps; float normalframetime;
time_t zoomtick, sizetick\
	, surfrenewtick, dragtick\
	, dragzoomtick, decoratetick;		// ��־��ʱ

/*
 * D3D
 */
D3DWnd *pd3dwnd;						// D3D��װ��
LPDIRECT3DDEVICE9 maindevice;			// ���豸 linked with "D3DWnd"
UINT *pbufferw, *pbufferh;				// �豸�������ߴ� linked with "D3DWnd's backbuffer"
//LPD3DXSPRITE  m_sprite;				// ��֪����ô��!

LPD3DXMESH backdecorate;
D3DLIGHT9 light;
D3DMATERIAL9 material;
D3DXMATRIX basemat;


Surfer surfer;							// ͼƬ��ʾ�� linked with "D3D Device" and BMP
/*
 * ͼƬ��
 */
//_finddata_t filegroup;
bool haspic;
PicPackList piclist;					// ͼƬ�б�
PicPack *mainpicpack;					// ��ͼƬ��
BMP *mainbmp;							// ��ǰͼƬ linked with "PicPack"����������

/*
 * ��Ϣ��ʾ
 */

// ��ɫ��
LPDIRECT3DVERTEXBUFFER9 colorblock;
LPDIRECT3DVERTEXBUFFER9 colorblockback;

// ������Ϣ		@��װ����WndDev
LPD3DXFONT font;						// ��ʾ����
LPD3DXFONT font2;
LPD3DXFONT fontcmd;
LPD3DXFONT fontpic;
//LPCD3DFont d3dfont1;					// ���ٵ�Ч��һ������������
RECT textrect0;							// ͼƬ��Ϣ����
RECT textrect1;							// surface��Ϣ����
RECT textrect2;							// ״̬��Ϣ����
RECT cmdrect;							// ��������ʾ����
RECT picrect;							// ͼƬ״̬��ʾ����
WCHAR picinfostr[256];					// ��ʾ��Ϣ���棬���洢������Ϣ-�����٣����ʶࣩ

/*
 * ����
 */
void ClickDragWindow_Custom(int cursorx, int cursory);// �Զ������촰��
void KeyDownProc(WPARAM wParam);		// WM_KEYDOWN��Ϣ������
void SetWindowMode(UINT8 wmode);		// ���ô���ģʽ
inline void MaintainWindowStyle();		// ά�ִ���ģʽ���������δ��ڣ�
inline void OnWinChange();				// ��ô��ںͿͻ�����С
inline void RefreshTextRect();			// ����������ʾ����
void FitWnd(int bmpwidth, int bmpheight);// ���ñ�׼���ڳߴ�
bool FullScreen_Windowed(bool tofull);
bool PureWindow(bool topure);
void SetStaticFps(float sfps);
// ���
inline short GetSizeType(CPoint point);	// ���ݵ�ǰ���꣬���������ͣ��ı䴰�ڴ�С��
inline void SetCursor_Custom();			// ������������С�����������ʽ
// ��ʼ��
HFONT hf;
bool Init();							// ��ʼ��
// D3D��ʼ��
bool D3DInit();							// D3D��ʼ��
// �ļ�
void OnSave();
void OnSaveAs();
bool OnWininitFile(LPWSTR cmdline);		// ��ʼ��ʱ����ͼƬ
void OpenFileWin();						// ���ļ�����
void SaveFileWin(WCHAR file[] = L"");	// �����ļ�����

bool OnSaveFile(WCHAR file[]);			// ����ͼƬ
bool OnLoadFile(WCHAR file[]);			// ����ͼƬ

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
 * ͼ��
 */
inline bool HasPic()					// ���㵱ǰ�Ƿ�ͼƬ�ǿ�
{
	if (mainbmp)
		if (mainbmp->isNotEmpty())
			return true;
	return false;
}
void InitNonPic();						// ��ʼ����ͼƬ״̬�Ĳ���
bool SetPic(short picidx = 1);			// ����ָ���������ͼƬ
bool SetTailPic();						// ��λ�����һ��ͼƬ
bool SetPrevPic();						// ��λ����һͼƬ
bool SetNextPic();						// ��λ����һͼƬ
void Drop();							// �����ǰͼƬ
void SetSurface(PicPack *oldpinpack);	// ����ͼƬ֮������surface
void SetPicInfo();						// ���õ�ǰͼƬ��Ϣ������mainpicpack

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
inline bool OnResetDevice2(int clientw, int clienth);	// �豸���ö���
inline bool OnResetDevice();			// �豸���ö���
inline void PostResetDevice();			// �豸���ú���

// GUI ϵͳ
CD3DGUISystem *g_gui;
int fontID1;

// �ص�����
void CALLBACK CMDProc(WCHAR *wstr);		// �����лص�
void CALLBACK GUICallback(int ID, WPARAM wp, LPARAM lp = NULL);// GUI�ص�
//MMRESULT TimerID_main;
//void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);//��ʱ������