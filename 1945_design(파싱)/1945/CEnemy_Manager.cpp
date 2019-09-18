#include "stdafx.h"
#include "CEnemy_Manager.h"
#include "Cursor.h"
#include "resource.h"
#include "tinyxml2.h"

using namespace tinyxml2;

unsigned int CEnemy_Manager::Move_Pattern = 1;
unsigned int CEnemy_Manager::Shoot_Pattern = 1;


void CEnemy_Manager::Enemy_Copy(list<Enemy*> *pPlay_Enemy_List)
{
	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end(); ++iter)
	{
		Enemy *e = new Enemy;
		e->fLocation_X = (*iter)->fLocation_X; 
		e->fLocation_Y = (*iter)->fLocation_Y;
		e->a = 1;
		e->fCenter_X= (*iter)->fLocation_X;
		e->fCenter_Y = (*iter)->fLocation_Y;
		e->nMove_Angle = 0;
		e->reload = 0;
		e->nShoot_Count = 0;
		e->nMove_Pattern = (*iter)->nMove_Pattern;
		e->nShoot_Pattern = (*iter)->nShoot_Pattern;
		e->nUnit_Number = (*iter)->nUnit_Number;
		(*pPlay_Enemy_List).push_back(e);
	}
	return;
}

void CEnemy_Manager::Unit_Image_Setting(HINSTANCE hInst, HWND hWnd)
{
	hUnit[UNIT_ONE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT1));
	hUnit[UNIT_TWO] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT2));
	hUnit[UNIT_THREE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT3));
	hUnit[BOSS_ONE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS1));
	hUnit[BOSS_TWO] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS2));
	hUnit[BOSS_THREE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS3));
	hUnit[BOSS_FOUR] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS4));
	hUnit[BOSS_FIVE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS5));
	hwnd = hWnd;
	return;
}

void CEnemy_Manager::Find_Client_First_Unit(int up_down)
{

	// 0 = up   1= down
	if (!Enemy_List.empty())
	{
		switch (up_down)
		{
		case 0: // 화면이 올라갈때 유닛 리스트의 시작점 갱신
		{
			BITMAP Image_Rect;
			
			for (list<Enemy*>::iterator iter = iter_first_unit; iter != Enemy_List.end(); ++iter)
			{
				GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);

				if ((*iter)->fLocation_Y + CCursor::nMap_Y < CLIENT_LENGTH && (*iter)->fLocation_Y + CCursor::nMap_Y + Image_Rect.bmHeight >= 0)
				{
					iter_first_unit = iter;
					break;
				}
			}
		}
		break;
		case 1:// 화면이 내려갈때 유닛 리스트의 시작점 갱신
		{
			while (iter_first_unit != Enemy_List.begin())
			{

				if ((*--iter_first_unit)->fLocation_Y + CCursor::nMap_Y > CLIENT_LENGTH)
				{
					++iter_first_unit;
					break;
				}
			}

		

		}
		break;
		}
	}
	return;
}

void CEnemy_Manager::Paint(HDC hdc)
{
	// 배치한 유닛 출력 여기부터 시작
	HDC UnitDC = CreateCompatibleDC(hdc);

	if (!Enemy_List.empty())
	{
		BITMAP Image_Rect;
		for (list<Enemy*>::iterator iter = iter_first_unit; iter != Enemy_List.end(); ++iter)
		{
			GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);
			if ((*iter)->fLocation_Y + CCursor::nMap_Y + Image_Rect.bmHeight >= 0 && (*iter)->fLocation_Y + CCursor::nMap_Y  <= CLIENT_LENGTH)
			{
				SelectObject(UnitDC, hUnit[(*iter)->nUnit_Number]);
				TransparentBlt(hdc, (*iter)->fLocation_X, (*iter)->fLocation_Y + CCursor::nMap_Y, Image_Rect.bmWidth, Image_Rect.bmHeight, UnitDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255,255,255));
			}
			else
			{
				break;
			}
		}
	}
	DeleteDC(UnitDC);

	return;
}

void CEnemy_Manager::Unit_Insert(int nNumber, int nX, int nY)
{
	Enemy *pEnemy = new Enemy;
	pEnemy->nUnit_Number = nNumber;
	pEnemy->fLocation_X = nX;
	pEnemy->fLocation_Y = nY;
	
	//보스면 패턴 정할 필요 x
	if (pEnemy->nUnit_Number >= UNIT_ONE && pEnemy->nUnit_Number <= UNIT_THREE)
	{
		//여기서 다이얼로그 박스 하나 더 출력해서 무브 패턴이랑 슛패턴 번호 설정
		if (DialogBox(NULL, MAKEINTRESOURCE(IDD_PATTERN_DLG), hwnd, Pattern_Dlg) == IDCANCEL)
		{
			delete pEnemy;
			return;
		}

		pEnemy->nMove_Pattern = Move_Pattern;
		pEnemy->nShoot_Pattern = Shoot_Pattern;
	}
	else if(pEnemy->nUnit_Number >=BOSS_ONE && pEnemy->nUnit_Number <=BOSS_FIVE)
	{
		pEnemy->nMove_Pattern = 0;
		pEnemy->nShoot_Pattern = 0;
	}

	if (Enemy_List.empty())
	{
		Enemy_List.push_front(pEnemy);
		iter_first_unit = Enemy_List.begin();
	}
	else
	{
		BITMAP iter_Rect, enemy_Rect;
		GetObject(hUnit[pEnemy->nUnit_Number], sizeof(BITMAP), &enemy_Rect);
		for (list<Enemy*>::iterator iter = iter_first_unit; iter != Enemy_List.end(); ++iter)
		{
			GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &iter_Rect);
			if ((*iter)->fLocation_Y + iter_Rect.bmHeight < pEnemy->fLocation_Y + enemy_Rect.bmHeight)
			{
				Enemy_List.insert(iter, pEnemy);
				if (iter==iter_first_unit)
				{
					iter_first_unit = --iter;
				}
				return;
			}
		}
		list<Enemy*>::iterator iter = Enemy_List.end();
		iter--;
		Enemy_List.push_back(pEnemy);
		if ((*iter)->fLocation_Y + CCursor::nMap_Y > CLIENT_LENGTH)
		{
			iter_first_unit = ++iter;
		}
	}

	return;
}

