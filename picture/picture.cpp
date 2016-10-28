// picture.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "picture.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PICTURE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	//���
	Init();
	D3DInit();

	WCHAR *param = 0;
	param = GetCommandLineW();
	OnLoadFile(param);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PICTURE));

	MSG msg = { 0 };

    // ����Ϣѭ��: 
	while (msg.message != WM_QUIT)
	{
		//
		//�ڷ�����Ϣǰ���Ķ���
		//
		//�����������,�ڷ�����Ϣǰ�������λ�ã���ֹ��Ϣ�����������ݴ���
		lastcursor = cursor;
		GetCursorPos(&cursor);
		GetCurPos();//�õ����λ�ã����ࣩ,������onmousemove����Ϊ����ȫ����Χ�������Ϣ��

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (IsIconic(mainwnd))
		{
			Sleep(10);//��С��״̬������Դ����
			continue;
		}

		//
		//������Ϣ��Ҫ���Ķ���
		//
		DelayFlag();//�����ӳٵı�־
		//bool oldsize = onsize;
		//if (oldsize && !onsize)//size��������
		//{
		//	Get2WndRect();
		//	if (maindevice)
		//	{
		//		ResetDevice();//�����豸��ʧ
		//	}
		//	if (!mainbmp.Empty())
		//		RefreshSurf();//�����´���ͼƬ������ʾ����
		//}
		//SetCursor();//���������ʽ
		//�϶�ʱ����꾲ֹʱ����ȡ��Ļ��ɫ
		//if (!ondrag && !onzoom)
		//{
		//	HDC pdc = ::GetDC(NULL);
		//	screencolor = GetPixel(pdc, cursor.x, cursor.y);
		//	DeleteDC(pdc);
		//	//��1��3�ֽڻ���
		//	DWORD low = screencolor & 0xFF;
		//	DWORD mid = screencolor & 0xFF00;
		//	DWORD high = screencolor & 0xFF0000;
		//	DWORD top = screencolor & 0xFF000000;
		//	screencolor = top + mid + (low << 16) + (high >> 16);
		//}

		//����
		loopcount++;
		time(&nowtime);

		//��Ⱦ
		Render();

		//������Դ����
		QueryPerformanceCounter(&etime);
		float testfps;
		if (etime.QuadPart != stime.QuadPart)
			testfps = (float)frequency.QuadPart / (etime.QuadPart - stime.QuadPart);
		if (ondrag || onzoom)
		{
			if (testfps > NORMAL_FPS)
				Sleep((UINT32)(1000.0f / NORMAL_FPS - 1000.0f / testfps));
		}
		else
		{
			if (testfps > MIN_FPS)
				Sleep((UINT32)max(1000.0f / MIN_FPS - 1000.0f / testfps, 1));
		}

		//����fps
		QueryPerformanceCounter(&etime);
		if (nowtime != lasttime)//ÿ��ִ��
		{
			fps = (float)frequency.QuadPart / (etime.QuadPart - stime.QuadPart);
			lasttime = nowtime;
		}

		stime = etime;//��ʼ��ʱ
		QueryPerformanceCounter(&stime);
    }
    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICTURE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PICTURE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowExW(WS_EX_ACCEPTFILES, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 520, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   mainwnd = hWnd;//�洢�����ھ��

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//
	//surfsrc�ı䣬һ��Ҫ��������surface�������ڳߴ�ı䣬����Ҫ�������ɣ���Ҫ���´�����ϢGet2WndRect()
	//Zoom()���Ѱ�����������surface
	//
	//

	char key;
	UINT uFileNum;
//	D3DSURFACE_DESC surfDesc;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_OPEN:
				OPENFILENAME opfn;
				WCHAR openfilename[MAX_PATH];//����ļ���  

				//��ʼ��     
				ZeroMemory(&opfn, sizeof(OPENFILENAME));
				opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
				//���ù���     
				opfn.lpstrFilter = L"�����ļ�\0*.*\0bmp�ļ�\0*.bmp\0png�ļ�\0*.png\0jpg�ļ�\0*.jpg\0";
				//Ĭ�Ϲ�����������Ϊ1     
				opfn.nFilterIndex = 1;
				//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ\0
				opfn.lpstrFile = openfilename;
				opfn.lpstrFile[0] = '\0';
				opfn.nMaxFile = sizeof(openfilename);
				//���ñ�־λ�����Ŀ¼���ļ��Ƿ����     
				opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
				//opfn.lpstrInitialDir = NULL;     
				// ��ʾ�Ի������û�ѡ���ļ�     
				if (GetOpenFileName(&opfn))
				{
					//ѡ���ļ������
					//SendMessage(mainwnd, WM_SETTEXT, NULL, (LPARAM)openfilename);
					OnDropFile(openfilename);
				}
				break;
			case IDM_CLEAR:
				Clear();
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			if (!IsIconic(mainwnd))
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
				Render();
				EndPaint(hWnd, &ps);
			}
			else
			{

			}
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_DROPFILES:
		uFileNum = ::DragQueryFile((HDROP)wParam, 0xffffffff, NULL, 0);
		::DragQueryFileW((HDROP)wParam, 0, strFileName, MAX_PATH);//��ȡ�ļ���
		OnDropFile(strFileName);
		break;
	case WM_SIZE:
		if (!IsIconic(mainwnd))
		{
			onsize = true;
			sizetick = GetTickCount64();

			GetCursorPos(&cursor);//sizeʱ��ѭ���л�ȡ��겻����
			int lastwrwidth = Widthof(clientrect);
			int lastwrheight = Heightof(clientrect);
			Get2WndRect();//���´������������Ϊsize��������ʱ����TODO��������һ����������Ϣʵʱ����

			if (maindevice)
			{
				ResetDevice();//�����豸��ʧ
			}

			//!!!surface������backbuffer�����˵��������Բ�����surface�����۵�ǰclip��ʲô״̬
			//��ͼƬʱ������clip���Ҵ�������������������surface
			if (!mainbmp.Empty() 
				&& clip 
				&&(Widthof(clientrect)>= lastwrwidth|| Heightof(clientrect) >= lastwrheight))
				RefreshSurf();
			SurfStatusChange();
		}
		break;
	case WM_MOVE:
		onmove = true;
		movetick = GetTickCount64();
		if (!onsize)
		{
			Get2WndRect();//sizeʱ����
			Render();//sizeʱ��move��Ϣ���ػ棬��onpaint�ػ�
		}
		break;
	case WM_KEYDOWN:
		key = wParam;
		if (mode == MODE_PIC)
		{
			switch (key)
			{
			case 'B'://�������л�1
				realzoom = zoom = 1;
				Zoom(oldzoom, zoom);
				oldzoom = zoom;

				SurfStatusChange();
				break;
			case 'C'://���ڵ��������ð���ͼ��
				if (!mainbmp.Empty())
				{
					wndrect.right = wndrect.left + zoomw + wbias;
					wndrect.bottom = wndrect.top + zoomh + hbias;

					//movewindow����wmsize��Ϣ���������
					MoveWindow(mainwnd, wndrect.left, wndrect.top
						, wndrect.right - wndrect.left
						, wndrect.bottom - wndrect.top, TRUE);
					Get2WndRect();

					surfsrc = { 0, 0 };

					RefreshSurf();

					SurfStatusChange();
				}
				break;
			case 'E'://�л���Ϣ��ʾ
				infoshow = !infoshow;
				break;
			case 'F'://���ݣ����ñ�׼���ڳߴ�
				if (!mainbmp.Empty())
				{
					FitWnd();
					Get2WndRect();

					surfsrc = { 0,0 };

					RefreshSurf();

					SurfStatusChange();
				}
				break;
			case 'M'://ͼƬ����
				if (!mainbmp.Empty())
				{
					CenterPic();

					RefreshSurf();

					SurfStatusChange();
				}
				break;
			case 'P'://���ͼ��
				Clear();
				break;
			case 'Q'://ͼ��Ż����Ͻ�
				surfsrc = { 0,0 };
				RefreshSurf();

				SurfStatusChange();
				break;
			case 'R'://�ֶ���Ⱦһ��
				Render();
				break;
			}
		}
		else if (mode == MODE_CMD)
		{
			;
		}
		break;
	case WM_MOUSEWHEEL:
		if (!mainbmp.Empty())
		{
			Togglezoom(wParam);//�޸�zoom
			Zoom(oldzoom, realzoom);//�Ŵ󣬵���surfaceλ��
			oldzoom = realzoom;

			SurfStatusChange();
		}

		break;
	case WM_SETCURSOR:
		//GetCursorPos(&cursor);
		if (inside(cursor, clientrect))
		{
			if(NoPic())
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			else
				SetCursor(LoadCursor(NULL, IDC_ARROW));//�������������
		}
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));

		break;
	case WM_LBUTTONDOWN:
		if (!mainbmp.Empty())
		{
			SetCapture(mainwnd);
			//GetCursorPos(&lastpos);
			lastpos = cursor;
			ondrag = true;
			break;
		}
	case WM_MOUSEMOVE:
		if (ondrag)
		{
			//�ƶ�ͼƬ
			surfsrc.x -= cursor.x - lastpos.x;
			surfsrc.y -= cursor.y - lastpos.y;

			//��¼���λ��
			lastpos = cursor;

			bool lastpicclipped = picclipped;

			//�ƶ�ʱ�����clip��־������ͼƬС��һ���ߴ磬��ȡ��clip����������surface�������϶��ٶ�
			//���������zoom<1�������
			if (zoomw <= MAX_FORCECLIP_WIDTH_DRAG && zoomh <= MAX_FORCECLIP_HEIGHT_DRAG && clip)
			{
				clip = false;
				RefreshSurf();
			}

			//���ͼƬʼ��ȫ���ڴ��ڿͻ�����Χ�ڣ�ǰ��surfclipped״̬����0�������߲�����clip��־���Ͳ�����
			if ((lastpicclipped || picclipped) && clip)
				RefreshSurf();

			CalcClipped();//����ͼƬclip״̬

			CalcSurfMapInfo();//����surface������backbuffer����
		}
		else
		{
			//GetCursorPos(&cursor);
			GetCurInfo();//��������ѭ����,��ֹ�϶�ͼ��ʱ��굱ǰ��������ֵ����
		}
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		ondrag = false;
		break;
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{
			//ʧȥ���㣬�����־
			ClearFlag();
		}
		break;
	case WM_ERASEBKGND:
		//����ȡ��������ֹ�ػ�ʱ��˸
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

