#include "stdafx.h"
#include "gui.h"

LARGE_INTEGER CGUIControl::frequency = { 0 };

CD3DGUISystem::CD3DGUISystem()
{
	device = NULL;
	pbufferw = NULL;
	pbufferh = NULL;

	defaultfont = NULL;
	fonts = NULL;
	controls = NULL;
	backdrop = NULL;
	bUseBackdrop = false;

	nFonts = 0;
	nControls = 0;
	
	lastLMBdown = false;
	pFocusControl = NULL;
	block = false;

	pEventProc = NULL;
}

CD3DGUISystem::CD3DGUISystem(LPDIRECT3DDEVICE9 dev)
{
	device = dev;
	pbufferw = NULL;
	pbufferh = NULL;

	defaultfont = NULL;
	fonts = NULL;
	controls = NULL;
	backdrop = NULL;
	bUseBackdrop = true;

	nFonts = 0;
	nControls = 0;

	lastLMBdown = false;
	pFocusControl = NULL;
	block = false;

	pEventProc = NULL;

	if (device)
	{
		HRESULT hr = D3DXCreateFontW(
			device,
			16, 0, 1, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLEARTYPE_NATURAL_QUALITY,	//CLEARTYPE_NATURAL_QUALITY, DEFAULT_QUALITY
			DEFAULT_PITCH | FF_DONTCARE,
			L"Tw Cen MT",
			&defaultfont);
	}
}

CD3DGUISystem::~CD3DGUISystem() 
{
	Shutdown();
}

void CD3DGUISystem::OnLostDevice()
{
	for (int i = 0; i < nControls; i++)
	{
		// ����
		defaultfont->OnLostDevice();
		for (int i = 0;i < nFonts; i++)
		{
			if (fonts[i])
			{
				fonts[i]->OnLostDevice();
			}
		}

		// Take action depending on what type it is.
		switch (controls[i]->type)
		{
		case GUI_CONTROL_STATICTEXT:
			break;
		case GUI_CONTROL_BUTTON:
			break;
		}
	}
}

void CD3DGUISystem::OnResetDevice()
{
	for (int i = 0; i < nControls; i++)
	{
		// ����
		defaultfont->OnResetDevice();
		for (int i = 0; i < nFonts; i++)
		{
			if (fonts[i])
			{
				fonts[i]->OnResetDevice();
			}
		}

		controls[i]->Dock(pbufferw, pbufferh);// �ؼ�����ͣ��
		switch (controls[i]->type)
		{
		case GUI_CONTROL_STATICTEXT:
			break;
		case GUI_CONTROL_BUTTON:
		case GUI_CONTROL_EDIT:
			controls[i]->RefreshVertexBuffer();//ˢ��ģ��
			break;
		}
	}
}

bool CD3DGUISystem::CreateDXFont(WCHAR *fontName, int *fontID
	, INT Height, INT Width, INT Weight
	, DWORD Quality
	, UINT MipLevels, bool Italics
	, DWORD Charset, DWORD OutputPrecision
	, DWORD PitchAndFamily)
{
	if (!device) return false;

	if (!fonts)
	{
		fonts = new LPD3DXFONT[1];
		if (!fonts) return false;
	}
	else
	{
		LPD3DXFONT *temp;
		temp = new LPD3DXFONT[nFonts + 1];
		if (!temp) return false;
		memcpy(temp, fonts, sizeof(LPD3DXFONT) * nFonts);
		delete[] fonts;
		fonts = temp;
	}

	if (FAILED(D3DXCreateFontW(device,
		Height, Width, Weight, MipLevels, Italics,
		Charset, OutputPrecision, Quality,
		PitchAndFamily, fontName,
		&fonts[nFonts]))) 
		return false;

	if (!fonts[nFonts]) 
		return false;

	if (fontID)
		*fontID = nFonts + 1;

	nFonts++;

	return true;
}

bool CD3DGUISystem::PreAddControl()
{
	if (!controls)
	{
		controls = new D3DGUIControl*[1];
		if (!controls) return false;
		controls[0] = new D3DGUIControl;
	}
	else
	{
		D3DGUIControl **temp;
		temp = new D3DGUIControl*[nControls + 1];
		if (!temp) return false;
		for (int i = 0; i < nControls; i++)
		{
			temp[i] = controls[i];
		}
		delete[] controls;
		controls = temp;
		controls[nControls] = new D3DGUIControl;
	}

	return true;
}

