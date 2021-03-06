#pragma once

//#include <thread>
#include "key.h"
#include "generalgeo.h"
#include "misc.h"
#include "fileread.h"
#include <strsafe.h>

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

// 颜色
#ifndef SETALPHA
#define SETALPHA(C, A)					((C & 0x00FFFFFF) | (A << 24))
#endif
#define COLOR_WHITE						0xFFFFFFFF

#define GDICOLORPART_RED(C)				((C) & 0xFF)
#define GDICOLORPART_GREEN(C)			((C >> 8) & 0xFF)
#define GDICOLORPART_BLUE(C)			((C >> 16) & 0xFF)
#define COLORGDI_WHITE					0x00FFFFFF
#define COLORGDI_BLACK					0x00000000
#define COLORGDI_MIDGREY				0x00CCCCCC
#define COLORGDI_ORANGE					0x0000FFFF
#define COLORGDI_MIDORANGE				0x0000BAF0
#define COLORGDI_LAKEBLUE				0x00FF901E
#define COLORGDI_BLUE					0x00FF0000
#define COLORGDI_BLUE1					RGB(33, 22, 200)
#define COLORGDI_BLUE2					RGB(80, 60, 240)
#define COLORGDI_GREEN1					RGB(60, 230, 1)
#define COLORGDI_GREEN2					0x0044BB66
#define COLORGDI_GREEN3					0x00226633
#define COLORGDI_LAKEGREEN				0x00A6FF34
#define COLORGDI_GREY					0x00DCDCDC
#define COLORGDI_LIGHTGREY				0x00F0F0F0
#define COLORGDI_HEAVYGREY				0x00606060
#define COLORGDI_DARKGREY				0x00202020
#define COLORGDI_RED					0x000000FF
#define COLORGDI_RED2					0x003322AA

#define COLORGDI_DEFAULT				0x00F0F0F0

// 文本排版
// 纵向默认TOP。加上DT_SINGLELINE可选择其它纵向位置。
// 横向默认LEFT
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

// 显示渐变效果速率
#define ALPHASPEED_HIGH					2.0f
#define ALPHASPEED_NORMAL				1.5f
#define ALPHASPEED_LOW					1.0f
#define ALPHASPEED_DEFAULT				ALPHASPEED_NORMAL

// 控件命令
#define GUI_CMD_NULL					0
#define GUI_CMD_USER					100
#define GUI_CMD_START					(GUI_CMD_USER + 1)
#define GUI_CMD_PAUSE					(GUI_CMD_USER + 2)
#define GUI_CMD_STOP					(GUI_CMD_USER + 3)

// 控件事件（用户端）
#define GUI_EVENT_NULL					0
#define GUI_EVENT_LBUTTONDOWN			1	
#define GUI_EVENT_LBUTTONUP				2	
#define GUI_EVENT_LBUTTONCLICKED		GUI_EVENT_LBUTTONUP	
#define GUI_EVENT_RBUTTONDOWN			3
#define GUI_EVENT_RBUTTONUP				4
#define GUI_EVENT_MOUSEENTER			5
#define GUI_EVENT_MOUSELEAVE			6
#define GUI_EVENT_MOUSEMOVE				7
#define GUI_EVENT_MOUSEWHEEL			8
#define GUI_EVENT_LBUTTONOUTUP			11
// 上层事件（对下层的扩展、覆盖）
#define GUI_EVENT_CMD					11
#define GUI_EVENT_TEXTCHANGE			12
#define GUI_EVENT_SELCHANGE				13
#define GUI_EVENT_SCROLL				14
#define GUI_EVENT_REFRESH				100

// 参数
#define GUI_DEFAULT_STATICWIDTH			120
#define GUI_DEFAULT_STATICHEIGHT		30
#define GUI_DEFAULT_BUTTONWIDTH			80
#define GUI_DEFAULT_BUTTONHEIGHT		30
#define GUI_DEFAULT_EDITWIDTH			80
#define GUI_DEFAULT_EDITHEIGHT			30
#define GUI_DEFAULT_BACKDROPWIDTH		120
#define GUI_DEFAULT_BACKDROPHEIGHT		60

#define GUI_DEFAULTTEXTLEN				63

// Minimum scroll bar thumb size
#define SCROLLBAR_MINTHUMBSIZE			8

