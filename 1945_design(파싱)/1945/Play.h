#pragma once
#include "Set.h"
#include "Bullet_Manager.h"

class CPlay
{
private:
	CSet *pSet;
	HWND hwnd;
	HDC hdc;
	int nY; //종스크롤 y 좌표
	HBITMAP *pImage;
	CEnemy_Manager *pEnemy_Manager;
	list<Enemy*> Play_Enemy_List;
	CBullet_Manager Bullet_Manager;
public:
	void Play_Init(CEnemy_Manager *Enemy_Manager);
	void Play_Exit();
	void Progress();
	void Unit_Move();
	void Unit_Shoot(list<Enemy*>::iterator unit);
	void Paint();
	void Play_Sound(string str, bool bt);
	CPlay(CSet *ptrSet, HWND hWnd, HINSTANCE hInst);
	~CPlay();
};