short GetSizeType(CPoint point)
{
	short sizetype = 0;

	if (point.x <= wndrect.left + SIZE_NEAR_PIXEL && point.x >= wndrect.left - SIZE_NEAR_PIXEL
		&& point.y <= wndrect.top + SIZE_NEAR_PIXEL && point.y >= wndrect.top - SIZE_NEAR_PIXEL)
		sizetype = HTTOPLEFT;
	else if (point.x <= wndrect.right + SIZE_NEAR_PIXEL && point.x >= wndrect.right - SIZE_NEAR_PIXEL
		&& point.y <= wndrect.bottom + SIZE_NEAR_PIXEL && point.y >= wndrect.bottom - SIZE_NEAR_PIXEL)
		sizetype = HTBOTTOMRIGHT;
	else if (point.x <= wndrect.right + SIZE_NEAR_PIXEL && point.x >= wndrect.right - SIZE_NEAR_PIXEL
		&& point.y <= wndrect.top + SIZE_NEAR_PIXEL && point.y >= wndrect.top - SIZE_NEAR_PIXEL)
		sizetype = HTTOPRIGHT;
	else if (point.x <= wndrect.left + SIZE_NEAR_PIXEL && point.x >= wndrect.left - SIZE_NEAR_PIXEL
		&& point.y <= wndrect.bottom + SIZE_NEAR_PIXEL && point.y >= wndrect.bottom - SIZE_NEAR_PIXEL)
		sizetype = HTBOTTOMLEFT;
	else if (point.x <= wndrect.left + SIZE_NEAR_PIXEL && point.x >= wndrect.left - SIZE_NEAR_PIXEL)
		sizetype = HTLEFT;
	else if (point.x <= wndrect.right + SIZE_NEAR_PIXEL && point.x >= wndrect.right - SIZE_NEAR_PIXEL)
		sizetype = HTRIGHT;
	else if (point.y <= wndrect.top + SIZE_NEAR_PIXEL && point.y >= wndrect.top - SIZE_NEAR_PIXEL)
		sizetype = HTTOP;
	else if (point.y <= wndrect.bottom + SIZE_NEAR_PIXEL && point.y >= wndrect.bottom - SIZE_NEAR_PIXEL)
		sizetype = HTBOTTOM;
	else if (point.y <= wndrect.bottom + SIZE_NEAR_PIXEL && point.y >= wndrect.bottom - SIZE_NEAR_PIXEL)
		sizetype = 0;
	else
	{
		sizetype = 0;//������������ƶ�����
	}

	return sizetype;
}

