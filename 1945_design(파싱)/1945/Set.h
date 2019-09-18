#pragma once
#include"Cursor.h"
#include"CEnemy_Manager.h"
typedef struct BG
{
	HBITMAP hBit;
	int nWidth;
	int nLength;
}BG;

class CPlay;

class CSet
{
private:
	HDC hdc;
	HWND hwnd;
	HINSTANCE hinst;
	string Map_File_Name;
	string Bgm_File_Name;
	CEnemy_Manager *pEnemy_Manager;
public:
	CPlay *pPlay;
	CCursor Cursor;
	bool b_Ex;
	BG Back_Ground;
public:
	void Setting(HWND hWnd, HINSTANCE hInst, CEnemy_Manager *Enemy_Manager, CPlay &Play);
	void Progress();
	void Render();
	void Play_Start();
	void Play_End();
	void Set_Back();
	void Set_Bgm();
	void Save();
	void Load();
	CSet();
	~CSet();
};

