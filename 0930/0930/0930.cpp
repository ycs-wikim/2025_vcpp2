// 0930.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0930.h"
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
    LoadStringW(hInstance, IDC_MY0930, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0930));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0930));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0930);
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

/// 나와 상대 그리고, 음식
RECT g_me, g_you, g_food, g_p1, g_p2;
/// 크기 값을 보관하는 변수
int g_my_size = 150;
/// 생명력을 보관하는 변수
int g_my_life = 5;
/// 게임 상태를 확인할 수 있는 변수: FALSE(게임중), TRUE(게임종료)
BOOL g_gameover;
/// 상대 이동 속도 결정 보관 변수
int g_interval = 1000;
/// 음식의 상태 정보 보관 변수 : FALSE(일반), TRUE(레어)
BOOL g_rare;
/// 치트키 정보
BOOL g_cheat;
/// 나의 이동 속도를 보관할 변수
int g_speed = 10;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 랜덤 Seed 값 변경
        srand(time(NULL));
        /// 나의 객체 좌표 설정 : 객체의 전체 범위가 결정. 140 * 140 크기의 정사각형
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = g_my_size;
        g_me.bottom = g_my_size;
        /// 상대의 좌표 설정
        g_you.left = 560;
        g_you.top = 560;
        g_you.right = 700;
        g_you.bottom = 700;
        /// 음식 좌표 설정
        g_food.left = rand() % 700;
        g_food.top = rand() % 700;
        g_food.right = g_food.left + 50;
        g_food.bottom = g_food.top + 50;
        /// 타이머 설치 - 상대 이동을 위해 사용
        /// 타이머는 설치되면 변경이 불가능!
        SetTimer(hWnd, 1, g_interval, NULL);
        /// 포탈 초기 설정 좌표
        g_p1.left = 750;
        g_p1.top = 10;
        g_p1.right = 800;
        g_p1.bottom = 60;
        g_p2.left = 10;
        g_p2.top = 700;
        g_p2.right = 60;
        g_p2.bottom = 750;
    }
        break;

    case WM_TIMER:
    {
        if (1 == wParam)
        {
            /// 나와 상대의 x 좌표 비교
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

            /// 나와 상대의 y 좌표 비교
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

            if (!g_cheat)
            {
                /// 겹침 처리를 진행 : 나와 상대의 겹침
                RECT ret;

                if (IntersectRect(&ret, &g_me, &g_you))
                {
                    /// 1. 나의 위치 초기화 동시에 크기 조절
                    g_me.left = 10;
                    g_me.top = 10;
                    g_my_size -= 25;
                    g_me.right = g_my_size;
                    g_me.bottom = g_my_size;

                    /// 2. 상대의 위치 초기화
                    g_you.left = 560;
                    g_you.top = 560;
                    g_you.right = 700;
                    g_you.bottom = 700;

                    /// 3. 나의 생명력을 감소
                    g_my_life--;

                    /// 게임 오버인지 확인!
                    if (0 > g_my_life)
                    {
                        g_my_life = 0;
                        KillTimer(hWnd, 1);         /// 상대 이동 금지
                        g_gameover = TRUE;          /// 게임 상태 변경
                    }

                    /*
                    /// 게임이 완전히 종료되는 상태
                    KillTimer(hWnd, 1);
                    MessageBox(hWnd, L"자바써", L"게임 끝", MB_OK);
                    */
                }
            }
            /// 화면에 다시 그리기를 요청
            InvalidateRect(hWnd, NULL, TRUE);
        } /// if (1 == wParam)


    } /// case
        break;

    case WM_KEYDOWN:
    {
        if (g_gameover)
            break;

        switch (wParam)
        {
        case VK_F4:
        {
            /// 치트 키 동작
            g_cheat = TRUE;
        }
            break;
        case VK_LEFT:
        {
            g_me.left -= g_speed;
            g_me.right -= g_speed;
        }
            break;
        case VK_RIGHT:
        {
            g_me.left += g_speed;
            g_me.right += g_speed;
        }
            break;
        case VK_UP:
        {
            g_me.top -= g_speed;
            g_me.bottom -= g_speed;
        }
            break;
        case VK_DOWN:
        {
            g_me.top += g_speed;
            g_me.bottom += g_speed;
        }
            break;
        }
        /// 내 객체의 좌표 계산 완료
        /// 음식을 먹었는지 여부를 확인
        RECT ret;

        if (IntersectRect(&ret, &g_me, &g_food))
        {
            /// 지금 먹은 음식이 레어 음식인지 확인!
            if (g_rare)
            {
                /// 레어 음식이라면 속도 증가
                g_speed += 5;
                /// 레어 속성 제거
                g_rare = FALSE;
            }
            /// 음식의 위치를 랜덤하게 다시 변경
            g_food.left = rand() % 700;
            g_food.top = rand() % 700;
            g_food.right = g_food.left + 50;
            g_food.bottom = g_food.top + 50;
            /// 레어 음식 여부를 결정
            int a1, a2;
            a1 = rand() % 10;
            if (a1 >= 8)     /// 1차 성공
            {
                a2 = rand() % 4;
                if (a2 == 2)
                    g_rare = TRUE;
            }

            /// 상대방의 속도를 변경
            g_interval -= 100;
            if (50 >= g_interval)
            {
                g_interval = 50;
            }
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_interval, NULL);
        }
        
        /// 포탈에 대한 접근 처리
        if (IntersectRect(&ret, &g_me, &g_p1))
        {
            g_p2.left = 10;
            g_p2.top = 700;
            g_p2.right = 60;
            g_p2.bottom = 750;
            g_me.left = 70;
            g_me.top = 700;
            g_me.right = g_me.left + g_my_size;
            g_me.bottom = g_me.top + g_my_size;

        }

        /// 화면 다시 그리기를 OS에게 요청 : 그래야~ 변경된 좌표 그림 확인이 가능!
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
            /// 나의 객체 그리기
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            /// 상대의 객체 그리기
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
            /// 음식 객체 그리기 : 레어 음식 여부를 같이~
            if (g_rare)
            {
                HPEN myPen, osPen;
                myPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
                osPen = (HPEN)SelectObject(hdc, myPen);
                Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
                SelectObject(hdc, osPen);
                DeleteObject(myPen);
            }
            else
            {
                Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
            }
            /// 포탈 정보 그리기
            Ellipse(hdc, g_p1.left, g_p1.top, g_p1.right, g_p1.bottom);
            Ellipse(hdc, g_p2.left, g_p2.top, g_p2.right, g_p2.bottom);

            /// 생명력 정보 출력
            WCHAR buf[128] = { 0, };
            wsprintf(buf, L"남은 생명력 : %d개", g_my_life);
            TextOut(hdc, 700, 10, buf, lstrlenW(buf));
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