void CEnemy_Manager::Unit_Delete(int nX, int nY, int nWidth, int nLength)
{
	if (Enemy_List.empty())
	{
		return;
	}

	RECT rc, Unit_Rect, Cursor_Rect;
	Cursor_Rect.left=nX;
	Cursor_Rect.top=nY;
	Cursor_Rect.right=nX+nWidth;
	Cursor_Rect.bottom=nY+nLength;

	BITMAP Image_Rect;
	for (list<Enemy*>::iterator iter = iter_first_unit; iter != Enemy_List.end(); ++iter)
	{
		GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);
		Unit_Rect.left = (*iter)->fLocation_X;
		Unit_Rect.top = (*iter)->fLocation_Y+CCursor::nMap_Y;
		Unit_Rect.right = (*iter)->fLocation_X + Image_Rect.bmWidth;
		Unit_Rect.bottom = (*iter)->fLocation_Y+CCursor::nMap_Y + Image_Rect.bmHeight;

		if ((*iter)->fLocation_Y + CCursor::nMap_Y + Image_Rect.bmHeight >= 0 && (*iter)->fLocation_Y + CCursor::nMap_Y <= CLIENT_LENGTH)
		{
			if (IntersectRect(&rc, &Unit_Rect, &Cursor_Rect) == true)
			{
				//유닛 지우기
				if (iter == iter_first_unit)
				{
					delete (*iter);
					iter_first_unit=Enemy_List.erase(iter);
				}
				else
				{
					delete (*iter);
					Enemy_List.erase(iter);
				}                             
				break;
			}
		}
		else
		{
			break;
		}
	}
	return;
}

void CEnemy_Manager::Unit_Load(string Enemy_File_Path)
{
	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		delete (*iter);
		iter = Enemy_List.erase(iter);
	}
	
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(Enemy_File_Path.c_str()) == XML_ERROR_FILE_NOT_FOUND)
	{
		system("pause"); //파일 못찾으면 pause;
	}
	else
	{
		XMLNode* child;
		const XMLNode* parent = doc.FirstChildElement("root"); // 맨 상위 노드

		for (child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			Enemy *pEnemy = new Enemy;
			child->FirstChildElement("Unit_Number")->QueryIntText(&(pEnemy->nUnit_Number));
			child->FirstChildElement("Location_X")->QueryFloatText(&(pEnemy->fLocation_X));
			child->FirstChildElement("Location_Y")->QueryFloatText(&(pEnemy->fLocation_Y));
			child->FirstChildElement("Move_Pattern")->QueryUnsignedText(&(pEnemy->nMove_Pattern));
			child->FirstChildElement("Shoot_Pattern")->QueryUnsignedText(&(pEnemy->nShoot_Pattern));

			pEnemy->a = 1;
			pEnemy->fCenter_X = pEnemy->fLocation_X;
			pEnemy->fCenter_Y = pEnemy->fLocation_Y;
			pEnemy->nMove_Angle = 0;
			pEnemy->reload = 0;
			pEnemy->nShoot_Count = 0;
			Enemy_List.push_back(pEnemy);

		}
	}

	iter_first_unit = Enemy_List.begin();
	CCursor::nMap_Y = 0;

	return;
}

CEnemy_Manager::CEnemy_Manager()
{

}


CEnemy_Manager::~CEnemy_Manager()
{
	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		delete(*iter);
		iter = Enemy_List.erase(iter);
	}
}


BOOL CALLBACK	Pattern_Dlg(HWND hDlg, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch (nMessage)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			//이동패턴
			if (IsDlgButtonChecked(hDlg, IDC_P1) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 1;
			}
			else if(IsDlgButtonChecked(hDlg, IDC_P2) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 2;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_P3) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 3;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_P4) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 4;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_P5) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 5;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_P6) == BST_CHECKED)
			{
				CEnemy_Manager::Move_Pattern = 6;
			}
			//슈팅패턴
			if (IsDlgButtonChecked(hDlg, IDC_S1) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 1;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_S2) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 2;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_S3) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 3;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_S4) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 4;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_S5) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 5;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_S6) == BST_CHECKED)
			{
				CEnemy_Manager::Shoot_Pattern = 6;
			}
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		break;
		case IDCANCEL:
		{
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
		}
	}
	break;
	}
	return FALSE;
}