bool CD3DGUISystem::AddBackdrop(WCHAR *TexFileName, int width, int height)
{
	if (!TexFileName) 
		return false;

	if (backdrop)
		SAFE_RELEASEDELETE(backdrop)
	backdrop = new D3DGUIControl;

	backdrop->type = GUI_CONTROL_BACKDROP;

	// Load the texture image from file.
	if (D3DXCreateTextureFromFile(device, TexFileName, &backdrop->m_overTex) != D3D_OK)
		return false;

	// default width/height
	float w = width > 0 ? width : (pbufferw ? *pbufferw : (float)800);
	float h = height > 0 ? height : (pbufferh ? *pbufferh : (float)600);

	D3DGUIVertex obj[] =
	{
		{ w, 0.0f, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f },
		{ w, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f },
		{ 0.0f, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f },
	};
	
	// Create the vertex buffer.
	if (FAILED(device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_SYSTEMMEM, &backdrop->vbuffer, NULL))) return false;
	
	// Fill the vertex buffer.
	void *ptr;
	if (FAILED(backdrop->vbuffer->Lock(0, sizeof(obj), (void**)&ptr, 0))) return false;
	memcpy(ptr, obj, sizeof(obj));
	backdrop->vbuffer->Unlock();

	bUseBackdrop = true;

	return true;
}

//������һ������ʹ����ͬ��ͬ�������ľ�̬�ı���
//�Դ󲿷־�̬�ı����ԣ����ܽ�ʹ����ͬ��������󣬵�Ҳ���Խ���������������ʹ�á�
bool CD3DGUISystem::AddStaticText(int ID, WCHAR *text, int x, int y, DWORD color, int fontID, int width, int height, byte dock)
{
	if (!text) return false;

	FALSE_RETURN_FALSE(PreAddControl());

	// Fill it with all the info we need for static text.
	controls[nControls]->type = GUI_CONTROL_STATICTEXT;
	controls[nControls]->ID = ID;

	controls[nControls]->color = color;
	controls[nControls]->dockmode = dock;
	controls[nControls]->dockX = x;
	controls[nControls]->dockY = y;
	controls[nControls]->width = width;
	controls[nControls]->height = height;
	controls[nControls]->Dock(pbufferw, pbufferh);

	controls[nControls]->font = (fontID > 0 && GetFont(fontID - 1))? GetFont(fontID - 1) : defaultfont;
	
	// Copy text data.
	mywcscpy(&controls[nControls]->text, text);
	
	// Increment total.
	nControls++;
	return true;
}

D3DGUIControl::D3DGUIControl()
{
	vbuffer = NULL;

	m_outTex = NULL;
	m_downTex = NULL;
	m_overTex = NULL;
	overdx = -1;
	overdy = -2;
	downdx = 1;
	downdy = 1;
}

D3DGUIControl::~D3DGUIControl()
{
	Release();// ����ERROR!

	CGUIControl::~CGUIControl();
}

bool D3DGUIControl::Release()
{
	SAFE_RELEASE(vbuffer)

	SAFE_RELEASE(m_outTex)
	SAFE_RELEASE(m_downTex)
	SAFE_RELEASE(m_overTex)

	CGUIControl::Release();

	return true;
}

void D3DGUIControl::Translation(int dx, int dy)
{
	CGUIControl::Translation(dx, dy);

	RefreshVertexBuffer();
	//if (!vbuffer) 
	//	return;

	//D3DVERTEXBUFFER_DESC vbd;
	//vbuffer->GetDesc(&vbd);

	//void *ptr;
	//if (FAILED(vbuffer->Lock(0, vbd.Size, (void**)&ptr, 0)))
	//	return;
	//*(float*)((byte*)ptr) += dx;//������
	//*(float*)((byte*)ptr + sizeof(float)) += dy;
	//*(float*)((byte*)ptr + sizeof(D3DGUIVertex)) += dx;
	//*(float*)((byte*)ptr + sizeof(D3DGUIVertex) + sizeof(float)) += dy;
	//*(float*)((byte*)ptr + 2 * sizeof(D3DGUIVertex)) += dx;
	//*(float*)((byte*)ptr + 2 * sizeof(D3DGUIVertex) + sizeof(float)) += dy;
	//*(float*)((byte*)ptr + 3 * sizeof(D3DGUIVertex)) += dx;
	//*(float*)((byte*)ptr + 3 * sizeof(D3DGUIVertex) + sizeof(float)) += dy;
	//vbuffer->Unlock();
}

