#pragma once

#include "d3dwnd.h"

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// ����״̬
#ifndef KEYSTATEMASK_DOWN
#define KEYSTATEMASK_DOWN				0x8000
#endif
#ifndef ISKEYDOWN
#define ISKEYDOWN(KEY)					((GetAsyncKeyState(KEY) & KEYSTATEMASK_DOWN) != 0)
#endif
#define ISSHIFTDOWN						ISKEYDOWN(VK_SHIFT)
#define CAPSLOCK_ON						((bool)(GetKeyState(VK_CAPITAL) & 0x01))
#define CAPS_STATUS_ON					(CAPSLOCK_ON ^ ISSHIFTDOWN)

#define ISCHAR(C)						(((C) <= 'Z' && (C) >= 'A') || (C) <= ('z' && (C) >= 'a'))
#define ISUPPERCHAR(C)					((C) <= 'Z' && (C) >= 'A')
#define ISLOWERCHAR(C)					((C) <= 'z' && (C) >= 'a')
#define TOUPPERCHAR_UNSAFE(C)			(C) += 'A' - 'a';
#define TOLOWERCHAR_UNSAFE(C)			(C) += 'a' - 'A';

// ����
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)					{ if (p) { (p)->Release(); (p) = NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)					{ if (p) { delete (p); (p) = NULL; } }
#endif
#ifndef SAFE_DELETE_LIST
#define SAFE_DELETE_LIST(p)				{ if (p) { delete[] (p); (p) = NULL; } }
#endif
#ifndef SAFE_RELEASEDELETE
#define SAFE_RELEASEDELETE(p)			{ if (p) { (p)->Release(); delete (p); (p) = NULL; } }
#endif
#ifndef NULL_RETURN_FALSE
#define NULL_RETURN_FALSE(p)			{ if (!p) return false; }
#endif
#ifndef FALSE_RETURN_FALSE
#define FALSE_RETURN_FALSE(p)			NULL_RETURN_FALSE(p)
#endif
#ifndef FAILED_RETURN_FALSE
#define FAILED_RETURN_FALSE(p)			{ if (FAILED(p)) return false; }
#endif
#ifndef SAFE_POINTERVALUE_0
#define SAFE_POINTERVALUE_0(p)			( (p)? *p : 0 )
#endif

// ��ɫ
#define SETALPHA(C, A)					((C & 0xFFFFFF) | (A << 24))
#define COLOR_WHITE						0xFFFFFFFF

//
#define TEXTFORMAT_DEFAULT				( DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE )
#define TEXTFORMAT_LEFT					( DT_LEFT | DT_VCENTER | DT_NOCLIP )

// �ؼ����� 
#define GUI_CONTROL_NULL				0
#define GUI_CONTROL_STATIC				1     // ��̬
#define GUI_CONTROL_BUTTON				2     // ��ť
#define GUI_CONTROL_EDIT				3     // �ı���
#define GUI_CONTROL_BACKDROP			4     // ����ͼ

// �ؼ�״̬
#define GUI_STATE_OUT					1
#define GUI_STATE_OVER					2
#define GUI_STATE_DOWN					3
//#define GUI_STATE_OUTDOWN				4

// �ؼ�ͣ��
#define GUI_WINDOCK_NORMAL				0
#define GUI_WINDOCK_RIGHT				1
#define GUI_WINDOCK_BOTTOM				2
#define GUI_WINDOCK_BOTTOMRIGHT			3
#define GUI_WINDOCK_BOTTOMHSPAN			4	// ��細��
#define GUI_WINDOCK_FULLSCREEN			5	// ����
#define GUI_WINDOCK_SCALE				6	// ��Դ��ڱ���

// �¼���������ˣ�
// GUIϵͳ����ؼ����¼���GUIϵͳͳһ�����¼������ݸ��ؼ���
#define GUI_MOUSEEVENT_MOVE				0b000
#define GUI_MOUSEEVENT_UP				0b001
#define GUI_MOUSEEVENT_DOWN				0b010
#define GUI_MOUSEEVENT_DOWNMOVE			0b011

#define GUI_KEYEVENT_KEYDOWN			0b00
#define GUI_KEYEVENT_IMECHAR			0b01

// �ؼ��¼����û��ˣ�
#define GUI_EVENT_NULL					0

#define GUI_EVENT_DOWN					1	
#define GUI_EVENT_UP					2	
#define GUI_EVENT_ENTER					3
#define GUI_EVENT_LEAVE					4
#define GUI_EVENT_OUTUP					5

#define GUI_EVENT_CMD					11

// ����
#define GUI_DEFAULT_STATICWIDTH			120
#define GUI_DEFAULT_STATICHEIGHT		30
#define GUI_DEFAULT_BUTTONWIDTH			80
#define GUI_DEFAULT_BUTTONHEIGHT		30
#define GUI_DEFAULT_EDITWIDTH			80
#define GUI_DEFAULT_EDITHEIGHT			30
#define GUI_DEFAULT_BACKDROPWIDTH		120
#define GUI_DEFAULT_BACKDROPHEIGHT		60

#define GUI_DEFAULTTEXTLEN				63


void ErrorShow(HRESULT hr, WCHAR *msg = NULL, HWND hwnd = NULL, WCHAR *title = L"ERROR");

void ChangeCtrlAlpha(LPDIRECT3DVERTEXBUFFER9 pbuf, byte alpha);
bool mywcscpy(WCHAR **dest, WCHAR *src);

class CGUIControl
{
public:
	byte type;							// �ؼ�����
	int ID;								// �ؼ�ID
	//int groupID;						// �ؼ����飬0��Ĭ����ʾ

	DWORD color;						// ��ɫ

	byte dockmode;						// ͣ��ģʽ
	float dockX, dockY, dockW, dockH;	// ͣ�����꣨��ԣ�
	INT8 displaydx, displaydy;			// ��ʾƫ��
	float width, height;				// ��ȡ��߶�
	float posX, posY;					// �ؼ�λ�ã����Ͻǵ����꣩

	int inputpos;						// �����ַ�λ��
	WCHAR *text;						// �ı�����
	DWORD textcolor;					// �ı���ɫ
	LPD3DXFONT font;					// �ı�����
	RECT textrect;						// �ı���ʾ����

	bool bDisabled;						// �ؼ�ʹ��
	bool bVisible;						// �ؼ��ɼ���

	byte state;							// ״̬

	// ��ʾЧ��effect������Ч����
	byte displayevent;					// ��Ҫ��Чʱ���¼�
	LARGE_INTEGER lasttick;				// ��ʱ
	byte alpha;							// ��Чalphaֵ
	INT8 overdx, overdy;				// ��ʾλ�ƣ�����¼���
	INT8 downdx, downdy;

	static LARGE_INTEGER frequency;
public:
	CGUIControl();
	virtual ~CGUIControl();
	virtual bool Release();

	virtual void KillFocus();// not stable
	virtual void SetEnabled(bool bEnabled);
	virtual void SetVisible(bool bVisible);

	virtual void Translation(int dx, int dy);// not stable
	virtual void Dock(UINT *pbufw, UINT *pbufh);// stable
	virtual bool HandleInput(WPARAM wParam, bool IME = false);// stable
	virtual byte HandleMouse(byte lmbstate, LONG mouseX, LONG mouseY);// stable

	virtual bool Render()// not stable
	{
		return false;
	};
};


#define D3DFVF_GUI						(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct D3DGUIVertex
{
	float x, y, z, rhw;
	DWORD color;
	float tu, tv;
};

#define D3DGUI_RENDER_VBUFFER(DEV, LPBUF)	{\
	DEV->SetStreamSource(0, LPBUF, 0, sizeof(D3DGUIVertex));\
	DEV->SetFVF(D3DFVF_GUI);\
	DEV->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); }