inline void SetCursor()
{
	if (sizeEnable)
	{
		mousestate = GetSizeType(cursor);
		switch (mousestate)
		{
		case HTLEFT:
		case HTRIGHT:
			::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			break;
		case HTTOP:
		case HTBOTTOM:
			::SetCursor(LoadCursor(NULL, IDC_SIZENS));
			break;
		case HTTOPLEFT:
		case HTBOTTOMRIGHT:
			::SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			break;
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
			::SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			break;
		default:
			::SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		}
	}
}

void Get2WndRect()
{
	GetClientRect(mainwnd, &clientrect);//�õ�client����ߴ�
	POINT clienttl = { 0, 0 };
	ClientToScreen(mainwnd, &clienttl);//���client�������Ͻǵ���Ļ����
	//�õ�client��ʵ��Ļ����
	clientrect.left = clienttl.x;
	clientrect.top = clienttl.y;
	/*clientrect.right += clientrect.left;
	clientrect.bottom += clientrect.top;*/
	clientrect.right += clienttl.x;
	clientrect.bottom += clienttl.y;

	GetWindowRect(mainwnd, &wndrect);//�õ���������
	wbias = Widthof(wndrect) - Widthof(clientrect);
	hbias = Heightof(wndrect) - Heightof(clientrect);

	//����clientsize
	clientsize = { Widthof(clientrect), Heightof(clientrect) };

	//�޸�������ʾ����
	textrect2.left = TEXTMARGIN_SIDE;
	textrect2.right = textrect2.left + 600;
	textrect2.bottom = clientrect.bottom - clientrect.top - TEXTMARGIN_BOTTOM;//�ײ����
	textrect2.top = clientrect.bottom - clientrect.top - 16;
}

bool Init()
{
	//int gwl_style = -16;
	//oldstyle = GetWindowLong(mainwnd, gwl_style);//-1946157056

	//oldstyle = -1946157056;
	////oldstyle = (oldstyle | WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
	////	& ~WS_CAPTION &~WS_BORDER&~WS_SYSMENU;
	//SetWindowLong(mainwnd, gwl_style, oldstyle);
	
	Get2WndRect();

	wlimit = 800;
	hlimit = 500;
	backcolor = COLOR_BKG;
	mode = MODE_PIC;
	loopcount = 0;
	ondrag = false;
	onzoom = false;
	onsize = false;
	onmove = false;
	sizeEnable = true;
	flagshow = true;
	infoshow = true;
	clientsize = { Widthof(clientrect), Heightof(clientrect) };

	QueryPerformanceFrequency(&frequency);
	etime = { 0 };
	stime = { 0 };
	lasttime = nowtime = 0;
	fps = 0;

	maindevice = NULL;
	mainsurf = NULL;
	realzoom = zoom = 1;
	actualzoomx = actualzoomy = realzoom;
	oldzoom = zoom; 
	zoomw = (int)(realzoom*mainbmp.width);
	zoomh = (int)(realzoom*mainbmp.height);
	clip = realzoom >= 1;
	surfsrc.x = 0;
	surfsrc.y = 0;

	//piccount = 0;
	//piclistlen = PICLISTLEN_INIT;
	//piclist = new PicPack[piclistlen];
	//mainpack = NULL;

	infoshow = true;
	textrect.left = TEXTMARGIN_SIDE;
	textrect.top = TEXTMARGIN_TOP;
	textrect.right = 400;
	textrect.bottom = 600;



	return true;
}

bool D3DInit()
{
	HRESULT hr;

	//Direct3D �ӿڶ���
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//caps
	if (FAILED(lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		MessageBoxW(mainwnd, L"get caps FAILED!", L"", 0);


	//display mode
	hr = lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displaymode);
	if (FAILED(hr))
		MessageBoxW(mainwnd, L"get displaymode FAILED!", L"", 0);

	//D3DPRESENT_PARAMETERS
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Widthof(clientrect);
	d3dpp.BackBufferHeight = Heightof(clientrect);
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;//���٣�displaymode.Format
	d3dpp.BackBufferCount = 2;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = mainwnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = FALSE;//��Ȼ���
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//������D3DFMT_D24S8����D3DFMT_D16
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	//�رմ�ֱͬ��,(��������֡�ʣ��������ڴ�ռ�ã��ϴ�����cpuռ����)
	//����ѭ������֡������£�����Ĭ�ϴ�ֱͬ������������cpuռ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexprocessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexprocessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	InitDevice();

	D3DXCreateFontW(
		maindevice,
		13, 5, 0, 1000, 0,			// �����ַ��Ŀ�ߡ��Ƿ�Ӵ֡�Mipmap�����Ƿ�Ϊб��	
		DEFAULT_CHARSET,			// Ĭ���ַ���
		OUT_DEFAULT_PRECIS,			// ������ȣ�ʹ��Ĭ��ֵ
		CLEARTYPE_NATURAL_QUALITY,	// �ı�����NONANTIALIASED_QUALITY/CLEARTYPE_NATURAL_QUALITY
		DEFAULT_PITCH | FF_DONTCARE,
		L"Arial Rounded MT Bold",	//Arial Rounded MT Bold
		&font
	);
	D3DXCreateFontW(
		maindevice,
		16, 7, 0, 1, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY,//NONANTIALIASED_QUALITY, DEFAULT_QUALITY
		DEFAULT_PITCH | FF_DONTCARE,
		L"Calibri",
		&font2
	);

	return true;
}

bool InitDevice()
{
	HRESULT hr = lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mainwnd
		, vertexprocessing, &d3dpp, &maindevice
	);

	if (FAILED(hr))
	{
		//MessageBoxW(mainwnd, L"Device create FAILED!", L"", 0);
		return false;
	}

	maindevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	return true;
}

