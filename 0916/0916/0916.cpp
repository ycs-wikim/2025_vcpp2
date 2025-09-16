// 0916.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0916.h"

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
    LoadStringW(hInstance, IDC_MY0916, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0916));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0916));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0916);
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

/// 시작 점의 좌표를 보관할 전역 변수
int g_x, g_y;
/// 선택한 그리기 종류를 별도로 보관할 플레그 전역 변수
/// 0 - 그리지 않는다. 1 - 선 그리기, 2 - 사각형 그리기, 3 - 타원 그리기
/// 4 - 자유선 그리기
int g_flag;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
        /// 그리기를 초기화
        g_flag = 0;
    }
        break;
    case WM_LBUTTONDOWN:
    {
        if (0 == g_flag)
            break;

        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        if (0 == g_flag)
            break;

        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        /// 선 속성 변경
        HPEN myPen, osPen;
        /// 1. 펜 객체를 생성
        myPen = CreatePen(PS_SOLID, 10, RGB(200, 100, 100));
        /// 2. OS에게 펜 교체를 요청. 3. OS의 펜이 반환
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 4. 사용
        if (1 == g_flag)
        {
            MoveToEx(hdc, g_x, g_y, NULL);    /// 시작점 설정
            LineTo(hdc, x, y);
        }
        else if (2 == g_flag)
        {
            /// 면색 변경
            HBRUSH myBrush, osBrush;
            /// 1. 브러시를 생성
            myBrush = CreateSolidBrush(RGB(0, 0, 0));
            /// 2. 브러시를 교체 요청. 3. OS의 브러시가 반환
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            /// 4. 사용
            Rectangle(hdc, g_x, g_y, x, y);
            /// 5. 원래 브러시로 복원
            SelectObject(hdc, osBrush);
            /// 6. 브러시 객체 삭제
            DeleteObject(myBrush);
        }
        else if (3 == g_flag)
        {
            Ellipse(hdc, g_x, g_y, x, y);
        }
        /// else if(4 == g_flag) // 자유선 그리기

        /// 5. OS의 원래 펜으로 복원
        SelectObject(hdc, osPen);
        /// 6. 생성한 펜 객체 해제
        DeleteObject(myPen);

        ReleaseDC(hWnd, hdc);
    }
        break;

    /// 메뉴를 클릭했을때 전달되는 윈도우 메시지
    case WM_COMMAND:
        {
            /// 메뉴를 선택했을 때, 해당 ID 값은 WPARAM
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_DRAW_LINE:
                g_flag = 1;
                //MessageBox(hWnd, L"선 그리기", L"피곤하다", MB_OK);
                break;
            case ID_DRAW_RECTANGLE:
                g_flag = 2;
                break;
            case ID_DRAW_ELLIPSE:
                g_flag = 3;
                break;
            case ID_DRAW_FREE:
                g_flag = 4;
                break;

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