//--------------------------------------------------------------------------------------
// 预定义的控件类型、状态、停靠模式
//--------------------------------------------------------------------------------------
enum GUI_CONTROL_TYPE
{
	GUI_CONTROL_NULL,
	GUI_CONTROL_STATIC,
	GUI_CONTROL_BUTTON,
	GUI_CONTROL_EDIT,
	GUI_CONTROL_BACKDROP,
	GUI_CONTROL_WAVE,
	GUI_CONTROL_TRISTATE,
	GUI_CONTROL_SWITCH,
	GUI_CONTROL_SCROLLBAR,
	GUI_CONTROL_COMBOBOX
	//GUI_CONTROL_CHECKBOX,
	//GUI_CONTROL_RADIOBUTTON,
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
	GUI_WINDOCK_BOTTOMHSPAN,// 水平横跨窗口（停靠底部）
	GUI_WINDOCK_FULLSCREEN,// 铺满
	GUI_WINDOCK_SCALE// 相对窗口比例
};

// 字符串拷贝（带内存申请）
bool mywcscpy(WCHAR **dest, WCHAR *src);

class CGUIControl
{
protected:
	// 基本信息
	GUI_CONTROL_TYPE type;				// 控件类型
	int ID;								// 控件ID
										//int groupID;						// 控件分组

	virtual void    UpdateRects();
public:
	// 属性
	DWORD color;						// 颜色

	GUI_WINDOCK_MODE dockMode;			// 停靠模式
	float dockX, dockY, dockW, dockH;	// 停靠坐标（根据停靠模式，有不同含义）
	INT8 dx, dy;						// 位置偏移
	int posX, posY;						// 控件位置（左上角的坐标）
	int width, height;					// 控件宽度、高度
	RECT rcBoundingBox;					// 控件区域

										// 文本
	int inputPos;						// 输入字符位置
	WCHAR *strText;						// 文本内容
	RECT rcText;						// 文本显示区域
	DWORD textColor;					// 文本颜色
	UINT textFormat;					// 文本格式

										// 参数
	bool bDisabled;						// 控件使能
	bool bVisible;						// 控件可见性

	GUI_CONTROL_STATE state;			// 状态

										// 显示特效
	byte displayEvent;					// 激发特效的事件
	LARGE_INTEGER lastTick;				// 计时
	float alpha;						// 特效alpha值
	INT8 overdx, overdy;				// 显示位移
	INT8 downdx, downdy;

	static LARGE_INTEGER frequency;		// 计数器频率（用于精确计时）
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
	bool			SetText(WCHAR *strText);
	void			SetLocation(int x, int y);
	void			SetSize(int width, int height);
	void			SetFormat(UINT nFormat);

	//解除聚焦
	virtual void	KillFocus();
	//设置使能
	virtual void	SetEnabled(bool bEnabled)
	{
		bDisabled = !bEnabled;
		displayEvent = GUI_EVENT_REFRESH;
	}
	virtual bool	GetEnabled() const
	{
		return !bDisabled;
	}
	//设置可见性
	virtual void	SetVisible(bool bVisible)
	{
		this->bVisible = bVisible;
		displayEvent = GUI_EVENT_REFRESH;
	}
	virtual bool	GetVisible() const
	{
		return bVisible;
	}

	// 平移
	virtual void	Translation(int dx, int dy);
	// 停靠
	virtual void	Dock(UINT *pbufw, UINT *pbufh);

	// 键盘处理
	virtual bool	HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 鼠标处理
	virtual byte	HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	// 命令处理
	virtual void	HandleCMD(UINT cmd)
	{
	}

	// 设置无效（需要重绘）
	virtual void	Invalidate();
	// 绘制
	virtual bool	Render()
	{
		return false;
	};
};


#ifdef USE_D3DGUI
#define D3DFVF_GUI			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

void D3DErrorShow(HRESULT hr, WCHAR *msg = NULL, HWND hwnd = NULL, WCHAR *title = L"ERROR");
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
	LPD3DXFONT font;					// 文本字体

	LPDIRECT3DVERTEXBUFFER9 model;		// 模型

	LPDIRECT3DTEXTURE9 tex;				// 纹理