bool OnDropFile(WCHAR file[])
{
	if (!maindevice)
	{
		//if (!ResetDevice())
			return false;
	}

	//��ȡͼ����Ϣ
	HRESULT hr;
	ZeroMemory(&imginfo, sizeof(D3DXIMAGE_INFO));
	D3DXGetImageInfoFromFile(file, &imginfo);
	//��ȡ������Ϣ
	if (!imginfo0.GetFile(file))
	{
		MessageBox(mainwnd, L"info read failed", L"1", 0);
		return false;
	}

	//����ǿձ���
	if (tempsurf)
		tempsurf->Release();
	tempsurf = NULL;
	//������ͼ��ƥ�����
	hr = maindevice->CreateOffscreenPlainSurface(
		(LONG)imginfo.Width, (LONG)imginfo.Height, D3DFMT_A8R8G8B8
		, D3DPOOL_SYSTEMMEM, &tempsurf, NULL);
	if (FAILED(hr))
		MessageBoxW(mainwnd, L"CreateOffscreenPlainSurface FAILED!", L"", 0);
	//װ��ͼ��
	hr = D3DXLoadSurfaceFromFileW(
		tempsurf, NULL, NULL, file
		, NULL, D3DX_FILTER_NONE, 0x00000000, NULL);
	if (FAILED(hr))
	{
		MessageBoxW(mainwnd, L"D3DXLoadSurfaceFromFileW FAILED!", L"", 0);
		return false;
	}
	//����BMP
	mainbmp.Load(tempsurf);
	zoomw = (int)(realzoom*mainbmp.width);
	zoomh = (int)(realzoom*mainbmp.height);
	//���surface
	tempsurf->Release();
	tempsurf = NULL;

	//��ͼƬ����
	CenterPic();
	//����Ϊsurface
	RefreshSurf();
	//��Ⱦ
	Render();
	//���棨���ԣ�
	//D3DXSaveSurfaceToFile(L"E:\\1.bmp", D3DXIFF_BMP, mainsurf, NULL, NULL);

	//piclist[0].Read(maindevice, file);
	//mainpack = &(piclist[0]);

	SetWindowTextW(mainwnd, file);

	return true;
}

bool OnLoadFile(LPWSTR file)
{
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	//MessageBoxW(mainwnd, szArgList[argCount - 1], L"", 0);
	if(argCount!=1)
		OnDropFile(szArgList[argCount - 1]);

	return false;
}

inline void DelayFlag()
{
	time_t nowtick = GetTickCount64();

	if (onzoom)
	{
		if (nowtick < zoomtick || nowtick - zoomtick>ZOOMFLAG_DELAY)
			onzoom = false;
	}
	if (onsize)
	{
		if (nowtick < sizetick || nowtick - sizetick>SIZEFLAG_DELAY)
			onsize = false;
	}
	if (onmove)
	{
		if (nowtick < movetick || nowtick - movetick>MOVEFLAG_DELAY)
			onmove = false;
	}
}

inline void ClearFlag()
{
	ondrag = false;
	onzoom = false;
	onsize = false;
}

bool FitWnd()
{
	wndrect.right = wndrect.left + wlimit + wbias;
	wndrect.bottom = wndrect.top + wlimit * (LONG)imginfo.Height / (LONG)imginfo.Width + hbias;
	if (wndrect.bottom - wndrect.top > hlimit)
	{
		wndrect.bottom = wndrect.top + hlimit + hbias;
		wndrect.right = wndrect.left + hlimit * (LONG)imginfo.Width / (LONG)imginfo.Height + wbias;
	}

	MoveWindow(mainwnd, wndrect.left, wndrect.top
		, wndrect.right - wndrect.left
		, wndrect.bottom - wndrect.top, TRUE);

	return true;
}