void D3DGUIControl::KillFocus()
{
	CGUIControl::KillFocus();

	//Translation(0 - displaydx, 0 - displaydy);
}

bool D3DGUIControl::RefreshVertexBuffer()
{
	if (vbuffer)
	{
		D3DGUIVertex obj[] =
		{
			{ (float)width + posX + displaydx, (float)posY + displaydy, 0.0f, 1.0f, color, 1.0f, 0.0f },
			{ (float)width + posX + displaydx, (float)height + posY + displaydy, 0.0f, 1.0f, color, 1.0f, 1.0f },
			{ (float)posX + displaydx, (float)posY + displaydy, 0.0f, 1.0f, color, 0.0f, 0.0f },
			{ (float)posX + displaydx, (float)height + posY + displaydy, 0.0f, 1.0f, color, 0.0f, 1.0f },
		};
		// Fill the vertex buffer.
		void *ptr;
		if (FAILED(vbuffer->Lock(0, sizeof(obj), (void**)&ptr, 0)))
		{
			return false;
		}
		memcpy(ptr, obj, sizeof(obj));
		vbuffer->Unlock();

		return true;
	}
}

void ErrorShow(HRESULT hr, WCHAR * msg, HWND hwnd, WCHAR * title)
{
	WCHAR errormsg[256] = { 0 };
	if (msg)
	{
		wcscpy_s(errormsg, msg);
		wcscat_s(errormsg, L": ");
	}

	switch (hr)
	{
	case E_OUTOFMEMORY:
		wcscat_s(errormsg, L"OUT OF MEMORY.");
		break;
	case E_FAIL:
		wcscat_s(errormsg, L"E_FAIL");
		break;
	case D3DERR_INVALIDCALL:
		wcscat_s(errormsg, L"D3DERR_INVALIDCALL");
		break;
	case 0x88760B59:
		wcscat_s(errormsg, L"0x88760B59");
		break;
	default:
		wcscat_s(errormsg, L"UNKNOWN");
		break;
	}
	MessageBoxW(hwnd, errormsg, title, MB_OK | MB_APPLMODAL);
}

void ChangeCtrlAlpha(LPDIRECT3DVERTEXBUFFER9 pbuf, byte alpha)
{
	if (!pbuf)
		return;

	D3DVERTEXBUFFER_DESC vbd;
	pbuf->GetDesc(&vbd);

	void *ptr;
	if (FAILED(pbuf->Lock(0, vbd.Size, (void**)&ptr, 0)))
		return;
	*((byte*)ptr + sizeof(D3DXVECTOR4) + 3) = alpha;
	*((byte*)ptr + sizeof(D3DGUIVertex) + sizeof(D3DXVECTOR4) + 3) = alpha;
	*((byte*)ptr + 2 * sizeof(D3DGUIVertex) + sizeof(D3DXVECTOR4) + 3) = alpha;
	*((byte*)ptr + 3 * sizeof(D3DGUIVertex) + sizeof(D3DXVECTOR4) + 3) = alpha;

	pbuf->Unlock();
}

bool mywcscpy(WCHAR ** dest, WCHAR * src)
{
	if (!dest || !src)
		return false;

	int len = wcslen(src);
	(*dest) = new WCHAR[len + 1];
	if (!*dest) 
		return false;
	memcpy((*dest), src, len * 2);
	(*dest)[len] = (WCHAR)'\0';
}

