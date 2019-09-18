#pragma once

#define Bullet_Width 20
#define Bullet_Heigth 20

typedef struct Bullet
{
	float x, y;
	float xx, yy;

}Bullet;

class CBullet_Manager
{
private:
	list<Bullet*> Bullet_List;
	HBITMAP hBullet;
public:
	void Bullet_Load(HINSTANCE hinst);
	void Bullet_Add(float fx, float fy, float fxx, float fyy);
	void Move();
	void Bullet_Clear();
	void Bullet_Paint(HDC MainDC);
	CBullet_Manager();
	~CBullet_Manager();
};