public:
	D3DGUIControl();
	virtual			~D3DGUIControl();
	virtual bool	Release();

	virtual void	Translation(int dx, int dy);// 平移 + RefreshVertexBuffer()
	virtual bool	RefreshVertexBuffer();
	virtual void	KillFocus();

	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIStatic :public D3DGUIControl
{
public:
	D3DGUIStatic();

	virtual bool	Render(LPDIRECT3DDEVICE9 dev);
};

class D3DGUIButton :public D3DGUIControl
{
public:
	LPDIRECT3DTEXTURE9 texOver;
	LPDIRECT3DTEXTURE9 texDown;
public:
	D3DGUIButton();
	virtual			~D3DGUIButton();
	virtual bool	Release();

	virtual byte	HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
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
#define BLOCK_ON_CONTROL(i)		{ pFocusControl = controls[i];	bBlock = true; }
#define FOCUS_OFF				pFocusControl = NULL
#define BLOCK_OFF				bBlock = false
#define ISFOCUS(i)				controls[i] == pFocusControl

typedef void (CALLBACK *LPGUIEVENTCALLBACK)(int ID, WPARAM wp, LPARAM lp);

class CD3DGUISystem
{
private:
	LPDIRECT3DDEVICE9 device;						// D3D设备
	D3DWnd *pD3DWnd;
	UINT *pBufferW, *pBufferH;						// link， 实时backbuffer尺寸

	LPD3DXFONT fontDefault;							// 默认字体
	LPD3DXFONT *fonts;								// Direct3D字体对象链表
	CGrowableArray <D3DGUIControl*> controls;		// GUI控件列表

	D3DGUIControl *pBackdrop;						// 背景图控件
	bool bUseBackdrop;								// 是否使用背景

	int nFonts;										// 字体总数
	int nControls;									// 控件总数

	D3DGUIControl *pFocusControl;					// 焦点
	bool bBlock;									// 是否阻塞 

	LPGUIEVENTCALLBACK pCallbackEvent;				// 事件回调
protected:
	void			VarInit();
	bool			AddControl(D3DGUIControl *pControl);

public:
	CD3DGUISystem();
	CD3DGUISystem(LPDIRECT3DDEVICE9 device);
	~CD3DGUISystem();

	bool			Bind(D3DWnd *pd3dwnd);			// 捆绑窗口
	void			Shutdown();
	void			OnLostDevice();
	void			OnResetDevice();

	bool			MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 添加自定义字体
	bool AddDXFont(WCHAR *fontName, int *fontID, INT Height = 0, INT Width = 0, INT Weight = FW_NORMAL
		, DWORD Quality = DEFAULT_QUALITY, UINT MipLevels = 1, bool Italics = FALSE
		, DWORD Charset = DEFAULT_CHARSET, DWORD OutputPrecision = OUT_DEFAULT_PRECIS
		, DWORD PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE);// 交换了参数次序

															  // 创建用于保存背景图几何形状的顶点缓存,并加载要映射到表面上的纹理图片
															  // 添加静态文本控件
															  // 添加按钮控件
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

	bool			DropControl(int ID);						// 删除控件
	int				GetState(int ID) const;						// 获取控件状态
	bool			SetControlText(int ID, WCHAR *text);		// 设置控件文本
#define ShowControl(ID)			SetControlVisible(ID, true)
#define HideControl(ID)			SetControlVisible(ID, false)
	bool			SetControlVisible(int ID, bool bVisible);	// 设置控件可见性
#define EnableControl(ID)		SetControlEnabled(ID, true)
#define DisableControl(ID)		SetControlEnabled(ID, false)
	bool			SetControlEnabled(int ID, bool bEnable);	// 设置控件使能
	LPD3DXFONT		GetFont(int ID) const;						// 从字体列表获取字体
	bool			SetFocus(int ID);							// 设置焦点
	bool			ClearFocus();

	// 设置回调函数
	// 事件处理
	// 绘制
	void			SetCallbackEvent(LPGUIEVENTCALLBACK pevent);
	void			HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	void			HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void			RenderBack();								// 背景绘制
	void			Render();									// 控件绘制
};
#endif // USE_D3DGUI


#ifdef USE_GDIGUI

inline bool SmoothLine(CDC *pDC, const POINT &start, const POINT &end, const DWORD &color = COLORGDI_BLACK);
inline bool SmoothLine2(CDC *pDC, const POINT &start, const POINT &end, const DWORD &color = COLORGDI_BLACK);



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
	STDMETHOD_(CDC *, GetMemDCBack)(THIS) PURE;
	STDMETHOD_(bool, GetRect)(THIS_ RECT *pRect) PURE;
	STDMETHOD_(bool, Present)(THIS_ int x, int y, int w, int h, int sx, int sy) PURE;
};