bool CD3DGUISystem::AddButton(int ID, int x, int y, int width, int height, WCHAR strText[], int fontID, byte dock, WCHAR *up, WCHAR *over, WCHAR *down)
{
	if (!device || !up || !over || !down) return false;

	FALSE_RETURN_FALSE(PreAddControl());

	controls[nControls]->type = GUI_CONTROL_BUTTON;
	controls[nControls]->ID = ID;

	controls[nControls]->color = COLOR_WHITE;// ����vertexbuffer��
	controls[nControls]->dockmode = dock;
	controls[nControls]->dockX = x;
	controls[nControls]->dockY = y;
	controls[nControls]->width = width;
	controls[nControls]->height = height;
	controls[nControls]->Dock(pbufferw, pbufferh);
	controls[nControls]->displaydx = 0;
	controls[nControls]->displaydy = 0;

	strText ? mywcscpy(&controls[nControls]->text, strText)
		: controls[nControls]->text = NULL;
	controls[nControls]->textcolor = 0xFFFFFFFF; 
	controls[nControls]->font = (fontID > 0 && GetFont(fontID - 1)) ? GetFont(fontID - 1) : defaultfont;
	
	controls[nControls]->overdx = -1;
	controls[nControls]->overdy = -1;
	controls[nControls]->downdx = 1;
	controls[nControls]->downdy = 1;
	controls[nControls]->state = GUI_STATE_OUT;
	controls[nControls]->alpha = 0;
	controls[nControls]->displayevent = GUI_EVENT_NULL;
	
	// Load the texture image from file.
	FAILED_RETURN_FALSE(D3DXCreateTextureFromFile(device, up, &controls[nControls]->m_outTex))

	FAILED_RETURN_FALSE(D3DXCreateTextureFromFile(device, over, &controls[nControls]->m_overTex))
	
	FAILED_RETURN_FALSE(D3DXCreateTextureFromFile(device, down, &controls[nControls]->m_downTex))
	
	// Create the vertex buffer.
	if (FAILED(device->CreateVertexBuffer(sizeof(D3DGUIVertex)*4, 0,
		D3DFVF_GUI, D3DPOOL_MANAGED, &controls[nControls]->vbuffer, NULL)))
	{
		return false;
	}
	controls[nControls]->RefreshVertexBuffer();
	
	// Increase.
	nControls++;
	
	return true;
}

bool CD3DGUISystem::ADDTextInput(int ID, int x, int y, int width, int height, int fontID, byte dock, DWORD color, DWORD txtcolor)
{
	if (!device) return false;

	FALSE_RETURN_FALSE(PreAddControl());

	controls[nControls]->type = GUI_CONTROL_EDIT;
	controls[nControls]->ID = ID;

	controls[nControls]->color = color;
	controls[nControls]->dockmode = dock;
	controls[nControls]->dockX = x;
	controls[nControls]->dockY = y;
	controls[nControls]->width = width;
	controls[nControls]->height = height;
	controls[nControls]->Dock(pbufferw, pbufferh);
	controls[nControls]->displaydx = 0;
	controls[nControls]->displaydy = 0;

	controls[nControls]->text = new WCHAR[GUI_DEFAULTTEXTLEN + 1];
	controls[nControls]->text[0] = L'\0';
	controls[nControls]->txtpos = 0;
	controls[nControls]->textcolor = txtcolor;
	controls[nControls]->font = (fontID > 0 && GetFont(fontID - 1)) ? GetFont(fontID - 1) : defaultfont;

	controls[nControls]->overdx = -1;
	controls[nControls]->overdy = -1;
	controls[nControls]->downdx = 1;
	controls[nControls]->downdy = 1;
	controls[nControls]->state = GUI_STATE_OUT;
	controls[nControls]->alpha = 0;
	controls[nControls]->displayevent = GUI_EVENT_NULL;

	// Create the vertex buffer.
	if (FAILED(device->CreateVertexBuffer(sizeof(D3DGUIVertex) * 4, 0,
		D3DFVF_GUI, D3DPOOL_SYSTEMMEM, &controls[nControls]->vbuffer, NULL)))
	{
		return false;
	}
	controls[nControls]->RefreshVertexBuffer();

	// Increase.
	nControls++;

	return true;
}

