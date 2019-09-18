// 1945.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "1945.h"
#include "Set.h"
#include "Play.h"
#include "CEnemy_Manager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HWND g_hwnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

CSet Set;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	srand((unsigned int)time(NULL));
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1945, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1945));

    MSG msg;
	CPlay Play(&Set, g_hwnd, hInst);  
	CEnemy_Manager Enemy_Manager;
	Set.Setting(g_hwnd, hInst, &Enemy_Manager, Play);  //핸들을 받고 적유닛관리자 핸들도 받는다.
	
    // 기본 메시지 루프입니다.
	DWORD		dwTime = GetTickCount();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (Set.b_Ex == true)
		{
			if (dwTime + 10 < GetTickCount())
			{
			
				dwTime = GetTickCount();
				Play.Paint();
				Play.Progress();
			}
		}
		else
		{
			if (dwTime + 10 < GetTickCount())
			{
				dwTime = GetTickCount();
				Set.Render();	//Set 에서 배경 깔고
				Set.Progress();   //progress 내에서 cursor 깔고
				
			}
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1945));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1945);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   int FrameX, FrameY;
   int CaptionY, MenuY;
   FrameX = GetSystemMetrics(SM_CXFRAME);
   FrameY = GetSystemMetrics(SM_CYFRAME);
   CaptionY = GetSystemMetrics(SM_CYCAPTION);
   MenuY = GetSystemMetrics(SM_CYMENU);
 
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CLIENT_WIDTH+(FrameX<<2), CLIENT_LENGTH+CaptionY+MenuY+(FrameY<<2), nullptr, nullptr, hInstance, nullptr);
   g_hwnd = hWnd;
   
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
//  목적:  주 창의 메시지를 처리합니다.
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
	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = CLIENT_WIDTH;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = CLIENT_LENGTH;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = CLIENT_WIDTH;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = CLIENT_LENGTH;
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
			case ID_BACKGROUND:
			{
				Set.Set_Back(); //디자인 툴에 맵 이미지 깔기 *위에 DlgProc에서 화면 지워지고 다시 깐다.
			}
			break;
			case ID_BGM:
			{
				Set.Set_Bgm();
			}
			break;
			case ID_EX_START:
			{
				if (Set.b_Ex == false) //실행되있지 않은 상태에서만 실행하게끔
				{
					sndPlaySoundA(NULL, SND_ASYNC);
					Set.Play_Start(); // 디자인 툴에 음악 깔고 Player 루프 돌고
				}
			}
			break;
			case ID_EX_STOP:
			{
				sndPlaySoundA(NULL, SND_ASYNC);
				Set.Play_End();
				         // 중지하면 음악 멈추고 Player 루프 안돌고
			}
			break;
			case ID_UNIT1:
			{
				Set.Cursor.Unit_Select(UNIT_ONE);
			}
			break;
			case ID_UNIT2:
			{
				Set.Cursor.Unit_Select(UNIT_TWO);
			}
			break;
			case ID_UNIT3:
			{
				Set.Cursor.Unit_Select(UNIT_THREE);
			}
			break;
			case ID_BOSS1:
			{
				Set.Cursor.Unit_Select(BOSS_ONE);
			}
			break;
			case ID_BOSS2:
			{
				Set.Cursor.Unit_Select(BOSS_TWO);
			}
			break;
			case ID_BOSS3 :
			{
				Set.Cursor.Unit_Select(BOSS_THREE);
			}
			break;
			case ID_BOSS4:
			{
				Set.Cursor.Unit_Select(BOSS_FOUR);
			}
			break;
			case ID_BOSS5:
			{
				Set.Cursor.Unit_Select(BOSS_FIVE);
			}
			break;
			case ID_SAVE: //디자인한 스테이지 문서화 저장
			{
				Set.Save();
			}
			break;
			case ID_LOAD:
			{
				if (Set.b_Ex == false) //실행되있지 않은 상태에서만 로드하게끔
				{
					Set.Load();
				}
			}
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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