class D3DGUIControl :public CGUIControl
{
public:
	LPDIRECT3DVERTEXBUFFER9 vbuffer;	// ģ��

	LPDIRECT3DTEXTURE9 m_outTex;		// ����
	LPDIRECT3DTEXTURE9 m_overTex;
	LPDIRECT3DTEXTURE9 m_downTex;

public:
	D3DGUIControl();
	~D3DGUIControl();
	bool Release();

	void Translation(int dx, int dy);// ƽ�� + RefreshVertexBuffer()
	void KillFocus();

	bool RefreshVertexBuffer();
	bool Render(LPDIRECT3DDEVICE9 dev);
};


class GDIGUIControl :public CGUIControl
{
public:
public:
	GDIGUIControl();
	~GDIGUIControl();
	bool Release();
};


#define FOCUS_CONTROL(i)		{ pFocusControl = controls[i]; }
#define BLOCK_ON_CONTROL(i)		{ pFocusControl = controls[i];	block = true; }
#define FOCUS_OFF				pFocusControl = NULL
#define BLOCK_OFF				block = false
#define ISFOCUS(i)				controls[i] == pFocusControl

typedef void (CALLBACK *LPGUIEVENTCALLBACK)(int ID, WPARAM wp, LPARAM lp);

class CD3DGUISystem
{
private:
	LPDIRECT3DDEVICE9 device;						// D3D�豸
	UINT *pbufferw, *pbufferh;						// link�� ʵʱbackbuffer�ߴ�

