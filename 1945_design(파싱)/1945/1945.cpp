// 1945.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include "1945.h"
#include "Set.h"
#include "Play.h"
#include "CEnemy_Manager.h"

#define MAX_LOADSTRING 100

// ���� ����:
HWND g_hwnd;
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

CSet Set;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	srand((unsigned int)time(NULL));
    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1945, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1945));

    MSG msg;
	CPlay Play(&Set, g_hwnd, hInst);  
	CEnemy_Manager Enemy_Manager;
	Set.Setting(g_hwnd, hInst, &Enemy_Manager, Play);  //�ڵ��� �ް� �����ְ����� �ڵ鵵 �޴´�.
	
    // �⺻ �޽��� �����Դϴ�.
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
				Set.Render();	//Set ���� ��� ���
				Set.Progress();   //progress ������ cursor ���
				
			}
		}
	}

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
			case ID_BACKGROUND:
			{
				Set.Set_Back(); //������ ���� �� �̹��� ��� *���� DlgProc���� ȭ�� �������� �ٽ� ���.
			}
			break;
			case ID_BGM:
			{
				Set.Set_Bgm();
			}
			break;
			case ID_EX_START:
			{
				if (Set.b_Ex == false) //��������� ���� ���¿����� �����ϰԲ�
				{
					sndPlaySoundA(NULL, SND_ASYNC);
					Set.Play_Start(); // ������ ���� ���� ��� Player ���� ����
				}
			}
			break;
			case ID_EX_STOP:
			{
				sndPlaySoundA(NULL, SND_ASYNC);
				Set.Play_End();
				         // �����ϸ� ���� ���߰� Player ���� �ȵ���
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
			case ID_SAVE: //�������� �������� ����ȭ ����
			{
				Set.Save();
			}
			break;
			case ID_LOAD:
			{
				if (Set.b_Ex == false) //��������� ���� ���¿����� �ε��ϰԲ�
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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
// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
