// 1028.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1028.h"

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
    LoadStringW(hInstance, IDC_MY1028, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1028));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1028));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1028);
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


LPARAM g_lParam;

/// 스레드 함수 : lParam을 전역 변수로 선언하고 실행
DWORD WINAPI pig(LPVOID param)
{
    HWND hWnd = (HWND)param;
    int x = LOWORD(g_lParam);
    int y = HIWORD(g_lParam);
    HDC hdc = GetDC(hWnd);

    for (int i = 0; i < y; i++)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, i);
        Sleep(30);         /// interrupt, 스케줄러, 양보
    }

    ReleaseDC(hWnd, hdc);
    /// OS에게 현재 쓰레드를 아름답게 종료시켜 주세요~
    ExitThread(0);
    return 0;
}

HWND g_hWnd;

/// 스레드 함수 : hWnd를 전역 변수로 선언하고 실행
DWORD WINAPI pig1(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    for (int i = 0; i < y; i++)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, i);
        Sleep(30);         /// interrupt, 스케줄러, 양보
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}


/// 일반 함수
void draw(LPARAM lParam, HWND hWnd)
{
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    HDC hdc = GetDC(hWnd);

    for (int i = 0; i < y; i++)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, i);
        Sleep(100);         /// interrupt, 스케줄러, 양보
    }

    ReleaseDC(hWnd, hdc);
}

HANDLE g_handle[10];
int g_cnt = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
        /// 스레드 제어 : 일시 중단
        for (int i = 0; i < 10; i++)
        {
            SuspendThread(g_handle[i]);
        }
    }
        break;

    case WM_KEYDOWN:
    {
        /// 스레드 제어 : 재개
        for (int i = 0; i < 10; i++)
        {
            ResumeThread(g_handle[i]);
        }
    }
        break;

    case WM_LBUTTONDOWN:
    {
        g_lParam = lParam;
        /// CreateThread는 OS에 지정된 정보로 흐름을 만들어 주세요~
        /// 라고 요청하는 API
        //CreateThread(NULL, 0, pig, (LPVOID)hWnd, 0, NULL);
        g_hWnd = hWnd;
        g_handle[g_cnt++] = CreateThread(NULL, 0, pig1, (LPVOID)lParam, 0, NULL);
        /// 일반 함수 호출
        //draw(lParam, hWnd);
    }
        break;

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
