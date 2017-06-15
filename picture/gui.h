#pragma once

//#include <thread>
#include "key.h"
#include "generalgeo.h"
#include "misc.h"
#include "fileread.h"

#ifdef USE_D3DGUI
#include "d3dwnd.h"

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif // USE_D3DGUI

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

#define BYTE_PART(I)					((I) & 0xFF)

// ��ɫ
#ifndef SETALPHA
#define SETALPHA(C, A)					((C & 0x00FFFFFF) | (A << 24))
#endif
#define COLOR_WHITE						0xFFFFFFFF

#define GDICOLORPART_RED(C)				((C) & 0xFF)
#define GDICOLORPART_GREEN(C)			((C >> 8) & 0xFF)
#define GDICOLORPART_BLUE(C)			((C >> 16) & 0xFF)
#define COLORGDI_WHITE					0x00FFFFFF
#define COLORGDI_BLACK					0x00000000
#define COLORGDI_MIDGREY				0x00DDDDDD
#define COLORGDI_ORANGE					0x0000FFFF
#define COLORGDI_MIDORANGE				0x0000BAF0
#define COLORGDI_LAKEBLUE				0x00FF901E
#define COLORGDI_BLUE					0x00FF0000
#define COLORGDI_BLUE1					RGB(33, 22, 200)
#define COLORGDI_LAKEGREEN				0x00A6FF34
#define COLORGDI_LIGHTGREY				0x00F1F1F1
#define COLORGDI_HEAVYGREY				0x00606060
#define COLORGDI_DARKGREY				0x00202020
#define COLORGDI_RED					0x000000FF
#define COLORGDI_GREEN1					RGB(60, 230, 1)

#define COLORGDI_DEFAULT				0x00F0F0F0

// �ı��Ű�
// ����Ĭ��TOP������DT_SINGLELINE��ѡ����������λ�á�
// ����Ĭ��LEFT
#define TEXTFORMAT_CENTER				( DT_CENTER | DT_SINGLELINE	| DT_VCENTER | DT_NOCLIP )
#define TEXTFORMAT_LEFT					( /*DT_LEFT	|*/ DT_SINGLELINE | DT_VCENTER | DT_NOCLIP )
#define TEXTFORMAT_RIGHT				( DT_RIGHT	| DT_SINGLELINE	| DT_VCENTER | DT_NOCLIP )
#define TEXTFORMAT_TOP					( DT_CENTER	| /*DT_TOP |*/ DT_NOCLIP )
#define TEXTFORMAT_LEFTTOP				( /*DT_LEFT	| DT_TOP |*/ DT_NOCLIP )
#define TEXTFORMAT_RIGHTTOP				( DT_RIGHT	| /*DT_TOP |*/ DT_NOCLIP )
#define TEXTFORMAT_BOTTOM				( DT_CENTER | DT_SINGLELINE | DT_BOTTOM | DT_NOCLIP )
#define TEXTFORMAT_LEFTBOTTOM			( /*DT_LEFT	|*/ DT_SINGLELINE | DT_BOTTOM | DT_NOCLIP )
#define TEXTFORMAT_RIGHTBOTTOM			( DT_RIGHT	| DT_SINGLELINE | DT_BOTTOM | DT_NOCLIP )

#define TEXTFORMAT_DEFAULT				TEXTFORMAT_CENTER

// ��ʾ����Ч������
#define ALPHASPEED_HIGH					2.0f
#define ALPHASPEED_NORMAL				1.5f
#define ALPHASPEED_LOW					1.0f
#define ALPHASPEED_DEFAULT				ALPHASPEED_NORMAL

// �¼���������ˣ�
// GUIϵͳ����ؼ����¼���GUIϵͳͳһ�����¼������ݸ��ؼ���
#define GUI_MOUSEEVENT_MOVE				0b000
#define GUI_MOUSEEVENT_UP				0b001
#define GUI_MOUSEEVENT_DOWN				0b010
#define GUI_MOUSEEVENT_DOWNMOVE			0b011