bool CD3DGUISystem::DropControl(int ID)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			//delete controls[i];
			for (int j = i; j < nControls-1; j++)
			{
				controls[j] = controls[j + 1];
			}
			D3DGUIControl **temp = new D3DGUIControl*[nControls - 1];
			for (int j = 0; j < nControls - 1; j++)
				temp[j] = controls[j];
			delete[] controls;
			controls = temp;
			nControls--;
		}
	}
	return false;
}

int CD3DGUISystem::GetState(int ID)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			return controls[i]->state;
		}
	}
	return -1;
}

bool CD3DGUISystem::SetControlText(int ID, WCHAR * text)
{
	NULL_RETURN_FALSE(text)

	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			static int len = wcslen(text);
			if (!controls[i]->text)
			{
				controls[i]->text = new WCHAR[len + 1];
			}
			else if ((int)wcslen(controls[i]->text) <= len)
			{
				delete[] controls[i]->text;
				controls[i]->text = new WCHAR[len + 1];
			}
			memcpy(controls[i]->text, text, len * 2);
			controls[i]->text[len] = (WCHAR)'\0';

			controls[i]->txtpos = len;

			return true;
		}
	}
	return false;
}

bool CD3DGUISystem::SetControlVisible(int ID, bool bVisible)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			controls[i]->SetVisible(bVisible);

			if (ISFOCUS(i) && !bVisible)
			{
				BLOCK_OFF;
				FOCUS_OFF;
			}

			return true;
		}
	}
	return false;
}

bool CD3DGUISystem::SetControlEnabled(int ID, bool bEnabled)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			controls[i]->SetEnabled(bEnabled);

			if (ISFOCUS(i) && !bEnabled)
			{
				BLOCK_OFF;
				FOCUS_OFF;
			}

			return true;
		}
	}
}

LPD3DXFONT CD3DGUISystem::GetFont(int ID)
{
	if (ID < 0 || ID >= nFonts) return NULL;
	return fonts[ID];
}

bool CD3DGUISystem::SetFocus(int ID)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->ID == ID)
		{
			if (pFocusControl)
			{
				pFocusControl->KillFocus();
			}

			block = false;
			FOCUS_CONTROL(i);

			return true;
		}
	}
	return false;
}

bool CD3DGUISystem::Bind(D3DWnd * pd3dwnd)
{
	if (pd3dwnd)
	{
		pbufferw = pd3dwnd->GetPBufferWidth();
		pbufferh = pd3dwnd->GetPBufferHeight();

		return true;
	}
	else
		return false;
}

void CD3DGUISystem::Shutdown()
{
	// �������
	for (int i = 0; i < nFonts; i++)
	{
		SAFE_RELEASE(fonts[i])
	}
	SAFE_DELETE_LIST(fonts)
	nFonts = 0;

	// �����ؼ����
	SAFE_RELEASEDELETE(backdrop)

	// �ؼ��б����
	for (int i = 0; i < nControls; i++)
	{
		SAFE_DELETE(controls[i])
	}
	SAFE_DELETE_LIST(controls)
	nControls = 0;
}

void CD3DGUISystem::SetEventProc(LPGUIEVENTCALLBACK pevent)
{
	pEventProc = pevent;
}

void CD3DGUISystem::HandleMouse(bool LMBDown, LONG mouseX, LONG mouseY)
{
	byte LMBstate = (byte)lastLMBdown + (byte)(LMBDown << 1);
	lastLMBdown = LMBDown;
	bool controldown = false;//�Ƿ��пؼ�����
	for (int i = 0; i < nControls; i++)
	{
		if (!block || ISFOCUS(i))//���������ߵ�i���ؼ����ǽ���ؼ�
		{
			if (!controls[i]->bDisabled && controls[i]->bVisible)// �ؼ�����ʹ��״̬
			{
				int event = controls[i]->HandleMouse(LMBstate, mouseX, mouseY);

				// �ؼ�ϵͳ����
				if (event == GUI_EVENT_DOWN)//���µİ�ť����������ť
				{
					if (controls[i]->type == GUI_CONTROL_BUTTON)
					{
						controldown = true;
						BLOCK_ON_CONTROL(i)
					}
					else if (controls[i]->type == GUI_CONTROL_EDIT)
					{
						controldown = true;
						FOCUS_CONTROL(i)
					}
				}
				else if (event == GUI_EVENT_OUTUP || event == GUI_EVENT_UP)
					BLOCK_OFF;

				if (event && pEventProc)//�ص�
				{
					pEventProc(controls[i]->ID, event, 0);
				}
			}
		}
	}
}

