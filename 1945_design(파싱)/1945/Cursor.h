#pragma once
#include"CEnemy_Manager.h"
typedef struct Cursor_Location
{
	int x;
	int y;
}Cursor_Location;

typedef struct Cursor_Rect
{
	int nWidth;
	int nLength;
}Cursor_Rect;

class CCursor
{
public:
	static int nMap_Y;
private:
	Cursor_Location Location;
	Cursor_Rect Rect;
	int nCursor_Unit;
	HBITMAP hUnit[9];
public:
	void Unit_Image_Set(HINSTANCE hinst);
	void Move(CEnemy_Manager *pEnemy_Manager);
	void Put(CEnemy_Manager *pEnemy_Manager);
	void Cut(CEnemy_Manager *pEnemy_Manager);
	void Paint(HDC hdc);
	void Unit_Select(int nUnit_Number);
	CCursor();
	~CCursor();
};