#define GUI_KEYEVENT_KEYDOWN			0b00
#define GUI_KEYEVENT_IMECHAR			0b01

// �ؼ�����
#define GUI_CMD_NULL					0
#define GUI_CMD_USER					100
#define GUI_CMD_START					(GUI_CMD_USER + 1)
#define GUI_CMD_PAUSE					(GUI_CMD_USER + 2)
#define GUI_CMD_STOP					(GUI_CMD_USER + 3)

// �ؼ��¼����û��ˣ�
#define GUI_EVENT_NULL					0

#define GUI_EVENT_DOWN					1	
#define GUI_EVENT_UP					2	
#define GUI_EVENT_ENTER					3
#define GUI_EVENT_LEAVE					4
#define GUI_EVENT_OUTUP					5

#define GUI_EVENT_CMD					11
#define GUI_EVENT_TEXTCHANGE			12
#define GUI_EVENT_REFRESH				100

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


//--------------------------------------------------------------------------------------
// Ԥ����Ŀؼ����͡�״̬��ͣ��ģʽ
//--------------------------------------------------------------------------------------
enum GUI_CONTROL_TYPE
{
	GUI_CONTROL_NULL,
	GUI_CONTROL_STATIC,
	GUI_CONTROL_BUTTON,
	GUI_CONTROL_EDIT,
	GUI_CONTROL_BACKDROP,
	GUI_CONTROL_WAVE,
	GUI_CONTROL_TRISTATE
	//GUI_CONTROL_SCROLLBAR,
	//GUI_CONTROL_CHECKBOX,
	//GUI_CONTROL_RADIOBUTTON,
	//GUI_CONTROL_COMBOBOX,
	//GUI_CONTROL_SLIDER,
	//GUI_CONTROL_LISTBOX,
};

enum GUI_CONTROL_STATE
{
	GUI_STATE_OUT,
	GUI_STATE_OVER,
	GUI_STATE_DOWN
};

enum GUI_WINDOCK_MODE
{
	GUI_WINDOCK_NORMAL,
	GUI_WINDOCK_RIGHT,
	GUI_WINDOCK_BOTTOM,
	GUI_WINDOCK_BOTTOMRIGHT,
	GUI_WINDOCK_BOTTOMHSPAN,// ˮƽ��細�ڣ�ͣ���ײ���
	GUI_WINDOCK_FULLSCREEN,// ����
	GUI_WINDOCK_SCALE// ��Դ��ڱ���
};

// �ַ������������ڴ����룩
bool mywcscpy(WCHAR **dest, WCHAR *src);

class CGUIControl
{
protected:
	// ������Ϣ
	GUI_CONTROL_TYPE type;				// �ؼ�����
	int ID;								// �ؼ�ID
										//int groupID;						// �ؼ�����

	virtual void    UpdateRects();
public:
	// ����
	DWORD color;						// ��ɫ

	GUI_WINDOCK_MODE dockMode;			// ͣ��ģʽ
	float dockX, dockY, dockW, dockH;	// ͣ�����꣨����ͣ��ģʽ���в�ͬ���壩
	INT8 dx, dy;						// λ��ƫ��
	int posX, posY;						// �ؼ�λ�ã����Ͻǵ����꣩
	int width, height;					// �ؼ���ȡ��߶�
	RECT boundingBox;					// �ؼ�����

										// �ı�
	int inputPos;						// �����ַ�λ��
	WCHAR *text;						// �ı�����
	DWORD textColor;					// �ı���ɫ
	RECT textRect;						// �ı���ʾ����
	UINT format;

	// ����
	bool bDisabled;						// �ؼ�ʹ��
	bool bVisible;						// �ؼ��ɼ���

	GUI_CONTROL_STATE state;			// ״̬

