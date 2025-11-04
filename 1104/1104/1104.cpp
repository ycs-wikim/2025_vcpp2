// 1104.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1104.h"

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
    LoadStringW(hInstance, IDC_MY1104, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1104));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1104));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1104);
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

/// 스레드 함수에서 받을 인수 구조체
typedef struct data_staruct
{
    HWND            m_hWnd;
    LPARAM          m_lParam;
} DS, *PDS;


/// 공유 자원
int g_x;

/// 자료형 선언
CRITICAL_SECTION g_cs;

///  세마포어 자료형 선언
HANDLE g_sem;

/// 스레드 함수의 동기화 문제 : Semaphore 객체로 해결
DWORD WINAPI func(LPVOID param)
{
    PDS ds = (PDS)param;
    int x = LOWORD(ds->m_lParam);
    int y = HIWORD(ds->m_lParam);
    HDC hdc = GetDC(ds->m_hWnd);

    for (int i = 0; i < y; i++)
    {
        WaitForSingleObject(g_sem, INFINITE);
        //////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        Sleep(100);                     /// 강제 문맥 교환
        LineTo(hdc, g_x, i);
        //////////////////////////////// 임계 영역 종료
        ReleaseSemaphore(g_sem, 1, NULL);
    }

    ReleaseDC(ds->m_hWnd, hdc);

    ExitThread(0);          /// OS에게 자원 정리 요청
    return 0;               /// 반환 값
}


/*
/// 스레드 함수의 동기화 문제 : Critical Section으로 해결
DWORD WINAPI func(LPVOID param)
{
    PDS ds = (PDS)param;
    int x = LOWORD(ds->m_lParam);
    int y = HIWORD(ds->m_lParam);
    HDC hdc = GetDC(ds->m_hWnd);

    

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);
        //////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        //////////////////////////////// 임계 영역 종료
        LeaveCriticalSection(&g_cs);
        Sleep(100);
    }

    

    ReleaseDC(ds->m_hWnd, hdc);

    ExitThread(0);          /// OS에게 자원 정리 요청
    return 0;               /// 반환 값
}
*/


/*      ///////////////////// 소프트웨어 적인 해결 방법 예제 : 실패
/// 플래그 변수
///     FALSE : 자원을 아무도 사용하지 않는 상태
///     TRUE : 자원을 누군가 사용하고 있는 상태
BOOL g_used = FALSE;

/// 스레드 함수의 동기화 문제 : SW적인 해결 방법 시도
DWORD WINAPI func(LPVOID param)
{
    PDS ds = (PDS)param;
    int x = LOWORD(ds->m_lParam);
    int y = HIWORD(ds->m_lParam);
    HDC hdc = GetDC(ds->m_hWnd);

    for (int i = 0; i < y; i++)
    {                                   /// t1  t2  t3   t4  ...
        /// 데커 알고리즘 : 대기
        while (TRUE == g_used){ }
        g_used = TRUE;      /// 공유 자원이 사용중!!!

        g_x = x;                        /// 33  68  132  777
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);

        g_used = FALSE;     /// 공유 자원을 다 사용했음!!!
        Sleep(100);
    }

    ReleaseDC(ds->m_hWnd, hdc);

    ExitThread(0);          /// OS에게 자원 정리 요청
    return 0;               /// 반환 값
}
*/



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// critical section 자료형 초기화
        InitializeCriticalSection(&g_cs);
        /// Semaphore 자료형 초기화
        g_sem = CreateSemaphore(NULL, 3, 3, NULL);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        /// 스레드 생성을 위한 정보체 생성과 설정
        DS ds = { 0, };
        ds.m_hWnd = hWnd;
        ds.m_lParam = lParam;

        /// 스레드 생성
        CreateThread(NULL, 0, func, &ds, 0, NULL);
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
    {
        /// 동기화 객체 Critical Section 해제
        DeleteCriticalSection(&g_cs);
        /// Semaphore 객체 해재
        CloseHandle(g_sem);
        PostQuitMessage(0);
    }
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