void CD3DGUISystem::HandleKeyboard(UINT8 keytype, WPARAM wParam)
{
	for (int i = 0; i < nControls; i++)
	{
		if (controls[i]->type == GUI_CONTROL_EDIT && ISFOCUS(i))
		{
			if (!controls[i]->bDisabled && controls[i]->bVisible)
			{
				bool cmd = controls[i]->HandleInput(wParam, keytype == GUI_KEYEVENT_IMECHAR);

				if (cmd && pEventProc)
				{
					pEventProc(controls[i]->ID, GUI_EVENT_CMD, (LPARAM)controls[i]->text);
					SetControlText(controls[i]->ID, L"");
				}
			}
		}
	}
}

void CD3DGUISystem::RenderBack()
{
	if (bUseBackdrop)
	{
		if (backdrop)
		{
			backdrop->Render(device);
		}
	}
}

void CD3DGUISystem::Render()
{
	if (!device)
		return;

	// ������ƿؼ�
	for (int i = 0; i < nControls; i++)
	{
		controls[i]->Render(device);
	}
}

bool D3DGUIControl::Render(LPDIRECT3DDEVICE9 dev)
{
	if (!dev || !bVisible)
		return false;

	// ��ʱ
	LARGE_INTEGER tick = { 0 };
	QueryPerformanceCounter(&tick);
	const float speed = 1.6f;

	switch (type)
	{
	case GUI_CONTROL_STATICTEXT:
		if (font && text) 
			font->DrawTextW(NULL, text, -1, &textrect, DT_LEFT | DT_NOCLIP, color);

		break;
	case GUI_CONTROL_BUTTON:
		if (!vbuffer)
			return false;

		// �ײ����
		dev->SetTexture(0, m_outTex);
		D3DGUI_RENDER_BUTTON(dev, vbuffer);

		// �ڶ�ͼ�����
		// ���ö��̴߳�����
		if (displayevent == GUI_EVENT_ENTER)
		{
			if (alpha != 255)
				alpha += (byte)min(255 - (int)alpha, 1000.0f*(tick.QuadPart - lasttick.QuadPart) / frequency.QuadPart *speed);
			else
				displayevent = GUI_EVENT_NULL;
		}
		else if (displayevent == GUI_EVENT_LEAVE)
		{
			if (alpha)
				alpha -= (byte)min((int)alpha, 1000.0f*(tick.QuadPart - lasttick.QuadPart) / frequency.QuadPart *speed);
			else
				displayevent = GUI_EVENT_NULL;
		}
		else if (displayevent == GUI_EVENT_OUTUP)
		{
			if (alpha)
				alpha -= (byte)min((int)alpha, 1000.0f*(tick.QuadPart - lasttick.QuadPart) / frequency.QuadPart *speed);
			else
				displayevent = GUI_EVENT_NULL;
		}
		else if (displayevent == GUI_EVENT_UP)
		{
			if (alpha != 255)
				alpha += (byte)min(255 - (int)alpha, 1000.0f*(tick.QuadPart - lasttick.QuadPart) / frequency.QuadPart*speed);
			else
				displayevent = GUI_EVENT_NULL;
		}
		else if (displayevent == GUI_EVENT_DOWN)
		{
			alpha = 255;
		}

		// ���ö�������
		if (displayevent == GUI_EVENT_OUTUP
			|| displayevent == GUI_EVENT_DOWN)
			dev->SetTexture(0, m_downTex);
		else
			dev->SetTexture(0, m_overTex);

		// ���Ƶڶ�ͼ��
		ChangeCtrlAlpha(vbuffer, alpha);//����ʵʱ͸����
		D3DGUI_RENDER_BUTTON(dev, vbuffer);
		ChangeCtrlAlpha(vbuffer, 255);


		// ��ť�ı�����
		if (text && font)
		{
			RECT textregion = { posX + displaydx, posY + displaydy - 1 //-1����
				, posX + displaydx + width + 2
				, posY + displaydy + height + 2 };
			//��Ӱ����
			/*defaultfont->DrawTextW(NULL, pCnt->text
			, -1, &textregion, DT_CENTER | DT_VCENTER | DT_NOCLIP
			, SETALPHA(~pCnt->textcolor, 255));*/
			DWORD alphacolor = D3DCOLOR_ARGB(textcolor >> 24
				, ((textcolor >> 16) & 0xFF)*(255 - alpha) / 255
				, ((textcolor >> 8) & 0xFF)*(255 - alpha) / 255
				, (textcolor & 0xFF)*(255 - alpha) / 255);

			textregion.bottom -= 2;
			textregion.right -= 2;
			font->DrawTextW(NULL, text
				, -1, &textregion, DT_CENTER | DT_VCENTER | DT_NOCLIP, alphacolor);
		}
		break;
	case GUI_CONTROL_EDIT:
		dev->SetTexture(0, NULL);
		D3DGUI_RENDER_BUTTON(dev, vbuffer);

		if (text && font)
		{
			font->DrawTextW(NULL, text
				, -1, &textrect, DT_LEFT | DT_VCENTER | DT_NOCLIP, textcolor);
		}
		break;
	case GUI_CONTROL_BACKDROP:
		dev->SetTexture(0, m_overTex);
		D3DGUI_RENDER_BUTTON(dev, vbuffer);
		break;
	}
	lasttick.QuadPart = tick.QuadPart;

	return true;
}