										// ��ʾ��Ч
	byte displayEvent;					// ������Ч���¼�
	LARGE_INTEGER lastTick;				// ��ʱ
	float alpha;						// ��Чalphaֵ
	INT8 overdx, overdy;				// ��ʾλ��
	INT8 downdx, downdy;

	static LARGE_INTEGER frequency;		// ������Ƶ�ʣ����ھ�ȷ��ʱ��
	static void		GetFrequency()
	{
		QueryPerformanceFrequency(&frequency);
	}
public:
	CGUIControl();
	virtual			~CGUIControl();
	virtual bool	Release();

	byte			GetType() const;
	int				GetID() const;
	void			SetID(int ID);
	bool			SetText(WCHAR *text);
	void			SetLocation(int x, int y);
	void			SetSize(int width, int height);
	void			SetFormat(UINT nFormat);

	//����۽�
	virtual void	KillFocus();
	//����ʹ��
	virtual void	SetEnabled(bool bEnabled)
	{
		bDisabled = !bEnabled;
	}
	virtual bool	GetEnabled() const
	{
		return !bDisabled;
	}
	//���ÿɼ���
	virtual void	SetVisible(bool bVisible)
	{
		this->bVisible = bVisible;
	}
	virtual bool	GetVisible() const
	{
		return bVisible;
	}

	// ƽ��
	virtual void	Translation(int dx, int dy);
	// ͣ��
	virtual void	Dock(UINT *pBufferW, UINT *pBufferH);

	// ���̴���
	virtual bool	HandleKeyboard(WPARAM wParam, bool IME = false);
	// ��괦��
	virtual byte	HandleMouse(byte LMBState, LONG mouseX, LONG mouseY);
	// �����
	virtual void	HandleCMD(UINT cmd)
	{
	}

	// ������Ч����Ҫ�ػ棩
	virtual void	Invalidate();
	// ����
	virtual bool	Render()
	{
		return false;
	};
};


#ifdef USE_D3DGUI
#define D3DFVF_GUI			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

void D3DErrorShow(HRESULT hr, WCHAR *msg = NULL, HWND hWnd = NULL, WCHAR *title = L"ERROR");
void ChangeAlpha(LPDIRECT3DVERTEXBUFFER9 pbuf, byte alpha);


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
	LPD3DXFONT pFontPic;					// �ı�����

	LPDIRECT3DVERTEXBUFFER9 vbuffer;	// ģ��

	LPDIRECT3DTEXTURE9 m_outTex;		// ����
public:
	D3DGUIControl();
	virtual			~D3DGUIControl();
	virtual bool	Release();

	virtual void	Translation(int dx, int dy);// ƽ�� + RefreshVertexBuffer()
	virtual bool	RefreshVertexBuffer();
	virtual void	KillFocus();

	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIStatic :public D3DGUIControl
{
public:
	D3DGUIStatic();
public:
	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIButton :public D3DGUIControl
{
public:
	LPDIRECT3DTEXTURE9 m_overTex;
	LPDIRECT3DTEXTURE9 m_downTex;
public:
	D3DGUIButton();
	virtual			~D3DGUIButton();
	virtual bool	Release();

	virtual byte	HandleMouse(byte LMBState, LONG mouseX, LONG mouseY);
	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIEdit :public D3DGUIControl
{
public:
	D3DGUIEdit();

	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIBack :public D3DGUIControl
{
public:
	D3DGUIBack();

	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
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
	UINT *pBufferW, *pBufferH;						// link�� ʵʱbackbuffer�ߴ�

	LPD3DXFONT defaultfont;							// Ĭ������
	LPD3DXFONT *fonts;								// Direct3D�����������
	CGrowableArray <D3DGUIControl*> controls;		// GUI�ؼ��б�

	D3DGUIControl *pBackdrop;						// ����ͼ�ؼ�
	bool bUseBackdrop;								// �Ƿ�ʹ�ñ���

	int nFonts;										// ��������
	int nControls;									// �ؼ�����

	bool lastLMBdown;								// �洢��갴�����
	D3DGUIControl *pFocusControl;					// ����
	bool block;										// �Ƿ����� 

	LPGUIEVENTCALLBACK pEventProc;					// �¼��ص�
protected:
	void			VarInit();
	bool			AddControl(D3DGUIControl *pControl);

public:
	CD3DGUISystem();
	CD3DGUISystem(LPDIRECT3DDEVICE9 device);
	~CD3DGUISystem();

	bool			Bind(D3DWnd *pD3DWnd);			// ���󴰿�
	void			Shutdown();
	void			OnLostDevice();
	void			OnResetDevice();

	// ����Զ�������
	bool AddDXFont(WCHAR *fontName, int *fontID, INT Height = 0, INT Width = 0, INT Weight = FW_NORMAL
		, DWORD Quality = DEFAULT_QUALITY, UINT MipLevels = 1, bool Italics = FALSE
		, DWORD Charset = DEFAULT_CHARSET, DWORD OutputPrecision = OUT_DEFAULT_PRECIS
		, DWORD PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE);// �����˲�������

															  // �������ڱ��汳��ͼ������״�Ķ��㻺��,������Ҫӳ�䵽�����ϵ�����ͼƬ
															  // ��Ӿ�̬�ı��ؼ�
															  // ��Ӱ�ť�ؼ�
	bool AddBackdrop(WCHAR *TexFileName, float x = 0, float y = 0
		, float width = GUI_DEFAULT_BACKDROPWIDTH, float height = GUI_DEFAULT_BACKDROPHEIGHT
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_FULLSCREEN);
	bool AddStatic(int ID, float x, float y
		, float width = GUI_DEFAULT_STATICWIDTH, float height = GUI_DEFAULT_STATICHEIGHT
		, WCHAR *text = L"", DWORD color = COLOR_WHITE, int fontID = 0
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);
	bool AddButton(int ID, float x, float y
		, float width = GUI_DEFAULT_BUTTONWIDTH, float height = GUI_DEFAULT_BUTTONHEIGHT
		, WCHAR *text = L"", DWORD color = COLOR_WHITE, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *up = L"1nd.png", WCHAR *over = L"2nd.png", WCHAR *down = L"3nd.png");
	bool AddEdit(int ID, float x, float y
		, float width = GUI_DEFAULT_EDITWIDTH, float height = GUI_DEFAULT_EDITHEIGHT
		, DWORD color = COLOR_WHITE, DWORD txtcolor = COLOR_WHITE, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);

	bool			DropControl(int ID);						// ɾ���ؼ�
	int				GetState(int ID) const;						// ��ȡ�ؼ�״̬
	bool			SetControlText(int ID, WCHAR *text);		// ���ÿؼ��ı�
#define ShowControl(ID)			SetControlVisible(ID, true)
#define HideControl(ID)			SetControlVisible(ID, false)
	bool			SetControlVisible(int ID, bool bVisible);	// ���ÿؼ��ɼ���
#define EnableControl(ID)		SetControlEnabled(ID, true)
#define DisableControl(ID)		SetControlEnabled(ID, false)
	bool			SetControlEnabled(int ID, bool bEnable);	// ���ÿؼ�ʹ��
	LPD3DXFONT		GetFont(int ID) const;						// �������б��ȡ����
	bool			SetFocus(int ID);							// ���ý���
	bool			ClearFocus();

	// ���ûص�����
	// �¼�����
	// ����
	void			SetCallbackEvent(LPGUIEVENTCALLBACK pevent);
	void			HandleMouse(bool LMBDown, LONG mouseX, LONG mouseY);
	void			HandleKeyboard(UINT8 keytype, WPARAM wParam);

	void			RenderBack();								// ��������
	void			Render();									// �ؼ�����
};
#endif // USE_D3DGUI


#ifdef USE_GDIGUI

inline bool SmoothLine(CDC *pDC, const POINT &start, const POINT &end, const DWORD &color = COLORGDI_BLACK, const DWORD &backcolor = COLORGDI_WHITE);
inline bool SmoothLine2(CDC *pDC, const POINT &start, const POINT &end, const DWORD &color = COLORGDI_BLACK, const DWORD &backcolor = COLORGDI_WHITE);



//�ӿڲ���
#undef INTERFACE
#define INTERFACE IGDIDevice

DECLARE_INTERFACE_(IGDIDevice, IUnknown)
{
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	/*** IGDIDevice methods ***/
	STDMETHOD(SetDevice)(THIS_ CDC *pdc) PURE;
	STDMETHOD_(bool, IsDCNull)(THIS) PURE;
	STDMETHOD_(CDC *, GetDestDC)(THIS) PURE;
	STDMETHOD_(CDC *, GetMemDC)(THIS) PURE;
};

typedef struct IGDIDevice *LPGDIDevice, *PGDIDevice;

class GDIDevice :public IGDIDevice {
private:
	memDCBMP *pmDB;	// �ڴ�DC
	CDC *pdevDC;	// Ŀ��DC

public:
	GDIDevice();
	~GDIDevice();

	HRESULT __stdcall	QueryInterface(const IID &riid, void** ppvObj);
	ULONG __stdcall		AddRef();
	ULONG __stdcall		Release();

	HRESULT __stdcall	SetDevice(CDC *pdc);
	bool __stdcall		IsDCNull()
	{
		return pdevDC == NULL || pmDB == NULL || pmDB->pDC == NULL;
	}
	CDC * __stdcall		GetDestDC()
	{
		return pdevDC;
	}
	CDC * __stdcall		GetMemDC()
	{
		return pmDB != NULL ? pmDB->pDC : NULL;
	}
};


class GDIGUIControl :public CGUIControl
{
public:
	CFont *font;	// �ı���ʾ����

	memPic *pic;	// ��ͼ
public:
	GDIGUIControl();
	virtual			~GDIGUIControl();
	virtual bool	Release();

	virtual void	DisplayCycle(LPGDIDevice dev);

	virtual bool	RenderText(CDC *pDC, UINT nFormat = TEXTFORMAT_DEFAULT, DWORD *pNewColor = NULL);
	virtual bool	Render(LPGDIDevice dev);
};

class GDIGUIStatic :public GDIGUIControl
{
public:
	GDIGUIStatic();
public:
	virtual bool	Render(LPGDIDevice dev);
};

class GDIGUIButton :public GDIGUIControl
{
public:
	memPic *picon;		//��꾭������ͼ
	memPic *picdown;	//����ʱ����ͼ
public:
	GDIGUIButton();
	virtual			~GDIGUIButton();
	virtual bool	Release();

	virtual byte	HandleMouse(byte LMBState, LONG mouseX, LONG mouseY);
	virtual void	DisplayCycle(LPGDIDevice dev);

	virtual bool	Render(LPGDIDevice dev);
};

enum GUI_TRISTATE
{
	GUI_TRISTATE_NORMAL,
	GUI_TRISTATE_ON,
	GUI_TRISTATE_OFF
};

class GDIGUITristate :public GDIGUIControl
{
public:
	memPic *picon;		//״̬#2��ͼ
	memPic *picoff;		//״̬#3��ͼ

	GUI_TRISTATE tristate;

	virtual void    UpdateRects();
public:
	GDIGUITristate();
	virtual			~GDIGUITristate();
	virtual bool	Release();

	void			SetOn();	// ����״̬
	void			SetOff();
	void			SetNormal();

	virtual bool	Render(LPGDIDevice dev);
};


#define GUIWAVE_STATE_STOPED	0
#define GUIWAVE_STATE_RUNNING	1
#define GUIWAVE_STATE_PAUSED	2

typedef short		SAMPLE_TYPE;

struct sample {
	SAMPLE_TYPE amp;
	LONGLONG time;
};

struct sample3D {
	SAMPLE_TYPE ampX;
	SAMPLE_TYPE ampY;
	SAMPLE_TYPE ampZ;
	LONGLONG time;
};

class GDIGUIWave :public GDIGUIControl
{
private:
	byte waveState;

	sample3D amp;					// ���β����������
	int lastX;						// ��һ��������������λ�ã���������
	float lastYx;					// ��һ������������������ظ߶ȣ�
	float lastYy;					// ��һ������������������ظ߶ȣ�
	float lastYz;					// ��һ������������������ظ߶ȣ�
	int waveMoved;					// �������ƶ����루��������
	int curwavemoved;				// ���β����ƶ��˵ľ���
	LARGE_INTEGER startTime;		// ��ʼ����ʱ��
	LARGE_INTEGER pauseTime;

	CGrowableArray <sample3D> samples;// ��������
	float timeSpan;					// ���ο�ʱ����
	float ampSpan;					// �����׼
	float freq;						// ����Ƶ��
	DWORD lineColor;				// ������ɫ
	DWORD lineColorY;
	DWORD lineColorZ;

	memDCBMP *waveBuf;				// ���λ�ͼ

	bool			SetStopBackground();
public:
	GDIGUIWave();
	virtual			~GDIGUIWave();
	virtual bool	Release();

	void			PrepareWaveDC(CDC * pdc);

	virtual void	DisplayCycle(LPGDIDevice dev);
	virtual void	HandleCMD(UINT cmd);

	virtual void	AddSample(SAMPLE_TYPE ampval1, SAMPLE_TYPE ampval2, SAMPLE_TYPE ampval3, LONGLONG tick = -1);
	virtual void	AddSampleX(SAMPLE_TYPE ampval, LONGLONG tick = -1);
	byte			GetState() const
	{
		return waveState;
	}
	void			SetLineColor(DWORD color)
	{
		lineColor = color;

		byte r = GDICOLORPART_RED(color);
		byte g = GDICOLORPART_GREEN(color);
		byte b = GDICOLORPART_BLUE(color);
		lineColorY = RGB(b, r, g);
		lineColorZ = RGB(g, r, b);
	}
	DWORD			GetLineColor() const
	{
		return lineColor;
	}
	void			Start();
	void			Pause();
	void			Resume();
	void			Stop();
	virtual bool	Render(LPGDIDevice dev);
};

class GDIGUIScrollBar : public GDIGUIControl
{
protected:
	int nItems;			// ѡ����Ŀ
	int position;		// Position of the first displayed item
	int pageSize;		// How many items are displayable in one page
	RECT rcUpButton;	// �ϰ�ť����
	RECT rcDownButton;	// �°�ť����
	RECT rcTrack;		// �������
	RECT rcThumb;		// ��������

	bool bShowThumb;	// �Ƿ���ʾ����
};


#define FOCUS_CONTROL(i)		{ pControlFocus = controls[i]; }
#define BLOCK_ON_CONTROL(i)		{ pControlFocus = controls[i];	bBlock = true; }
#define FOCUS_OFF				{ pControlFocus = NULL; }
#define BLOCK_OFF				{ bBlock = false; }
#define ISFOCUS(i)				( controls[i] == pControlFocus )

typedef void (CALLBACK *LPGUIEVENTCALLBACK)(int ID, WPARAM wp, LPARAM lp);

struct tpgui
{
	void *pguisys;
};

class CGDIGUISystem
{
private:
	LPGDIDevice device;
	UINT *pBufW, *pBufH;						// link�� ʵʱbackbuffer�ߴ�

	CFont *fontDefault;							// Ĭ������
	CFont **fonts;								// �����������
	CGrowableArray <GDIGUIControl*> controls;	// GUI�ؼ��б�

	GDIGUIControl *backdrop;					// ����ͼ�ؼ�
	bool bUseBackdrop;

	int nFonts;									// ��������
	int nControls;								// �ؼ�����

	bool lastLMBdown;							// �洢��갴�����
	GDIGUIControl *pControlFocus;				// ����
	bool bBlock;								// �Ƿ����� 

	LPGUIEVENTCALLBACK pEventProc;				// �¼��ص�

	tpgui threadParam;
	DWORD threadID;
	HANDLE hThreadGUI;
	static void		ThreadGUI(LPVOID lpParam);	// GUI������ʾЧ���߳�

protected:
	void			VarInit();					// ������ʼ��
	bool			AddControl(GDIGUIControl *pControl);

public:
	static float time;							// �ڲ���ʱ

	CGDIGUISystem();
	CGDIGUISystem(LPGDIDevice dev);
	~CGDIGUISystem();

	void			BeginGUIThread();			// ����GUI������ʾЧ���߳�
												//bool			Bind(CWnd *pWnd);			// ���󴰿�
	void			ShutDown();

	CFont			*GetFont(int ID) const;
	bool			SetControlText(int ID, WCHAR *text);// ���ÿؼ��ı�

														// �������
	bool AddGDIFont(WCHAR *fontName, int *fontID, INT Height = 0, INT Width = 0, INT Weight = 0
		, BYTE Quality = DEFAULT_QUALITY, int Escapement = 0, int Orientation = 0
		, bool Italics = false, bool Underline = false, bool StrikeOut = false
		, BYTE Charset = DEFAULT_CHARSET, BYTE OutputPrecision = OUT_DEFAULT_PRECIS
		, BYTE ClipPrecision = CLIP_DEFAULT_PRECIS
		, BYTE PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE);// �����˲�������

	bool AddStatic(int ID, float x, float y
		, float width = GUI_DEFAULT_STATICWIDTH, float height = GUI_DEFAULT_STATICHEIGHT
		, WCHAR *text = L"", DWORD color = COLORGDI_WHITE, int fontID = 0
		, WCHAR *file = NULL, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);
	bool AddButton(int ID, float x, float y
		, float width = GUI_DEFAULT_BUTTONWIDTH, float height = GUI_DEFAULT_BUTTONHEIGHT
		, WCHAR *text = L"", DWORD color = COLORGDI_BLACK, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *up = L"btn1.png", WCHAR *on = L"btn2.png", WCHAR *down = L"btn3.png");
	bool AddWave(int ID, float x, float y
		, float width = 300, float height = 100
		, DWORD color = COLORGDI_WHITE, DWORD lineColor = COLORGDI_DARKGREY
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL, int fontID = 0);
	bool AddTristate(int ID, float x, float y
		, float width = 10, float height = 10
		, WCHAR *text = L"", DWORD color = COLORGDI_BLACK, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *normal = L"drip2.png", WCHAR *on = L"drip2g.png", WCHAR *off = L"drip2o.png");

	// Control retrieval
	GDIGUIControl*	GetControl(int ID, UINT nControlType) const;
	GDIGUIStatic*	GetStatic(int ID) const
	{
		return (GDIGUIStatic*)GetControl(ID, GUI_CONTROL_STATIC);
	}
	GDIGUIButton*	GetButton(int ID) const
	{
		return (GDIGUIButton*)GetControl(ID, GUI_CONTROL_BUTTON);
	}
	GDIGUITristate*	GetTristate(int ID) const
	{
		return (GDIGUITristate*)GetControl(ID, GUI_CONTROL_TRISTATE);
	}
	GDIGUIWave*		GetWave(int ID) const
	{
		return (GDIGUIWave*)GetControl(ID, GUI_CONTROL_WAVE);
	}

	// ���ûص�����
	// �¼�����
	void			SetCallbackEvent(LPGUIEVENTCALLBACK pevent);
	void			HandleMouse(bool LMBDown, LONG mouseX, LONG mouseY);
	void			HandleKeyboard(UINT8 keytype, WPARAM wParam);
	void			HandleCMD(int ID, UINT cmd);

	void			Invalidate();
	void			Render();
};

#endif // USE_GDIGUI