typedef struct IGDIDevice *LPGDIDevice, *PGDIDevice;

class GDIDevice :public IGDIDevice {
private:
	memDCBMP *pmDB;	// 内存DC
	memDCBMP *pmDBBack;	// 内存DC(背景)
	CDC *pdevDC;	// 目标DC
	RECT rcRedraw;

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
	CDC * __stdcall		GetMemDCBack()
	{
		return pmDBBack != NULL ? pmDBBack->pDC : NULL;
	}
	bool __stdcall		GetRect(RECT *pRect)
	{
		if (pRect == NULL)
			return false;
		if (pmDB != NULL)
		{
			*pRect = RECT(pmDB->x, pmDB->y, pmDB->cx, pmDB->cy);
			return true;
		}

		return false;
	}
	bool __stdcall		Present(int x, int y, int w, int h, int sx, int sy)
	{
		if (pdevDC != NULL && pmDB != NULL && pmDB->pDC != NULL)
		{
			if (rcRedraw.left == 0 && rcRedraw.right == 0
				&& rcRedraw.top == 0 && rcRedraw.bottom == 0)
			{
				rcRedraw = RECT(sx, sy, w, h);
			}
			else
			{
				RECT rcNew = RECT(sx, sy, w, h);
				UnionRect(&rcRedraw, &rcRedraw, &rcNew);
			}
			//pdevDC->TransparentBlt();
			pdevDC->BitBlt(x, y, w, h, pmDB->pDC, sx, sy, SRCCOPY);
			return true;
		}

		return false;
	}
};


class GDIGUIControl :public CGUIControl
{
public:
	CFont *font;	// 文本显示字体

	memPic *pic;	// 贴图
public:
	int test;
	GDIGUIControl();
	virtual			~GDIGUIControl();
	virtual bool	Release();

	virtual bool	DisplayCycle(LPGDIDevice dev);

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

class GDIGUIBack :public GDIGUIControl
{
public:
	GDIGUIBack();

	virtual bool	Render(LPGDIDevice dev);
};

class GDIGUIButton :public GDIGUIControl
{
public:
	memPic *picon;		//鼠标经过的贴图
	memPic *picdown;	//按下时的贴图
public:
	GDIGUIButton();
	virtual			~GDIGUIButton();
	virtual bool	Release();

	virtual byte	HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	virtual bool	DisplayCycle(LPGDIDevice dev);

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
	memPic *picon;		//状态#2贴图
	memPic *picoff;		//状态#3贴图

	GUI_TRISTATE tristate;

	virtual void    UpdateRects();
public:
	GDIGUITristate();
	virtual			~GDIGUITristate();
	virtual bool	Release();

	void			SetOn();	// 设置状态
	void			SetOff();
	void			SetNormal();

	virtual bool	Render(LPGDIDevice dev);
};


class GDIGUISwitch :public GDIGUIControl
{
public:
	memPic *picon;		//状态#2贴图
	bool stateOn;

	virtual void    UpdateRects();
public:
	GDIGUISwitch();
	virtual			~GDIGUISwitch();
	virtual bool	Release();

	bool			GetOn();
	void			SetOn();	// 设置状态
	void			SetOff();

	virtual bool	Render(LPGDIDevice dev);
};

class GDIGUIEdit :public GDIGUIControl
{
public:
public:
	GDIGUIEdit();
	//virtual			~GDIGUIEdit();
	//virtual bool	Release();

	virtual bool	Render(LPGDIDevice dev);
};


#define WAVE_STATE_STOPED	0
#define WAVE_STATE_RUNNING	1
#define WAVE_STATE_PAUSED	2


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

enum TIMETYPE {
	TIMETYPE_1,
	TIMETYPE_2
};
struct timeStamp {
	LONGLONG time;
	TIMETYPE type;
};

class GDIGUIWave :public GDIGUIControl
{
private:
	byte waveState;					// 状态