	LPD3DXFONT defaultfont;							// Ĭ������
	LPD3DXFONT *fonts;								// Direct3D�����������
	D3DGUIControl **controls;						// GUI�ؼ��б�

	D3DGUIControl *backdrop;						// ����ͼ�ؼ�
	bool bUseBackdrop;

	int nFonts;										// ��������
	int nControls;									// �ؼ�����

	bool lastLMBdown;								// �洢��갴�����
	D3DGUIControl *pFocusControl;					// ����
	bool block;										// �Ƿ����� 

	LPGUIEVENTCALLBACK pEventProc;					// �¼��ص�
protected:
	bool ControlListExpand();//��չ�ؼ��б�ռ�

public:
	CD3DGUISystem();
	CD3DGUISystem(LPDIRECT3DDEVICE9 device);
	~CD3DGUISystem();

	bool Bind(D3DWnd *pd3dwnd);
	void Shutdown();
	void OnLostDevice();
	void OnResetDevice();

	// ��������
	bool CreateDXFont(WCHAR *fontName, int *fontID
		, INT Height = 0, INT Width = 0, INT Weight = 0
		, DWORD Quality = DEFAULT_QUALITY // �����˴���
		, UINT MipLevels = 1, bool Italics = false
		, DWORD Charset = DEFAULT_CHARSET, DWORD OutputPrecision = OUT_DEFAULT_PRECIS
		, DWORD PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE
	);

	// �������ڱ��汳��ͼ������״�Ķ��㻺��,������Ҫӳ�䵽�����ϵ�����ͼ��
	// ��Ӿ�̬�ı��ؼ�
	// ��Ӱ�ť�ؼ�
	bool AddBackdrop(WCHAR *TexFileName, float x = 0, float y = 0
		, float width = GUI_DEFAULT_BACKDROPWIDTH, float height = GUI_DEFAULT_BACKDROPHEIGHT
		, byte dock = GUI_WINDOCK_FULLSCREEN);
	bool AddStatic(int ID, float x, float y
		, float width = GUI_DEFAULT_STATICWIDTH, float height = GUI_DEFAULT_STATICHEIGHT
		, WCHAR *text = L"", DWORD color = COLOR_WHITE, int fontID = 0
		, byte dock = GUI_WINDOCK_NORMAL);
	bool AddButton(int ID, float x, float y
		, float width = GUI_DEFAULT_BUTTONWIDTH, float height = GUI_DEFAULT_BUTTONHEIGHT
		, WCHAR *text = L"", int fontID = 0, byte dock = GUI_WINDOCK_NORMAL
		, WCHAR *up = L"1nd.png", WCHAR *over = L"2nd.png", WCHAR *down = L"3nd.png");
	bool AddEdit(int ID, float x, float y
		, float width = GUI_DEFAULT_EDITWIDTH, float height = GUI_DEFAULT_EDITHEIGHT
		, DWORD color = COLOR_WHITE, DWORD txtcolor = COLOR_WHITE, int fontID = 0, byte dock = GUI_WINDOCK_NORMAL);

	bool DropControl(int ID);							// ɾ��һ���ؼ�
	int GetState(int ID);								// ��ȡ�ؼ�״̬
	bool SetControlText(int ID, WCHAR *text);			// ���ÿؼ��ı�
#define ShowControl(ID)			SetControlVisible(ID, true)
#define HideControl(ID)			SetControlVisible(ID, false)
	bool SetControlVisible(int ID, bool bVisible);		// ���ÿؼ��ɼ���
#define EnableControl(ID)		SetControlEnabled(ID, true)
#define DisableControl(ID)		SetControlEnabled(ID, false)
	bool SetControlEnabled(int ID, bool bEnable);		// ���ÿؼ�ʹ��
	LPD3DXFONT GetFont(int ID);							// �������б��ȡ����
	bool SetFocus(int ID);								// ���ý���

	// ���ûص�����
	// �¼�����
	// ����
	void SetEventProc(LPGUIEVENTCALLBACK pevent);
	void HandleMouse(bool LMBDown, LONG mouseX, LONG mouseY);
	void HandleKeyboard(UINT8 keytype, WPARAM wParam);

	void RenderBack();									// ��������
	void Render();										// �ؼ�����
};