CGUIControl::CGUIControl()
{
	QueryPerformanceFrequency(&frequency);

	type = GUI_CONTROL_NULL;
	ID = 0;

	color = 0;
	dockmode = GUI_WINDOCK_NORMAL;
	dockX = 0;
	dockY = 0;
	posX = 0;
	posY = 0;
	displaydx = 0;
	displaydy = 0;
	width = 0;
	height = 0;

	txtpos = 0;
	text = NULL;
	textcolor = COLOR_WHITE;
	font = NULL;
	textrect = {0, 0, 0, 0};

	bDisabled = false;
	bVisible = true;

	state = GUI_STATE_OUT;

	displayevent = GUI_EVENT_NULL;
	lasttick = { 0 };
	alpha = 0;
	overdx = 0;
	overdy = 0;
	downdx = 0;
	downdy = 0;
}

CGUIControl::~CGUIControl()
{
	Release();
}

bool CGUIControl::Release()
{
	SAFE_DELETE_LIST(text)
	SAFE_RELEASE(font)

	return false;
}

void CGUIControl::Translation(int dx, int dy)
{
	displaydx += dx;
	displaydy += dy;
}

void CGUIControl::Dock(UINT * pbufw, UINT * pbufh)
{
	UINT bufw = SAFE_POINTERVALUE_0(pbufw);
	UINT bufh = SAFE_POINTERVALUE_0(pbufh);

	if (dockmode == GUI_WINDOCK_NORMAL)
	{
		posX = dockX;
		posY = dockY;
	}
	else if (dockmode == GUI_WINDOCK_RIGHT)
	{
		posX = bufw - width - dockX;
		posY = dockY;
	}
	else if (dockmode == GUI_WINDOCK_BOTTOM)
	{
		posX = dockX;
		posY = bufh - height - dockY;
	}
	else if (dockmode == GUI_WINDOCK_BOTTOMRIGHT)
	{
		posX = bufw - width - dockX;
		posY = bufh - height - dockY;
	}
	else if (dockmode == GUI_WINDOCK_BOTTOMHSPAN)
	{
		width = bufw;
		posX = 0;
		posY = bufh - height - dockY;
	}
	textrect = RECT(posX, posY, width, height);// �����ı���ʾ����
}