									// 参数
	float timeSpan;					// 波形框时间跨度
	float ampSpan;					// 振幅基准
	DWORD lineColor;				// 线条颜色
	DWORD lineColorY;
	DWORD lineColorZ;

	// 样本
	CGrowableArray <sample3D> samples;// 样本数组
	CGrowableArray <timeStamp> times;// 峰值数组
	float freq;						// 样本频率
	int nHandled;					// 已经处理了多少样本（绘制结束即完成处理，已加入列表但没绘制算没处理）
	int nRenderd;					// 已经绘制了多少样本
	int nReview;					// 波形值查看，在列表中位置

	memDCBMP *waveBuf;				// 波形绘图
									// 辅助绘图
	sample3D amp;					// 本次采样样本振幅
	int lastX;						// 上一个采样样本横向位置（像素数）
	float lastYx;					// 上一个采样样本振幅（像素高度）
	float lastYy;					// 上一个采样样本振幅（像素高度）
	float lastYz;					// 上一个采样样本振幅（像素高度）
	int waveMoved;					// 波形总移动距离（像素数）
	int newWaveMoved;				// 本次波形移动了的距离

									// 计时
	LARGE_INTEGER startTime;		// 起始采样时间
	LARGE_INTEGER pauseTime;		// 暂停时间（用户恢复暂停时更新起始时间）
	LARGE_INTEGER curTime;			// 记录当前绘制时间
	LONGLONG lastBeat;				// 心跳（用于计算样本频率）
	LONGLONG secRec;				// 整数秒数记录（用于按秒操作）
	double sec;						// 精确的秒数记录

	bool			SetEmptyBackground();//绘制停止状态的波形图
	void			RestoreFlag();	//复位参数，一次采样结束，第二次开始时复位
public:
	GDIGUIWave();
	virtual			~GDIGUIWave();
	virtual bool	Release();

	void			PrepareWaveDC(CDC * pdc);

	virtual byte	HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	virtual bool	DisplayCycle(LPGDIDevice dev);
	virtual void	HandleCMD(UINT cmd);

	virtual void	AddSample(SAMPLE_TYPE ampval1, SAMPLE_TYPE ampval2, SAMPLE_TYPE ampval3, LONGLONG tick = -1);
	virtual void	AddTimeStamp(LONGLONG peakTime, TIMETYPE type = TIMETYPE_1);
	const sample3D*	GetLastSample() const;
	int				GetSampleCount() const;
	byte			GetState() const
	{
		return waveState;
	}
	void			SetTimeSpan(float span);
	void			SetAmpSpan(float span);
	void			SetLineColor(DWORD color)
	{
		lineColor = color;

		byte r = GDICOLORPART_RED(color);
		byte g = GDICOLORPART_GREEN(color);
		byte b = GDICOLORPART_BLUE(color);
		lineColorY = RGB(g, b, r);
		lineColorZ = RGB(b, r, g);
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
public:
	memPic *picTrack;
	memPic *picThumb;
	memPic *picUpButton;
	memPic *picDownButton;

	GDIGUIScrollBar();
	virtual         ~GDIGUIScrollBar();

	void InitPic();
	virtual byte    HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	virtual bool	DisplayCycle(LPGDIDevice dev);

	virtual bool	Render(LPGDIDevice dev);
	virtual void    UpdateRects();

	void            SetTrackRange(int nStart, int nEnd);
	int             GetTrackPos()
	{
		return nPosition;
	}
	void            SetTrackPos(int nPosition)
	{
		this->nPosition = nPosition; Cap(); UpdateThumbRect();
	}
	int             GetPageSize()
	{
		return nPageSize;
	}
	void            SetPageSize(int nPageSize)
	{
		this->nPageSize = nPageSize; Cap(); UpdateThumbRect();
		if (!picThumb)
			picThumb = new memPic();
		picThumb->LoadImg(L"ScrollBarThumb.png", ROUND(width), HEIGHTOF(rcThumb));
	}

	void            Scroll(int nDelta);    // Scroll by nDelta items (plus or minus)
	void            ShowItem(int nIndex);  // Ensure that item nIndex is displayed, scroll if necessary

protected:
	virtual void    UpdateThumbRect();
	void            Cap();  // Clips position at boundaries. Ensures it stays within legal range.

	int nItems;			// 选项数目
	int nStart;			// First item
	int nEnd;			// The index after the last item
	int nPosition;		// Position of the first displayed item
	int nPageSize;		// How many items are displayable in one page
	RECT rcUpButton;	// 上按钮区域
	RECT rcDownButton;	// 下按钮区域
	RECT rcTrack;		// 轨道区域
	RECT rcThumb;		// 滑块区域
	POINT lastMouse;	// Last mouse position

	bool bShowThumb;	// 是否显示滑块
	bool bDrag;			// 是否在拖动滑块状态
};

struct GUIComboBoxItem
{
	WCHAR strText[256];
	void* pData;

