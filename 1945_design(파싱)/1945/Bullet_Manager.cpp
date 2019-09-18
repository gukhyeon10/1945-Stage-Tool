#include "stdafx.h"
#include "Resource.h"
#include "Bullet_Manager.h"

void CBullet_Manager::Bullet_Load(HINSTANCE hinst)
{
	hBullet = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_ENEMY_BULLET));
	return;
}

void CBullet_Manager::Bullet_Add(float fx, float fy, float fxx, float fyy)
{
	Bullet *pBullet = new Bullet;
	pBullet->x = fx;
	pBullet->y = fy;
	pBullet->xx = fxx;
	pBullet->yy = fyy;
	Bullet_List.push_back(pBullet);
	return;
}

void CBullet_Manager::Move()
{

	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end();)
	{
		if ((*iter)->x <= CLIENT_WIDTH && (*iter)->x + Bullet_Width >= 0 && (*iter)->y + Bullet_Heigth >= 0 && (*iter)->y <= CLIENT_LENGTH)
		{
			(*iter)->x += (*iter)->xx;
			(*iter)->y += (*iter)->yy;
			++iter;
		}
		else
		{
			delete (*iter);
			iter = Bullet_List.erase(iter);
		}
	}
	return;
}

void CBullet_Manager::Bullet_Clear()
{
	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end();)
	{
		delete (*iter);
		iter = Bullet_List.erase(iter);
	}
	return;
}

void CBullet_Manager::Bullet_Paint(HDC MainDC)
{
	HDC MemDC = CreateCompatibleDC(MainDC);
	SelectObject(MemDC, hBullet);
	//TransparentBlt(MainDC, 50, 50, Bullet_Width, Bullet_Heigth, MemDC, 0, 0, Bullet_Width, Bullet_Heigth, RGB(255, 255, 255));
	
	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end(); ++iter)
	{
			TransparentBlt(MainDC, (*iter)->x, (*iter)->y, Bullet_Width, Bullet_Heigth, MemDC, 0, 0, Bullet_Width, Bullet_Heigth, RGB(255, 255, 255));
	}

	DeleteDC(MemDC);
	return;
}

CBullet_Manager::CBullet_Manager()
{

}


CBullet_Manager::~CBullet_Manager()
{
	Bullet_Clear();
}
