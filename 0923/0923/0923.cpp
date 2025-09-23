// 0923.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0923.h"
#include <stdlib.h>
#include <time.h>

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
    LoadStringW(hInstance, IDC_MY0923, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0923));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0923));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0923);
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

/// 나의 객체 정보
RECT g_me;
/// 상대 객체 정보
RECT g_you;
/// 음식 객체 정보
RECT g_food;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:             /// 생성자와 동일한 동작
    {
        /// 1. 나의 객체의 초기화 위치 설정
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 150;
        g_me.bottom = 150;
        /// 2. 상대 객체의 초기화 위치 설정
        g_you.left = 500;
        g_you.top = 500;
        g_you.right = 640;
        g_you.bottom = 640;
        /// 3. 주기적인 윈도우 메시지 호출 요청 : 타이머 설정
        /// - 1초마다 ID가 1인 타이머가 지속적으로 호출
        SetTimer(hWnd, 1, 1000, NULL);
        /// 4. 음식의 초기화 위치 설정
        g_food.left = 250;
        g_food.top = 250;
        g_food.right = 300;
        g_food.bottom = 300;
        /// 5. 랜덤 Seed 값 변경
        srand(time(NULL));
    }
        break;

    case WM_TIMER:
    {
        /// 내가 전달한 타이머를 확인
        if (1 == wParam)
        {
            if (g_me.left < g_you.left)
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                g_you.left += 10;
                g_you.right += 10;
            }

            if (g_me.top < g_you.top)
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
        }
        /// 좌표 이동이 끝난 상태
        /// 겹침을 확인
        RECT ret;   /// 겹침이 있을때, 겹친 영역 좌표 값을 반환

        /// 상대가 움직인 상태에서 나를 잡았는지 확인
        if (IntersectRect(&ret, &g_me, &g_you))
        {
            /// 초기 설정된 타이머 ID를 이용하여 타이머를 정지
            KillTimer(hWnd, 1);
            MessageBox(hWnd, L"잡았다", L"게임 오바", MB_OK);
        }
        
        /// WM_PAINT를 호출한다.
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
            case VK_LEFT:       /// VK_ == Virtual Keyboard
            {
                g_me.left -= 10;
                g_me.right -= 10;
                /// 좌표 이동은 반드시 먼저 실행한다.
                if (10 > g_me.left)
                {
                    g_me.left = 10;
                    g_me.right = g_me.left + 140;
                }
            }
                break;
            case VK_RIGHT:
            {
                g_me.left += 10;
                g_me.right += 10;

                if (660 < g_me.left)
                {
                    g_me.left = 660;
                    g_me.right = 800;
                }
                    
            }
                break;
            case VK_UP:
            {
                g_me.top -= 10;
                g_me.bottom -= 10;
            }
                break;
            case VK_DOWN:
            {
                g_me.top += 10;
                g_me.bottom += 10;
            }
                break;
        }
        /// 나의 객체 좌표 이동이 완료

        /// 내가 음식을 먹었는지 확인
        RECT ret;

        if (IntersectRect(&ret, &g_me, &g_food))
        {
            /*
            /// 음식을 사라지게 만든다.
            g_food.left = -100;
            g_food.top = -100;
            g_food.right = -100;
            g_food.bottom = -100;
            */
            /// 음식을 랜덤한 위치에 좌표 계산
            g_food.left = rand() % 600;
            g_food.top = rand() % 600;
            g_food.right = g_food.left + 50;
            g_food.bottom = g_food.top + 50;
        }

        /// WM_PAINT를 호출한다.
        InvalidateRect(hWnd, NULL, TRUE);
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
            /// 그라운드 출력
            Rectangle(hdc, 10, 10, 800, 600);
            /// 나의 객체 출력
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            /// 상대 객체 출력
            Ellipse(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
            /// 음식 객체 출력
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);

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