	RECT rcActive;
	bool bVisible;
};

class GDIGUIComboBox : public GDIGUIControl
{
public:
	memPic *picDropDown;
	memPic *picButton;

	GDIGUIComboBox();
	virtual         ~GDIGUIComboBox();

	void InitPic();
	virtual byte    HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	//virtual bool	DisplayCycle(LPGDIDevice dev);

	virtual bool    Render(LPGDIDevice dev);
	virtual void    UpdateRects();

	HRESULT         AddItem(const WCHAR* strText, void* pData);
	void            RemoveAllItems();
	void            RemoveItem(UINT index);
	bool            ContainsItem(const WCHAR* strText, UINT iStart = 0);
	int             FindItem(const WCHAR* strText, UINT iStart = 0);
	//void* GetItemData(const WCHAR* strText);
	//void* GetItemData(int nIndex);
	void            SetDropHeight(UINT nHeight)
	{
		nDropHeight = nHeight; UpdateRects();
	}
	int             GetScrollBarWidth() const
	{
		return nSBWidth;
	}
	void            SetScrollBarWidth(int nWidth)
	{
		nSBWidth = nWidth; UpdateRects();
	}

	int             GetSelectedIndex() const
	{
		return iSelected;
	}
	void			SetSelectedIndex(int sel)
	{
		iSelected = sel;
		iFocused = iSelected;
		scrollBar.SetTrackPos(iSelected);
	}
	//void* GetSelectedData();
	//GUIComboBoxItem* GetSelectedItem();

	UINT            GetNumItems()
	{
		return items.GetSize();
	}
	GUIComboBoxItem* GetItem(UINT index)
	{
		return items.GetAt(index);
	}

	//HRESULT         SetSelectedByIndex(UINT index);
	//HRESULT         SetSelectedByText(const WCHAR* strText);
	//HRESULT         SetSelectedByData(void* pData);

	//自己加的
	const RECT		&GetTextRect() const
	{
		return rcText;
	}
	const RECT		&GetButtonRect() const
	{
		return rcButton;
	}
	const RECT		&GetDropDownRect() const
	{
		return rcDropdown;
	}

protected:
	int iSelected;
	int iFocused;
	int nRowHeight;//自己加的，行高
	int nDropHeight;
	GDIGUIScrollBar scrollBar;
	int nSBWidth;

	bool lastOpend;
	bool bOpened;

	RECT rcText;
	RECT rcButton;
	RECT rcDropdown;
	RECT rcDropdownText;

	CGrowableArray <GUIComboBoxItem*> items;
};


#define FOCUS_CONTROL_GDI(i)		{ pControlFocus = controls[i]; }
#define BLOCK_ON_CONTROL_GDI(i)		{ pControlFocus = controls[i];	bBlock = true; }
#define FOCUS_OFF_GDI				{ pControlFocus = NULL; }
#define BLOCK_OFF_GDI				{ bBlock = false; }
#define ISFOCUS_GDI(i)				( controls[i] == pControlFocus )

typedef void (CALLBACK *LPGDIGUIEVENTCALLBACK)(int ID, WPARAM wp, GDIGUIControl *pControl);

struct threadParam
{
	void *pGUISys;
};

class CGDIGUISystem
{
private:
	LPGDIDevice device;
	UINT *pBufW, *pBufH;						// link， 实时backbuffer尺寸

	CFont *fontDefault;							// 默认字体
	CFont **fonts;								// 字体对象链表
	CGrowableArray <GDIGUIControl*> controls;	// GUI控件列表

	CGrowableArray <GDIGUIControl*> backdrop;	// 背景图控件
	bool bUseBackdrop;

	int nFonts;									// 字体总数
	int nControls;								// 控件总数

