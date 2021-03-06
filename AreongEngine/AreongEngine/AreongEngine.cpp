// AreongEngine.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "AreongEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AREONGENGINE, szWindowClass, MAX_LOADSTRING);

	// 창 클래스 등록
	RegisterWindowClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다
    if (!InitInstance (hInstance, nCmdShow))
    {
		MessageBox(0, L"InitInstance - Failed", L"Error", MB_OK);

        return false;
    }

	// 필요 클래스들 초기화
	if (!InitClass())
	{
		MessageBox(0, L"Class Initialization - Failed", L"Error", MB_OK);

		return false;
	}

	// 메시지 루프 처리
	MessageLoop();

	// 종료 후 메모리 정리
	if (!ReleaseClass())
	{
		MessageBox(0, L"Class Release - Failed", L"Error", MB_OK);

		return false;
	}

	return 0;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AREONGENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_AREONGENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// 모니터 해상도 읽어온다
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	// 풀 스크린 모드일 경우와 아닐 경우 셋팅이 다르다
	if (fullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	}
	else
	{
		screenWidth = 1280;
		screenHeight = 720;

		// 화면을 디스플레이의 가운데에 오게한다
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hWnd = CreateWindowEx(NULL,
	   szWindowClass,
	   szTitle,
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT,
	   screenWidth, screenHeight,
	   nullptr,
	   nullptr,
	   hInstance,
	   nullptr);

	if (!hWnd)
	{
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

// 필요 클래스들 초기화
bool InitClass()
{
	// 렌더 매니저 초기화
	m_renderManager = new RenderManager();
	if (m_renderManager == nullptr)
	{
		MessageBox(0, L"renderManager null", L"Error", MB_OK);
		return false;
	}

	if (!m_renderManager->Init(screenWidth, screenHeight, hWnd))
	{
		MessageBox(0, L"renderManager init failed", L"Error", MB_OK);
		return false;
	}

	// 인풋 매니저 초기화
	m_InputManager = new InputManager();
	if (m_InputManager == nullptr)
	{

		MessageBox(0, L"inputmanager null", L"Error", MB_OK);
		return false;
	}

	if (!m_InputManager->Init())
	{
		MessageBox(0, L"inputmanager init failed", L"Error", MB_OK);
		return false;
	}

	return true;
}

// 종료 후 메모리 정리
bool ReleaseClass()
{
	if (m_renderManager != nullptr)
	{
		delete m_renderManager;
	}

	if (m_InputManager != nullptr)
	{
		delete m_InputManager;
	}

	return true;
}

// 프로젝트 세이브
bool SaveProject()
{
	return true;
}

int MessageLoop()
{
	HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_AREONGENGINE));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (true)
	{
		// 윈도우 메시지가 있을 경우
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 종료 메시지의 경우 루프 종료
			if (msg.message == WM_QUIT)
			{
				// 현재 저장해야 하는지 여부 체크 후
				// 저장 한 다음에 종료할 것
				if (!SaveProject())
				{
					MessageBox(0, L"Save Project - Failed", L"Error", MB_OK);
				}

				break;
			}

			// 단축키 작동됬으면 단축키, 아니면 메시지 처리
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			// esc키 감지 및 루프 종료 처리
			if (m_InputManager->IsKeyDown(VK_ESCAPE))
			{
				return false;
			}

			// esc키 감지 및 루프 종료 처리
			if (m_InputManager->IsKeyDown(VK_F1))
			{
				// 큐브만들기
				if (m_renderManager != nullptr)
					m_renderManager->MakeCube();
			}

			if (m_InputManager->IsKeyDown(VK_TAB))
			{
				// 모델 리스트 중 다음 것 선택
				if (m_renderManager != nullptr)
					m_renderManager->SelectPlayer();
			}

			// 매 프레임 렌더 매니저로 렌더링
			if(m_renderManager != nullptr)
				m_renderManager->Render();
		}
	}

	return (int)msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				ShutdownWindows();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
		m_InputManager->KeyDown(wParam);
		break;
	case WM_KEYUP:
		m_InputManager->KeyUp(wParam);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

void ShutdownWindows()
{
	if (fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(hWnd);
	UnregisterClass(szWindowClass, hInst);
}
