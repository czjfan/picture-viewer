#pragma once

// Types of controls we support.
#define UGP_GUI_STATICTEXT 1     //��̬�ı�
#define UGP_GUI_BUTTON     2     // ��ť
#define UGP_GUI_BACKDROP   3     //����ͼ

// Mouse button states.
#define UGP_BUTTON_UP      1    
#define UGP_BUTTON_OVER    2
#define UGP_BUTTON_DOWN    3

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// A structure for our custom vertex type
struct stGUIVertex
{
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) 

struct stGUIControl
{
	//�ؼ�����
	int m_type;
	
	//�ؼ�id
	int m_id;

	//�ؼ���ɫ
	unsigned long m_color;

	// ����ؼ������Ǿ�̬�ı�������GUISystem�����������е������������Ƕ��㻺�������е�������
	int m_listID;

	//�ؼ����Ͻǵ�����λ��
	float m_xPos, m_yPos;

	// ��ť�Ŀ�Ⱥ͸߶ȣ����԰�ť��Ч����̬�ı�������ͼʱ��Ч��
	float m_width, m_height;

	// ��̬�ı����������ݣ����Ծ�̬�ı���Ч��
	WCHAR *m_text;
	
	// ����ͼ�õ����� �����Ա���ͼ��Ч��
	LPDIRECT3DTEXTURE9 m_backDrop;

	// ��ť���𣬰��£�����ƶ�����ť��ʱ�ֱ���ʾ��ͼƬ�����԰�ť��Ч��
	LPDIRECT3DTEXTURE9 m_upTex, m_downTex, m_overTex;
};

class CD3DGUISystem
{
public:
	CD3DGUISystem(LPDIRECT3DDEVICE9 device, int w, int h);
	~CD3DGUISystem() { Shutdown(); }

	void OnLost();
	void OnReset(LPDIRECT3DDEVICE9 device);
	// ��������
	// @fontName �´������������
	// @size     �´����������Ĵ�С
	// @fontID   �´��������IDָ��
	bool CreateFont(WCHAR *fontName, int *fontID
		, INT Height = 0, INT Width = 0, INT Weight = 0
		, DWORD Quality = DEFAULT_QUALITY//�����˴���
		, UINT MipLevels = 1, bool Italics = false
		, DWORD Charset = DEFAULT_CHARSET, DWORD OutputPrecision = OUT_DEFAULT_PRECIS
		, DWORD PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE
		);

	// �������ڱ��汳��ͼ������״�Ķ��㻺��,������Ҫӳ�䵽�����ϵ�����ͼ��
	// @fileName Ҫ���ص�����ͼ���ļ���
	bool AddBackdrop(WCHAR *fileName);
	
	// ��Ӿ�̬�ı��ؼ�
	// @id �ؼ�ID
	// @text ��Ҫ��ʾ���ı�����
	// @x   �ı�����ʼx����
	// @y   �ı�����ʼy����
	// @color �ı���ɫ
	// @fontID  �ı�Ҫ���õ������id�������������е�������
	bool AddStaticText(int id, WCHAR *text, float x, float y, unsigned long color, int fontID);
	
	// ��Ӱ�ť�ؼ�
	// @id �ؼ�id
	// @x  ���Ͻǵ�x����
	// @y  ���Ͻǵ�y����
	// @up  ��ť����ʱ��ʾ������
	// @over ����ڰ�ť��ʱ��ʾ������
	// @down ��ť����ʱ��ʾ������
	bool AddButton(int id, float x, float y, WCHAR *up, WCHAR *over, WCHAR *down);
	
	void Shutdown();
	
	LPD3DXFONT GetFont(int id)
	{
		if (id < 0 || id >= m_totalFonts) return NULL;
		return m_fonts[id];
	}
	
	stGUIControl *GetGUIControl(int id)
	{
		if (id < 0 || id >= m_totalControls) return NULL;
		return &m_controls[id];
	}
	
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(int id)
	{
		if (id < 0 || id >= m_totalBuffers) return NULL;
		return m_vertexBuffers[id];
	}
	
	int GetTotalFonts() { return m_totalFonts; }

	int GetTotalControls() { return m_totalControls; }

	int GetTotalBuffers() { return m_totalBuffers; }

	int GetWindowWidth() { return m_windowWidth; }

	int GetWindowHeight() { return m_windowHeight; }

	LPDIRECT3DDEVICE9 GetDevice() { return m_device; }

	stGUIControl *GetBackDrop() { return &m_backDrop; }

	LPDIRECT3DVERTEXBUFFER9 GetBackDropBuffer() { return m_backDropBuffer; }

	bool UseBackDrop() { return m_useBackDrop; }

	void SetWindowSize(int w, int h) { m_windowWidth = w; m_windowHeight = h; }
		
private:

	// D3D�豸,���ڴ���Direct3D������󡢶��㻺�������
	LPDIRECT3DDEVICE9 m_device;
	
	// Direct3D�����������
	LPD3DXFONT *m_fonts;
	
	// GUI�ؼ���������
	stGUIControl *m_controls;

	//���㻺��������Direct3D���㻺���������,������洢ϵͳ���õ���ȫ����ť�Ķ��㻺��.
	LPDIRECT3DVERTEXBUFFER9 *m_vertexBuffers;
	
	//����ͼ���Դ洢����ͼ��Ϣ������ֻ��һ������ͼ����˽���ӿؼ������зֳ�����
	stGUIControl m_backDrop;

	//����ͼ�Ķ��㻺�����ڴ洢ȫ�������ķ��ε����������ݡ�
	LPDIRECT3DVERTEXBUFFER9 m_backDropBuffer;
	
	// �Ƿ�ʹ�ñ���ͼ
	bool m_useBackDrop;
	
	// ��������
	int m_totalFonts;
	
	// �ؼ�����
	int m_totalControls;
	
	// ���㻺������
	int m_totalBuffers;
	
	// ���ڿ�ȡ��߶�
	int m_windowWidth;
	int m_windowHeight;

};

// @gui ��Ҫ�����GUI
// @LMBDown ����Ƕ�������
// @mouseX ������Ϊ��λ��ָ���x����
// @mouseY ������Ϊ��λ��ָ���y����
// @funcPtr �ص�����ָ��
// @id      �ص�������Ҫ�Ŀؼ�id
// @state   �ص�������Ҫ�Ŀؼ�״̬
void ProcessGUI(CD3DGUISystem *gui, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state));
