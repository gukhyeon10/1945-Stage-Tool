#pragma once

typedef struct Enemy
{
	int nUnit_Number;
	float fLocation_X, fLocation_Y; // 출현 시작점
	float fCenter_X, fCenter_Y; // 이동패턴의 중심좌표
	float a; // 속도
	DWORD reload; // 발사 텀
	unsigned int nShoot_Count;
	unsigned int nMove_Angle;
	unsigned int nMove_Pattern;
	unsigned int nShoot_Pattern;
}Enemy;

BOOL CALLBACK Pattern_Dlg(HWND, UINT, WPARAM, LPARAM);


class CEnemy_Manager
{
public:
	static unsigned int Move_Pattern;
	static unsigned int Shoot_Pattern;
	HBITMAP hUnit[9];
	
private:
	list<Enemy*> Enemy_List;
	list<Enemy*>::iterator iter_first_unit;
	
	HWND hwnd;
public:
	void Enemy_Copy(list<Enemy*> *pPlay_Enemy_List);
	void Unit_Image_Setting(HINSTANCE hInst, HWND hWnd);
	void Find_Client_First_Unit(int up_down);
	void Unit_Insert(int nNumber, int nX, int nY);
	void Unit_Delete(int nX, int nY, int nWidth, int nLength);
	void Unit_Load(string Enemy_File_Path);
	void Paint(HDC hdc);
	CEnemy_Manager();
	~CEnemy_Manager();
};