	GDIGUIControl *pControlFocus;				// 焦点
	bool bBlock;								// 是否阻塞 

	LPGDIGUIEVENTCALLBACK pEventProc;			// 事件回调

	bool bInvalidate;

	DWORD threadID;
	HANDLE hThreadGUI;
	static void		ThreadGUI(LPVOID lpParam);	// GUI渐变显示效果线程

protected:
	void			VarInit();					// 参数初始化
	bool			AddControl(GDIGUIControl *pControl);
	bool			AddBack(GDIGUIBack *pControl);

public:
	static int countLoop;
	static float time;							// 内部计时

	CGDIGUISystem();
	CGDIGUISystem(LPGDIDevice dev);
	~CGDIGUISystem();

	void			BeginGUIThread();			// 启动GUI渐变显示效果线程
												//bool			Bind(CWnd *pWnd);			// 捆绑窗口
	void			ShutDown();

	CFont			*GetFont(int ID) const;
	bool			SetControlText(int ID, WCHAR *strText);// 设置控件文本

														   // 添加字体
	bool AddFont(WCHAR *fontName, int *fontID, INT Height = 0, INT Width = 0, INT Weight = 0
		, BYTE Quality = DEFAULT_QUALITY, int Escapement = 0, int Orientation = 0
		, bool Italics = false, bool Underline = false, bool StrikeOut = false
		, BYTE Charset = DEFAULT_CHARSET, BYTE OutputPrecision = OUT_DEFAULT_PRECIS
		, BYTE ClipPrecision = CLIP_DEFAULT_PRECIS
		, BYTE PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE);// 交换了参数次序

	bool AddStatic(int ID, float x, float y
		, float width = GUI_DEFAULT_STATICWIDTH, float height = GUI_DEFAULT_STATICHEIGHT
		, WCHAR *strText = L"", DWORD color = COLORGDI_WHITE, int fontID = 0
		, WCHAR *file = NULL, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);
	bool AddBackdrop(int ID, float x, float y
		, float width = GUI_DEFAULT_BUTTONWIDTH, float height = GUI_DEFAULT_BUTTONHEIGHT
		, WCHAR *file = NULL, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);
	bool AddButton(int ID, float x, float y
		, float width = GUI_DEFAULT_BUTTONWIDTH, float height = GUI_DEFAULT_BUTTONHEIGHT
		, WCHAR *strText = L"", DWORD color = COLORGDI_BLACK, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *up = L"btn1.png", WCHAR *on = L"btn2.png", WCHAR *down = L"btn3.png");
	bool AddWave(int ID, float x, float y
		, float width = 300, float height = 100
		, DWORD color = COLORGDI_WHITE, DWORD lineColor = COLORGDI_DARKGREY
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL, int fontID = 0);
	bool AddTristate(int ID, float x, float y
		, float width = 10, float height = 10
		, WCHAR *strText = L"", DWORD color = COLORGDI_BLACK, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *normal = L"drip2.png", WCHAR *on = L"drip2g.png", WCHAR *off = L"drip2o.png");
	bool AddSwitch(int ID, float x, float y
		, float width = 10, float height = 10
		, WCHAR *strText = L"", DWORD color = COLORGDI_BLACK, int fontID = 0, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL
		, WCHAR *normal = L"circlen.png", WCHAR *on = L"circlef.png");
	bool AddScrollBar(int ID, float x, float y
		, float width = 10, float height = 10
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);//测试用
	bool AddComboBox(int ID, float x, float y
		, float width = 100, float height = 20
		, GUI_WINDOCK_MODE dock = GUI_WINDOCK_NORMAL);//测试用

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
	GDIGUIComboBox*	GetComboBox(int ID) const
	{
		return (GDIGUIComboBox*)GetControl(ID, GUI_CONTROL_COMBOBOX);
	}
	GDIGUISwitch*	GetSwitch(int ID) const
	{
		return (GDIGUISwitch*)GetControl(ID, GUI_CONTROL_SWITCH);
	}

	// 设置回调函数
	// 事件处理
	void			SetCallbackEvent(LPGDIGUIEVENTCALLBACK pevent);
	int				HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	void			HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void			HandleCMD(int ID, UINT cmd);

	void			Invalidate();
	void			Render();
};

#endif // USE_GDIGUI