bool InfoRender()
{
	WCHAR subinfo[200] = { 0 };

	//��ʾ״̬
	if(mainbmp.Empty())
		swprintf_s(infowstr, _T("pic: FALSE"));
	else
	{
		if(outsideclient)
			swprintf_s(infowstr, _T("pic: TRUE (OUTSIDE SCREEN��)"));
		else
			swprintf_s(infowstr, _T("pic: TRUE"));

		if (/*!mainbmp.Empty() && */!mainsurf)//mainbmp���ڵ�surface������
		{
			wcscat_s(infowstr, L"  ");
			swprintf_s(subinfo, _T("SURFACE FAILED!"));
			wcscat_s(infowstr, subinfo);
		}
	}
	wcscat_s(infowstr, L"\n");

	//ͼ��ߴ�
	swprintf_s(subinfo, _T("pic size: %d�� %d  zoom: %.3f/%.3f"), mainbmp.width, mainbmp.height, zoom, realzoom);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//ͼ���ʽ
	swprintf_s(subinfo, _T("format: %S"), GetFMT(imginfo.Format).c_str());
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//ͼ���ļ���С
	swprintf_s(subinfo, _T("file size: %.3lf KB"), (double)imginfo0.bytecount / 1000);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//surface�ߴ�
	if (mainsurf)
	{
		swprintf_s(subinfo, _T("intended surface: %d�� %d")
			, zoomw, zoomh);
		wcscat_s(infowstr, subinfo);
		wcscat_s(infowstr, L"\n");

		swprintf_s(subinfo, _T("surface: %d�� %d  actzoom: X %.4f Y %.4f")
			, (LONG)surfDesc.Width, (LONG)surfDesc.Height, actualzoomx, actualzoomy);
	}
	else
	{
		swprintf_s(subinfo, _T("intended surface: -��-\n"));
		wcscat_s(infowstr, subinfo);
		swprintf_s(subinfo, _T("surface: -��-"));
	}

	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//surface��ʼ��
	swprintf_s(subinfo, _T("surface base: %d, %d"), -surfsrc.x, -surfsrc.y);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n"); 
		
	//clipsurface��ʼ��
	swprintf_s(subinfo, _T("clipsurface base: %d, %d"), surfbase.x, surfbase.y);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");


	//buffer�ߴ�
	swprintf_s(subinfo, _T("buffer: %d�� %d"), d3dpp.BackBufferWidth, d3dpp.BackBufferHeight);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//fps
	swprintf_s(subinfo, _T("fps: %.1f"), fps);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//����
	/*swprintf_s(subinfo, _T("loops: %lld"), loopcount);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");*/

	//����ɫ
	swprintf_s(subinfo, _T("backcolor: %08X.ARGB"), backcolor);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//�������λ��#1
	swprintf_s(subinfo, _T("cursor pos: %S"), cursorposshow[cursorpos].c_str());
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//�������λ��#2
	if(NoPic())
		swprintf_s(subinfo, _T("cursor pixel: -, -"));
	else
		swprintf_s(subinfo, _T("cursor pixel: %d, %d"), curpixel.x, curpixel.y);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//���������ɫ
	swprintf_s(subinfo, _T("pixel color: %02X.%06X.ARGB")
		, (pixelcolor >> 24), (pixelcolor & 0xFFFFFF));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n"); 

	//���������ɫ
	swprintf_s(subinfo, _T("screen color: %02X.%06X.ARGB")
		, (pixelcolor >> 24), (screencolor & 0xFFFFFF));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//���λ��
	swprintf_s(subinfo, _T("cursor: %d, %d"), cursor.x, cursor.y);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//�����Դ��ڿͻ���λ��
	swprintf_s(subinfo, _T("cursor client: %d, %d")
		, cursor.x - clientrect.left, cursor.y - clientrect.top);
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"\n");

	//font->DrawTextW(NULL, infowstr, -1, &textrect, DT_LEFT | DT_NOCLIP, COLOR_TEXT0);
	font->DrawTextW(NULL, infowstr, -1, &textrect, DT_LEFT | DT_NOCLIP, COLOR_TEXT1);



	wcscpy_s(infowstr, L"");
	//��־
	if (!mainbmp.Empty())
		swprintf_s(subinfo, _T("pic: ��"));
	else
		swprintf_s(subinfo, _T("pic: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (mainsurf)
		swprintf_s(subinfo, _T("surf: ��"));
	else
		swprintf_s(subinfo, _T("surf: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (onzoom)
		swprintf_s(subinfo, _T("onzoom: ��"));//��̡�
	else
		swprintf_s(subinfo, _T("onzoom: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (ondrag)
		swprintf_s(subinfo, _T("ondrag: ��"));
	else
		swprintf_s(subinfo, _T("ondrag: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (onsize)
		swprintf_s(subinfo, _T("onsize: ��"));
	else
		swprintf_s(subinfo, _T("onsize: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (onmove)
		swprintf_s(subinfo, _T("onmove: ��"));
	else
		swprintf_s(subinfo, _T("onmove: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (clip)
		swprintf_s(subinfo, _T("clipon: ��"));
	else
		swprintf_s(subinfo, _T("clipon: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (surfclipped)
		swprintf_s(subinfo, _T("surfclipped: ��"));
	else
		swprintf_s(subinfo, _T("surfclipped: ��"));
	wcscat_s(infowstr, subinfo);
	wcscat_s(infowstr, L"    ");

	if (picclipped)
		swprintf_s(subinfo, _T("picclipped: ��"));
	else
		swprintf_s(subinfo, _T("picclipped: ��"));
	wcscat_s(infowstr, subinfo); 
	wcscat_s(infowstr, L"    ");

	if (outsideclient)
		swprintf_s(subinfo, _T("picout: ��"));
	else
		swprintf_s(subinfo, _T("picout: ��"));
	wcscat_s(infowstr, subinfo); 
		wcscat_s(infowstr, L"    ");

	font2->DrawTextW(NULL, infowstr, -1, &textrect2, DT_LEFT | DT_TOP | DT_NOCLIP, COLOR_TEXT1);

	return true;
}

inline bool LoadBackbuffer()
{
	//���backbuffer
	LPDIRECT3DSURFACE9 backbuffer = NULL;
	HRESULT hr = maindevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (FAILED(hr))
	{
		//MessageBoxW(mainwnd, L"GetBackBuffer FAILED!", L"", 0);
		return false;
	}
	//����
	/*const short rwidth = 8;
	RECT rect = {0, 0, rwidth, rwidth};
	for (int i = 0; (i-1)*rwidth < Heightof(clientrect); i++)
	{
		for (int j = 0; (j-1)*rwidth < Widthof(clientrect); j+=2)
		{
			if ((i & 0x01) == 0)
				rect = { j*rwidth,i*rwidth,j*rwidth + rwidth,i*rwidth + rwidth };
			else
				rect = { (j + 1)*rwidth,i*rwidth,(j + 1)*rwidth + rwidth,i*rwidth + rwidth };
			maindevice->ColorFill(backbuffer, &rect, D3DCOLOR_XRGB(204, 203, 204));
		}
	}*/
	//������backbuffer
	//if (clip)
	//{
	//	RECT surfrect;
	//	surfrect.left = 0;//�����ʼ��surfsrcĳһάС��0����ƽ������0��㣬������destpointΪ��0
	//	surfrect.top = 0;
	//	surfrect.right = min(Widthof(clientrect) - surfbase.x, surfDesc.Width);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
	//	surfrect.bottom = min(Heightof(clientrect) - surfbase.y, surfDesc.Height);

	//	hr = maindevice->UpdateSurface(mainsurf, &surfrect, backbuffer, &surfbase);//��surface����
	//}
	//else
	//{
	//	//surface�������򣬾�surface����
	//	POINT destpoint;
	//	RECT surfrect;
	//	if (surfsrc.x < 0)
	//		destpoint.x = -surfsrc.x;
	//	else
	//		destpoint.x = 0;
	//	if (surfsrc.y < 0)
	//		destpoint.y = -surfsrc.y;
	//	else
	//		destpoint.y = 0;
	//	surfrect.left = max(surfsrc.x, 0);//�����ʼ��surfsrcĳһάС��0����ƽ������0��㣬������destpointΪ��0
	//	surfrect.top = max(surfsrc.y, 0);
	//	surfrect.right = min(zoomw, surfrect.left + Widthof(clientrect) - destpoint.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
	//	surfrect.bottom = min(zoomh, surfrect.top + Heightof(clientrect) - destpoint.y);

	//	hr = maindevice->UpdateSurface(mainsurf, &surfrect, backbuffer, &destpoint);//��surface����
	//}
	hr = maindevice->UpdateSurface(mainsurf, &surfrect, backbuffer, &surfbase);//��surface����

	//maindevice->StretchRect(mainsurf, 0, backbuffer, 0, D3DTEXF_NONE);
	if (backbuffer)
		backbuffer->Release();
}

bool Render()
{
	/*if (!maindevice)
	{
		if (!ResetDevice())
			return false;
	}*/

	HRESULT hr;

	//����
	maindevice->Clear(0, NULL, D3DCLEAR_TARGET, backcolor, 1.0f, 0);
	maindevice->BeginScene();
	if (!NoPic())
	{
		//surface�������������ж�(�봰�ڿͻ����޽���),��ֹ��Ⱦ
		if (//���򳬳�surface�һ���
			surfsrc.x > zoomw || surfsrc.y > zoomh
			//�������²���surface�����
			|| surfsrc.x + Widthof(clientrect) <= 0 || surfsrc.y + Heightof(clientrect) <= 0
			)
		{
			outsideclient = true;
		}
		else
		{
			LoadBackbuffer();//surfaceװ�ص�backbuffer
			outsideclient = false;
		}
	}
	if (infoshow)
		InfoRender();
	maindevice->EndScene();
	hr = maindevice->Present(NULL, NULL, NULL, NULL);

	//�����豸��ʧ
	if (hr == D3DERR_DEVICELOST)
	{
		if (maindevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetDevice();
		}
	}

	return true;
}

void CenterPic()
{
	if (zoomw <= d3dpp.BackBufferWidth
		&& zoomh <= d3dpp.BackBufferHeight)
	{
		surfsrc.x = (LONG)(zoomw / 2 - d3dpp.BackBufferWidth / 2);
		surfsrc.y = (LONG)(zoomh / 2 - d3dpp.BackBufferHeight / 2);
	}
	else
	{
		surfsrc.x = 0;
		surfsrc.y = 0;
	}
}

void Togglezoom(WPARAM wParam)
{
	float adds = 0;
	QueryPerformanceCounter(&wheeltick);

	if (zoom<1)
		adds = (float)(frequency.QuadPart / (wheeltick.QuadPart - lastwheeltick.QuadPart)
			*(0.012*zoom));//�����ٶ���ʱ��ͷŴ����й�
	else
		adds = (float)(frequency.QuadPart / (wheeltick.QuadPart - lastwheeltick.QuadPart)
			*(0.002*zoom));//�����ٶ���ʱ��ͷŴ����й�
	lastwheeltick = wheeltick;
	//>=1�Ŵ���ʱ���⴦��
	if (zoom >= 1 && adds < 1)
	{
		if ((short)HIWORD(wParam) > 0)//����
			adds = 1;
		else//����
		{
			if (zoom >= 2)
				adds = 1;
			else if (zoom > 1)
				adds = zoom - 1;
		}
	}
	//adds����
	if (adds < MIN_DELTAZOOM)
		adds = MIN_DELTAZOOM;
	if ((short)HIWORD(wParam) < 0)
		adds = -adds;//addsȡ��

					 //zoom += (short)HIWORD(wParam) / 120;

	if (adds > 0)
	{
		zoom += adds;
	}
	else
	{
		zoom += adds;
	}
	//zoom������
	if (zoom > MAX_ZOOM)
		zoom = MAX_ZOOM;
	if (zoom*min(mainbmp.width, mainbmp.height) < 1)
		zoom = 1.0f / min(mainbmp.width, mainbmp.height);

	//����ʵ��ʹ�õ�zoom
	if (zoom >= 1)
		realzoom = roundf2ui16(zoom);
	else
		realzoom = zoom;
	onzoom = true;//���ñ�־
	zoomtick = GetTickCount64();
}

bool Zoom(float oldzoom, float zoom)
{
	zoomw = (int)(zoom*mainbmp.width);
	zoomh = (int)(zoom*mainbmp.height);

	actualzoomx = (float)zoomw / mainbmp.width;
	actualzoomy = (float)zoomh / mainbmp.height;

	if (oldzoom == zoom)
		return false;

	UINT oldswidth = (UINT)(mainbmp.width*oldzoom), oldsheight = (UINT)(mainbmp.height*oldzoom);
	/*POINT cursorpos;
	GetCursorPos(&cursorpos);*/
	//�ͻ�������Ļ����
	POINT clienttl = { 0, 0 };
	ClientToScreen(mainwnd, &clienttl);
	//�����Կͻ�������
	POINT cursortoclient;
	cursortoclient.x = cursor.x - clienttl.x;
	cursortoclient.y = cursor.y - clienttl.y;
	//���λ�����surface������
	POINT cursortosurface;
	cursortosurface.x = surfsrc.x + cursortoclient.x;
	cursortosurface.y = surfsrc.y + cursortoclient.y;
	//������ʼ��#1
	short altstate = GetAsyncKeyState(VK_MENU);
	if (altstate & 0x8000)
	{
		surfsrc.x = surfsrc.x + ((zoom - oldzoom)*mainbmp.width) / 2;
		surfsrc.y = surfsrc.y + ((zoom - oldzoom)*mainbmp.height) / 2;
	}
	else
	{
		surfsrc.x = (LONG)(roundf2l((float)cursortosurface.x*zoomw / oldswidth) - cursortoclient.x);
		surfsrc.y = (LONG)(roundf2l((float)cursortosurface.y*zoomh / oldsheight) - cursortoclient.y);
	}
	//������ʼ��#2���������������λ��(��Ҫ���ñ�����¼��Ҫ���ֵ�������أ���������)
	/*surfsrc.x = round(curpixel.x*realzoom - cursortoclient.x);
	surfsrc.y = round(curpixel.y*realzoom - cursortoclient.y);*/

	//����ڷŴ�׶Σ����ڿͻ���������ȫ����surface�������surface��ʾ����
	//��ֹСͼƬ�Ŵ�Ѹ��ƫ�봰�ڿͻ���
	if (zoomw <= d3dpp.BackBufferWidth
		&& zoomh <= d3dpp.BackBufferHeight
		&& zoom > oldzoom)
	{
		if (surfsrc.x > 0)
			surfsrc.x = 0;
		if (surfsrc.y > 0)
			surfsrc.y = 0;
		if (surfsrc.x < (LONG)zoomw - (LONG)d3dpp.BackBufferWidth)
			surfsrc.x = (LONG)zoomw - (LONG)d3dpp.BackBufferWidth;
		if (surfsrc.y < (LONG)zoomh - (LONG)d3dpp.BackBufferHeight)
			surfsrc.y = (LONG)zoomh - (LONG)d3dpp.BackBufferHeight;
	}
	//����clip��־
	clip = (realzoom >= 1)
		|| (realzoom < 1 && (zoomw > MAX_FORCECLIP_WIDTH || zoomh > MAX_FORCECLIP_HEIGHT));

	RefreshSurf();

	GetCurInfo();//������������λ��,��Ϊzoom���ܵ���΢Сƫ��

	if (!mainsurf)
		return false;

	return true;
}

bool RefreshSurf()
{
	//mainbmp.CreateSurf(maindevice, &mainsurf, backcolor, zoom);//��surface����
	mainbmp.CreateSurf_Clip(maindevice, &mainsurf
		, surfsrc, clientsize, surfbase, surfclipped
		, realzoom, clip, backcolor);//��surface����

	if (mainsurf)
	{
		mainsurf->GetDesc(&surfDesc);
		CalcSurfMapInfo();//����surface������backbuffer����
		return true;
	}
	else
		return false;
}

inline void DisplayStatusChange(bool winchange, bool surfchange)
{
	if(winchange)
		Get2WndRect();//���¼��㴰�ڴ�С

	if (surfchange)
		RefreshSurf();//��������ͼƬ

	if (surfchange || winchange)
		CalcClipped();//����ͼƬclip״̬

	if (surfchange)
		CalcSurfMapInfo();//����surface������backbuffer����
}

inline void SurfStatusChange()
{
	CalcClipped();//����ͼƬclip״̬

	CalcSurfMapInfo();//����surface������backbuffer����
}


inline void CalcSurfMapInfo()
{
	if (clip)
	{
		surfrect.left = 0;//�����ʼ��surfsrcĳһάС��0����ƽ������0��㣬������destpointΪ��0
		surfrect.top = 0;
		surfrect.right = min(Widthof(clientrect) - surfbase.x, surfDesc.Width);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
		surfrect.bottom = min(Heightof(clientrect) - surfbase.y, surfDesc.Height);
	}
	else
	{
		//surface�������򣬾�surface����
		if (surfsrc.x < 0)
			surfbase.x = -surfsrc.x;
		else
			surfbase.x = 0;
		if (surfsrc.y < 0)
			surfbase.y = -surfsrc.y;
		else
			surfbase.y = 0;
		surfrect.left = max(surfsrc.x, 0);//�����ʼ��surfsrcĳһάС��0����ƽ������0��㣬������destpointΪ��0
		surfrect.top = max(surfsrc.y, 0);
		surfrect.right = min(zoomw, surfrect.left + Widthof(clientrect) - surfbase.x);//�����ʼ���������򲻹����ڿͻ������������Ҳ���²�
		surfrect.bottom = min(zoomh, surfrect.top + Heightof(clientrect) - surfbase.y);
	}
}

inline void CalcClipped()
{
	POINT surfsize = { zoomw, zoomh };//Ԥ�Ƶ�surface�ߴ磬����clip�ж�
	surfsize.y = zoomh;
	if (surfsrc.x > 0)//surface�����clip
	{
		surfsize.x -= surfsrc.x;
	}
	if (surfsrc.y > 0)//surface�ϲ���clip
	{
		surfsize.y -= surfsrc.y;
	}
	if (zoomw - surfsrc.x > Widthof(clientrect))//surface�Ҳ���clip
		surfsize.x -= (zoomw - surfsrc.x - Widthof(clientrect));
	if (zoomh - surfsrc.y > Heightof(clientrect))//surface�²���clip
		surfsize.y -= (zoomh - surfsrc.y - Heightof(clientrect));
	if (surfsize.x < zoomw || surfsize.y < zoomh)
		picclipped = true;
	else
		picclipped = false;
}

inline void GetCurInfo()
{
	/*POINT cursor;
	GetCursorPos(&cursor);*/

	//�ͻ�������Ļ����
	POINT clienttl = { 0, 0 };
	ClientToScreen(mainwnd, &clienttl);
	//�����Կͻ�������
	POINT cursortoclient;
	cursortoclient.x = cursor.x - clienttl.x;
	cursortoclient.y = cursor.y - clienttl.y;
	//���λ�����surface������=surfsrc.x + cursortoclient.x,���ﲻ����������
	curpixel.x = (LONG)((surfsrc.x + cursortoclient.x) / actualzoomx);
	curpixel.y = (LONG)((surfsrc.y + cursortoclient.y) / actualzoomy);

	//�������λ�ú�����Ϣ�Ļ�ȡ
	GetCurColor();
}

inline void GetCurPos()
{
	//�ͻ�������Ļ����
	POINT clienttl = { 0, 0 };
	ClientToScreen(mainwnd, &clienttl);
	//�����Կͻ�������
	POINT cursortoclient;
	cursortoclient.x = cursor.x - clienttl.x;
	cursortoclient.y = cursor.y - clienttl.y;

	//��õ�ǰ���λ�ã����ࣩ
	POINT cursortosurf = { surfsrc.x + cursortoclient.x, surfsrc.y + cursortoclient.y };
	RECT surfrect = { 0, 0, zoomw - 1, zoomh - 1 };
	if (!inside(cursor, clientrect))
		cursorpos = CURSORPOS_OUTWINDOW;
	else if (NoPic())
		cursorpos = CURSORPOS_BLANK;
	else if (/*curpixel.x < 0 || curpixel.x + 1>mainbmp.width
			 || curpixel.y < 0 || curpixel.y + 1>mainbmp.height*/
		!inside(cursortosurf, surfrect))
		cursorpos = CURSORPOS_BLANK;
	else
		cursorpos = CURSORPOS_PIC;
}

inline bool GetCurColor()
{
	if (NoPic())
	{
		pixelcolor = 0;
		return false;
	}

	if (curpixel.x < 0 || curpixel.x + 1>mainbmp.width
		|| curpixel.y < 0 || curpixel.y + 1>mainbmp.height
		|| !inside(cursor, clientrect))//�������س���ͼƬ��Χ����곬�����ڷ�Χ
	{
		pixelcolor = 0;
		return false;
	}
	else
		return mainbmp.GetPixel(curpixel.x, curpixel.y, &pixelcolor);

}

inline bool NoPic()
{
	return (mainbmp.Empty() || !mainsurf);
}

inline void Clear()
{
	mainbmp.Clear();
	if (mainsurf)
		mainsurf->Release();
	mainsurf = NULL;
	ZeroMemory(&imginfo, sizeof(D3DXIMAGE_INFO));
	ZeroMemory(&imginfo0, sizeof(PicInfo));
}

bool ResetDevice()
{
	if (!maindevice)
		return false;

	d3dpp.BackBufferWidth = clientrect.right - clientrect.left;
	d3dpp.BackBufferHeight = clientrect.bottom - clientrect.top;
	/*if(maindevice)
		maindevice->Release();
	if (!InitDevice())
		return false;*/

	//�޸�device����
	OnLostDevice();
	return OnResetDevice();

	//font->Release();
	//D3DXCreateFontW(
	//	maindevice,
	//	13, 5, 0, 1000, 0,			// �����ַ��Ŀ�ߡ��Ƿ�Ӵ֡�Mipmap�����Ƿ�Ϊб��	
	//	DEFAULT_CHARSET,        // Ĭ���ַ���
	//	OUT_DEFAULT_PRECIS,     // ������ȣ�ʹ��Ĭ��ֵ
	//	CLEARTYPE_NATURAL_QUALITY,	// �ı�����NONANTIALIASED_QUALITY/CLEARTYPE_NATURAL_QUALITY
	//	DEFAULT_PITCH | FF_DONTCARE,
	//	L"Arial Rounded MT Bold",
	//	&font
	//);

	//return true;
}

void OnLostDevice()
{
	font->OnLostDevice();
	font2->OnLostDevice();
	/*if(mainsurf)
		mainsurf->Release();*/
}

bool OnResetDevice()
{
	if (FAILED(maindevice->Reset(&d3dpp)))
	{
		return false;
	}
	font2->OnResetDevice();
	font->OnResetDevice();

	return true;
}

string GetFMT(D3DFORMAT fmt)
{
	switch (fmt)
	{
	case D3DFMT_R8G8B8:
		return "D3DFMT_R8G8B8";
	case D3DFMT_A8R8G8B8:
		return "D3DFMT_A8R8G8B8";
	case D3DFMT_X8R8G8B8:
		return "D3DFMT_X8R8G8B8"; 
	case D3DFMT_R5G6B5:
		return "D3DFMT_R5G6B5";
	case D3DFMT_X1R5G5B5:
		return "D3DFMT_X1R5G5B5";
	case D3DFMT_A1R5G5B5:
		return "D3DFMT_A1R5G5B5";
	case D3DFMT_A4R4G4B4:
		return "D3DFMT_A4R4G4B4";
	case D3DFMT_R3G3B2:
		return "D3DFMT_R3G3B2";
	case D3DFMT_A8:
		return "D3DFMT_A8";
	case D3DFMT_A8R3G3B2:
		return "D3DFMT_A8R3G3B2";
	case D3DFMT_X4R4G4B4:
		return "D3DFMT_X4R4G4B4";
	case D3DFMT_A2B10G10R10:
		return "D3DFMT_A2B10G10R10";
	case D3DFMT_A8B8G8R8:
		return "D3DFMT_A8B8G8R8";
	case D3DFMT_X8B8G8R8:
		return "D3DFMT_X8B8G8R8";
	case D3DFMT_G16R16:
		return "D3DFMT_G16R16";
	case D3DFMT_A2R10G10B10:
		return "D3DFMT_A2R10G10B10";
	case D3DFMT_A16B16G16R16:
		return "D3DFMT_A16B16G16R16";
	case D3DFMT_A8P8:
		return "D3DFMT_A8P8";
	case D3DFMT_P8:
		return "D3DFMT_P8";
	case D3DFMT_L8:
		return "D3DFMT_L8";
	case D3DFMT_A8L8:
		return "D3DFMT_A8L8";
	case D3DFMT_A4L4:
		return "D3DFMT_A4L4";
	case D3DFMT_V8U8:
		return "D3DFMT_V8U8";
	case D3DFMT_L6V5U5:
		return "D3DFMT_L6V5U5";
	case D3DFMT_X8L8V8U8:
		return "D3DFMT_X8L8V8U8";
	case D3DFMT_Q8W8V8U8:
		return "D3DFMT_Q8W8V8U8";
	case D3DFMT_V16U16:
		return "D3DFMT_V16U16";
	case D3DFMT_A2W10V10U10:
		return "D3DFMT_A2W10V10U10";
	default:
		return "UNKNOWN";
	}
}

inline int Widthof(const RECT & rect)
{
	return rect.right - rect.left;
}

inline int Heightof(const RECT & rect)
{
	return rect.bottom - rect.top;
}

inline bool inside(const POINT & point, const RECT & rect)
{
	if (point.x<rect.left || point.x>rect.right
		|| point.y<rect.top || point.y>rect.bottom)
		return false;
	else
		return true;
}
