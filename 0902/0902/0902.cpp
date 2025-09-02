// 0902.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0902.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0902, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0902));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0902));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0902);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
/// *** 콜백 함수의 인수는 다음과 같이 고정되어 있다!
/// HWND : 사용자가 입력한 윈도우 자체
/// UINT : 사용자가 취한 액션 (키보드, 마우스 ... )
/// WPARAM : 키보드 관련 정보가 전달
/// LPARAM : 마우스 관련 정보가 전달

int g_x = 10;
int g_y = 10;

/// 콜백 함수(호출될때마다 다시 시작)
/// ==> OS가 호출하는 일반적인 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 사용자가 키보드의 키를 누르자마자 발생하는 메시지
    case WM_KEYDOWN:
        MessageBox(hWnd, L"키보드가 눌렸어요", L"야호", MB_OK);
        break;

    /// 마우스의 왼쪽 버튼을 누른 순간 발생하는 메시지
    //case WM_LBUTTONDOWN:
    /// 마우스가 이동할 때마다 발생하는 메시지
    case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);     /// LOW WORD
            int y = HIWORD(lParam);     /// HI WORD
            //MessageBox(hWnd, L"마우스의 왼쪽 버튼이 눌렸어요", L"야호", MB_OK);
            /// WM_PAINT 이외의 코드 영역에서 HDC를 얻을 수 있는 API
            HDC hdc = GetDC(hWnd);      /// 이 윈도우(hWnd)의 HDC를 요청

            MoveToEx(hdc, g_x, g_y, NULL);    /// xx = 10, yy = 10
            LineTo(hdc, x, y);

            //Rectangle(hdc, g_x, g_y, x, y);

            g_x = g_x + 10;
            g_y += 10;

            ReleaseDC(hWnd, hdc);       /// 이 윈도우에서 사용한 HDC 더 이상 사용하지 않는다.
        }
        break;

    /// WM_ ==> Window Message
    /// 메뉴를 클릭했을때, 발생하는 메시지
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
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    /// 화면을 그린다.
    /// 화면 상에 항상 언제나 표시되거나 그려져야 하는 내용을 그리는 코드가 있는 곳.
    /// 호출되는 경우.
    ///     1. 프로그램이 실행될 때 : OS가 자동으로 호출
    ///     2. 창의 크기를 변경할 때
    ///     3. 창을 최대화할 때
    ///     4. 창을 최소화할 때
    ///     5. 창을 원래 크기로 되돌릴 때
    ///     6. 윈도우 화면 밖으로 창이 나갔을 때
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            /// HDC : H(andle) DC(Device(모니터, 화면) Context(문맥, 정보체))
            /// BeginPaint( ) API: hWnd (이 윈도우에) 추가 정보(ps)에 넣어달라.
            ///     화면에 그리기를 시작하겠다를 OS에게 알리는 API
            ///     WM_PAINT에서만 정상적으로 동작하는 API
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /// 1. 선 그리기 : 2개의 API
            ///     - 시작점을 설정하기 위한 API : MoveToEx( )
            ///     - 끝점을 설정하기 위한 API : LineTo( )
            MoveToEx(hdc, 10, 10, NULL);
            LineTo(hdc, 100, 100);

            /// 2. 사각형 그리기 : 1개의 API
            Rectangle(hdc, 10, 10, 100, 100);

            EndPaint(hWnd, &ps);
            /// EndPaint( ) API : 화면 그리기를 종료하겠다를 OS에게 알리는 API
            ///     WM_PAINT 이외의 코드 영역에서는 정상적으로 동작하지 않는다.
        }
        break;
    /// 윈도우/프로그램을 종료
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    /// 처리할 윈도우 메시지가 없는 경우.
    default:
        /// Default Window Proc : 받았던 정보를 그대로 다시 OS에게 되돌리는 함수
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
