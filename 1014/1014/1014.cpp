// 1014.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1014.h"

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
    LoadStringW(hInstance, IDC_MY1014, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1014));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1014));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1014);
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

/// 아이콘 객체를 선언
RECT g_icon;
/// 선택 정보 보관 변수
BOOL g_select;
/// 변위 값 변수 선언
int g_x, g_y;

/// 블록 자료형을 선언
struct Blocks
{
    RECT rect;
    int type;
    int name;
    int color;
};

#define BLOCK_SIZE  10
/// 블록 자료형 변수 선언
Blocks g_block[BLOCK_SIZE][BLOCK_SIZE];



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                g_block[i][j].rect.left = 10 + (i * 50);
                g_block[i][j].rect.top = 10 + (j * 50);
                g_block[i][j].rect.right = g_block[i][j].rect.left + 50;
                g_block[i][j].rect.bottom = g_block[i][j].rect.top + 50;
                g_block[i][j].type = 1;
            }
        }

        g_icon.left = 50;
        g_icon.top = 50;
        g_icon.right = 150;
        g_icon.bottom = 150;
    }
        break;
    case WM_LBUTTONDOWN:
    {
        /// 아이콘 객체를 선택
        RECT ret, current;      /// 현재 위치 임시 RECT 자료형
        current.left = LOWORD(lParam);
        current.top = HIWORD(lParam);
        current.right = current.left + 1;
        current.bottom = current.top + 1;

        if (IntersectRect(&ret, &g_icon, &current))
        {
            /// 마우스로 아이콘을 클릭한 상태
            g_select = TRUE;
            /// 변위/거리 값 계산
            g_x = current.left - g_icon.left;
            g_y = current.top - g_icon.top;
        }

        BOOL changed = FALSE;
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (IntersectRect(&ret, &g_block[i][j].rect, &current))
                {
                    g_block[i][j].type = 2;
                    changed = TRUE;
                }
            }
        }

        if (changed)
            InvalidateRect(hWnd, NULL, TRUE);
    }
        break;
    case WM_MOUSEMOVE:
    {
        if (FALSE == g_select)
            break;
        /// 아이콘이 같이 움직이도록 코드 작성
        g_icon.left = LOWORD(lParam) - g_x;
        g_icon.top = HIWORD(lParam) - g_y;
        g_icon.right = g_icon.left + 100;
        g_icon.bottom = g_icon.top + 100;

        /// 화면을 다시 그리라고 요청
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;
    case WM_LBUTTONUP:
    {
        g_select = FALSE;
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
            Rectangle(hdc, g_icon.left, g_icon.top, g_icon.right, g_icon.bottom);

            for (int i = 0; i < BLOCK_SIZE; i++)
            {
                for (int j = 0; j < BLOCK_SIZE; j++)
                {
                    if (1 == g_block[i][j].type)
                        Rectangle(hdc,
                            g_block[i][j].rect.left, g_block[i][j].rect.top,
                            g_block[i][j].rect.right, g_block[i][j].rect.bottom
                        );
                    else if(2 == g_block[i][j].type)
                        Ellipse(hdc,
                            g_block[i][j].rect.left, g_block[i][j].rect.top,
                            g_block[i][j].rect.right, g_block[i][j].rect.bottom
                        );
                }
            }
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
