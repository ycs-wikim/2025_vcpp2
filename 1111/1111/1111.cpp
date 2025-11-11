// 1111.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1111.h"

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
    HANDLE mux;
    /// OS에게 "예에"라는 이름의 mutex가 존재하는 확인
    mux = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"예에");
    if (NULL == mux)
    {
        /// 예에 라는 뮤텍스 이름이 없는 상태
        //MessageBox(NULL, L"예에가 없습니다.", L"저런...", MB_OK);
        /// 예에 라는 뮤텍스 이름을 등록
        mux = CreateMutex(NULL, TRUE, L"예에");
    }
    else
    {
        MessageBox(NULL, L"다중 클라이언트 실행은 불법입니다.", L"우후훗", MB_OK);
        return 0;
    }


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1111, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1111));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1111));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1111);
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


/// 스레드 함수 == main 함수

typedef struct data_set
{
    HWND            m_hWnd;
    LPARAM          m_lParam;
} DS, *PDS;

/// 공유 자원 선언
int g_x;

/// Critical Section 자료형 선언
CRITICAL_SECTION g_cs;

/// Critical Section 스레드 생성
DWORD WINAPI pig_cs(LPVOID param)
{
    PDS pds = (PDS)param;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    HDC hdc = GetDC(pds->m_hWnd);

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);
        /////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////////////// 임계 영역 종료
        LeaveCriticalSection(&g_cs);
        Sleep(100);
    }

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}

/// Semaphore 자료형 선언
HANDLE g_sem;
/// 위치 값 설정을 위한 변수
int g_pos = 15;

/// Semaphore 스레드
DWORD WINAPI pig_sem(LPVOID param)
{
    PDS pds = (PDS)param;
    int pos = g_pos;
    g_pos += 15;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    HDC hdc = GetDC(pds->m_hWnd);
    WCHAR buf[128] = { 0, };
    WCHAR sbuf[16] = { 0, };

    WaitForSingleObject(g_sem, INFINITE);
    for (int i = 0; i < 100; i++)
    {
        memset(buf, 0x00, 128);
        for (int j = 0; j < i + 1; j++)
        {
            //// i = 20인 상태
            lstrcat(buf, L"|");     /// |||||||||||....
        }
        wsprintf(sbuf, L"%d%%", i + 1);      /// 20%
        lstrcat(buf, sbuf);
        TextOut(hdc, 10, pos, buf, lstrlen(buf));
        Sleep(100);
    }
    ReleaseSemaphore(g_sem, 1, NULL);

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}

/// Mutex 자료형 서언
HANDLE g_mux;

/// Mutex 스레드 생성
DWORD WINAPI pig_mux(LPVOID param)
{
    PDS pds = (PDS)param;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    HDC hdc = GetDC(pds->m_hWnd);

    for (int i = 0; i < y; i++)
    {
        /// Mutex를 공유 자원 동기화 == Critical Section
        WaitForSingleObject(g_mux, INFINITE);
        /////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////////////// 임계 영역 종료
        ReleaseMutex(g_mux);
        Sleep(100);
    }

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// Critical Section 초기화 : 동기화 객체가 OS 내부에 생성!
        /// 오로지 공유 자원에 대한 접근 여부만 관심 있음!
        InitializeCriticalSection(&g_cs);
        /// Semaphore 객체 초기화
        /// 1. 보안 속성 : NULL 인 경우, 현재 프로세스의 권한을 상속하여 그대로 사용
        /// 2. 초기 (사용 가능한 자원)개수 : 3개
        /// 3. 최대 (사용 가능한 자원)개수 : 3개
        /// 4. 이름 : 지역(프로세스 내부)적인 구분을 위한 ID 값
        g_sem = CreateSemaphore(NULL, 3, 3, NULL);
        /// Mutex 객체 초기화
        /// 1. 보안 속성
        /// 2. 초기 소유자 : 
        /// 3. 이름 : 글로벌(OS)한 구분을 위한 ID 값
        g_mux = CreateMutex(NULL, FALSE, NULL);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        /// 스레드로 전달될 변수 선언과 초기화
        DS ds = { 0, };
        ds.m_hWnd = hWnd;
        ds.m_lParam = lParam;

        /// Critical Section 스레드 생성
        //CreateThread(NULL, 0, pig_cs, (LPVOID)&ds, 0, NULL);
        /// Semaphore 스레드 생성
        //CreateThread(NULL, 0, pig_sem, (LPVOID)&ds, 0, NULL);
        /// Mutex 스레드 생성
        CreateThread(NULL, 0, pig_mux, (LPVOID)&ds, 0, NULL);
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
        /// Critical Section 자원 해제
        DeleteCriticalSection(&g_cs);
        /// Semaphore 자원 해제
        CloseHandle(g_sem);
        /// Mutex 자원 해제
        CloseHandle(g_mux);
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