bool CGUIControl::HandleInput(WPARAM wParam, bool isIME)
{
	if (bDisabled || !bVisible)
		return false;

	if (wParam == '\b')
	{
		if (txtpos > 0)
		{
			text[--txtpos] = 0;
		}
	}
	else if (wParam == '\r')
	{
		return true;
	}
	else if (txtpos < GUI_DEFAULTTEXTLEN)
	{
		if (isIME)
		{
			text[txtpos++] = wParam;
			text[txtpos] = 0;
		}
		else
		{
			if (wParam >= 'A' && wParam <= 'Z'
				|| wParam >= 'a' && wParam <= 'z'
				|| wParam >= '0' && wParam <= '9'
				|| wParam == ' ')
			{
				if (ISLOWERCHAR(wParam) && CAPS_STATUS_ON)
					TOUPPERCHAR_UNSAFE(wParam)
				else if (ISUPPERCHAR(wParam) && !CAPS_STATUS_ON)
					TOLOWERCHAR_UNSAFE(wParam)

					text[txtpos++] = wParam;
				text[txtpos] = 0;
			}
			else if (wParam == 190)
			{
				text[txtpos++] = '.';
				text[txtpos] = 0;
			}
		}
	}
	return false;
}

byte CGUIControl::HandleMouse(byte lmbstate, LONG mouseX, LONG mouseY)
{
	if (bDisabled || !bVisible)
		return GUI_EVENT_NULL;

	byte theevent = GUI_EVENT_NULL;
	byte thestate = state;
	bool inside = (mouseX > posX) && (mouseX < posX + width) &&
		(mouseY > posY) && (mouseY < posY + height);

	// ����¼�����
	theevent = GUI_EVENT_NULL;
	if (inside)
	{
		if (state == GUI_STATE_OUT)
		{
			theevent = GUI_EVENT_ENTER;
			thestate = GUI_STATE_OVER;
		}
	}
	else
	{
		if (state == GUI_STATE_OVER)
		{
			theevent = GUI_EVENT_LEAVE;
			thestate = GUI_STATE_OUT;
		}
	}

	switch (lmbstate) {
	case GUI_MOUSEEVENT_DOWN:
		if (inside && state != GUI_STATE_DOWN)
		{
			theevent = GUI_EVENT_DOWN;
			thestate = GUI_STATE_DOWN;
		}
		break;
	case GUI_MOUSEEVENT_UP:
		if (state == GUI_STATE_DOWN)
		{
			if (inside)
			{
				theevent = GUI_EVENT_UP;
				thestate = GUI_STATE_OVER;
			}
			else
			{
				theevent = GUI_EVENT_OUTUP;
				thestate = GUI_STATE_OUT;
			}
		}
		break;
	}

	// ��ͬ���Ϳؼ�����
	switch (type)
	{
	case GUI_CONTROL_BUTTON:
		if (theevent)
		{
			int ddx = 0, ddy = 0;
			if (thestate == GUI_STATE_OUT)
			{
				ddx = 0;
				ddy = 0;
			}
			else if (thestate == GUI_STATE_OVER)
			{
				ddx = overdx;
				ddy = overdy;
			}
			else if (thestate == GUI_STATE_DOWN)
			{
				ddx = downdx;
				ddy = downdy;
			}
			Translation(ddx - displaydx, ddy - displaydy);
		}

		// ��ʾ
		if (theevent)
		{
			//�����¼�
			displayevent = theevent;
			QueryPerformanceCounter(&lasttick);
			//�¼���ʼ�趨
			if (displayevent == GUI_EVENT_LEAVE)
				alpha = 255;
			else if (displayevent == GUI_EVENT_UP)
				alpha = 0;
		}

		state = thestate;

		break;
	case GUI_CONTROL_EDIT:
		switch (lmbstate) {
		case GUI_MOUSEEVENT_DOWN:
			state = thestate;

			break;
		}
		break;
	}

	return theevent;
}

void CGUIControl::KillFocus()
{
	POINT cursor;
	GetCursorPos(&cursor);
	bool inside = (cursor.x > posX) && (cursor.x < posX + width) &&
		(cursor.y > posY) && (cursor.y < posY + height);

	Translation(0 - displaydx, 0 - displaydy);

	if (inside)
	{
		state = GUI_STATE_OVER;
	}
	else
	{
		state = GUI_STATE_OUT;
	}

	displayevent = GUI_EVENT_NULL;
	alpha = 0;
}

void CGUIControl::SetEnabled(bool bEnabled)
{
	bDisabled = !bEnabled;
}

void CGUIControl::SetVisible(bool bVisible)
{
	this->bVisible = bVisible;
}
