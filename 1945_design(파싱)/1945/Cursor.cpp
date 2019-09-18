#include "stdafx.h"
#include "Cursor.h"
#include "Resource.h"

int CCursor::nMap_Y = 0; // »≠∏È »æΩ∫≈©∑— Ω√¿€ ¡¬«• 

void CCursor::Unit_Image_Set(HINSTANCE hinst)
{
	hUnit[UNIT_ONE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_UNIT1));
	hUnit[UNIT_TWO] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_UNIT2));
	hUnit[UNIT_THREE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_UNIT3));
	hUnit[BOSS_ONE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BOSS1));
	hUnit[BOSS_TWO] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BOSS2));
	hUnit[BOSS_THREE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BOSS3));
	hUnit[BOSS_FOUR] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BOSS4));
	hUnit[BOSS_FIVE] = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BOSS5));
	return;
}

void CCursor::Move(CEnemy_Manager *pEnemy_Manager)
{
	if(GetAsyncKeyState(VK_LEFT))
	{
		Location.x -= 2;
		if (Location.x < 0)
		{
			Location.x = 0;
		}
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		Location.x += 2;
		if (Location.x > CLIENT_WIDTH-Rect.nWidth)
		{
			Location.x = CLIENT_WIDTH - Rect.nWidth;
		}
	}
	if(GetAsyncKeyState(VK_UP))
	{
		Location.y -= 2;
		if (Location.y < 0)
		{
			Location.y = 0;
			nMap_Y += 2;
			pEnemy_Manager->Find_Client_First_Unit(0);
		}
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		Location.y += 2;
		if (Location.y > CLIENT_LENGTH-Rect.nLength)
		{
			Location.y = CLIENT_LENGTH - Rect.nLength;
			if (nMap_Y - 2 >= 0)
			{
				nMap_Y -= 2;
				pEnemy_Manager->Find_Client_First_Unit(1);
			}
		}
	}
	return;
}

void CCursor::Put(CEnemy_Manager *pEnemy_Manager)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		pEnemy_Manager->Unit_Insert(nCursor_Unit, Location.x, Location.y-nMap_Y);
	}
	return;
}

void CCursor::Cut(CEnemy_Manager *pEnemy_Manager)
{
	if (GetAsyncKeyState(VK_BACK) & 0x0001)
	{
		pEnemy_Manager->Unit_Delete(Location.x, Location.y, Rect.nWidth, Rect.nLength);
	}
	return;
}

void CCursor::Paint(HDC hdc)
{
	HDC MemDC = CreateCompatibleDC(hdc);
	SelectObject(MemDC, hUnit[nCursor_Unit]);
	
	BitBlt(hdc, Location.x, Location.y, Rect.nWidth, Rect.nLength, MemDC, 0, 0, SRCCOPY);

	DeleteDC(MemDC);
	return;
}

void CCursor::Unit_Select(int nUnit_Number)
{
	nCursor_Unit = nUnit_Number;
	BITMAP Image_Rect;
	GetObject(hUnit[nUnit_Number], sizeof(BITMAP), &Image_Rect);
	
	Rect.nWidth = Image_Rect.bmWidth;
	Rect.nLength = Image_Rect.bmHeight;
	return;
}

CCursor::CCursor()
{
	nCursor_Unit = UNIT_ONE;
	Location.x=100;
	Location.y = 350;
	Rect.nLength = 100;
	Rect.nWidth = 100;

}


CCursor::~CCursor()
{
}
