#include "stdafx.h"
#include "Play.h"

void CPlay::Progress()
{
	nY++; //종스크롤
	for (int nIndex = 0; nIndex < 2; nIndex++)
	{
		Unit_Move();      //Play_Enemy_List move & shoot;
		for (int nBullet_Speed = 0; nBullet_Speed < 3; nBullet_Speed++)
		{
			Bullet_Manager.Move(); //Bullet_Move;
			
		}
	}
	return;
}

void CPlay::Paint()
{
	
	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_LENGTH);
	SelectObject(MainDC, hMainBitmap);

	HDC BackDC = CreateCompatibleDC(hdc);
	HBITMAP hBackBitmap = CreateCompatibleBitmap(hdc, pSet->Back_Ground.nWidth, pSet->Back_Ground.nLength);
	SelectObject(BackDC, hBackBitmap);

	HDC MemDC = CreateCompatibleDC(hdc);
	SelectObject(MemDC, pSet->Back_Ground.hBit);

	BitBlt(BackDC, 0, (nY%pSet->Back_Ground.nLength), pSet->Back_Ground.nWidth, pSet->Back_Ground.nLength, MemDC, 0, 0, SRCCOPY);
	BitBlt(BackDC, 0, 0, pSet->Back_Ground.nWidth, (nY%pSet->Back_Ground.nLength), MemDC, 0, pSet->Back_Ground.nLength - (nY%pSet->Back_Ground.nLength), SRCCOPY);

	StretchBlt(MainDC, 0, 0, CLIENT_WIDTH, CLIENT_LENGTH, BackDC, 0, 0, pSet->Back_Ground.nWidth, pSet->Back_Ground.nLength, SRCCOPY);

	HDC UnitDC = CreateCompatibleDC(hdc);

	BITMAP Image_Rect;                            

	//유닛 출력
	for (list<Enemy*>::iterator iter = Play_Enemy_List.begin(); iter != Play_Enemy_List.end();)
	{
		GetObject(pImage[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);
		//유닛이 클라이언트 창 내에 있을때
		if ((*iter)->fLocation_Y + nY + Image_Rect.bmHeight >= 0 && (*iter)->fLocation_Y + nY <= CLIENT_LENGTH && (*iter)->fLocation_X + Image_Rect.bmWidth>=0 && (*iter)->fLocation_X <= CLIENT_WIDTH)
		{
			SelectObject(UnitDC, pImage[(*iter)->nUnit_Number]);
			TransparentBlt(MainDC, (*iter)->fLocation_X, (*iter)->fLocation_Y + nY, Image_Rect.bmWidth, Image_Rect.bmHeight, UnitDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));
			++iter;
		}//유닛이 클라이언트 창 밖으로 나갔을때
		else if ((*iter)->fLocation_Y + nY > CLIENT_LENGTH || (*iter)->fLocation_X + Image_Rect.bmWidth<0 || (*iter)->fLocation_X >CLIENT_WIDTH)
		{
			delete (*iter);
			iter = Play_Enemy_List.erase(iter);
		}//다음 유닛이 아직 클라이언트 창에 나타나지 않을때
		else if ((*iter)->fLocation_Y + nY + Image_Rect.bmHeight < 0)
		{
			break;
		}
	}

	//총알 출력
	Bullet_Manager.Bullet_Paint(MainDC);
	

	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_LENGTH, MainDC, 0, 0, SRCCOPY);
	
	

	DeleteDC(BackDC);
	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);
	DeleteObject(hBackBitmap);
	DeleteDC(MemDC);
	DeleteDC(UnitDC);

	return;
}


void CPlay::Play_Init(CEnemy_Manager *Enemy_Manager)
{
	nY = 0;
	pEnemy_Manager = Enemy_Manager;
	pEnemy_Manager->Enemy_Copy(&Play_Enemy_List);

	pImage = pEnemy_Manager->hUnit;
	return;
}

void CPlay::Play_Exit()
{
	for (list<Enemy*>::iterator iter = Play_Enemy_List.begin(); iter != Play_Enemy_List.end();)
	{
		delete (*iter);
		iter = Play_Enemy_List.erase(iter);
	}
	
	Bullet_Manager.Bullet_Clear();

	return;
}

void CPlay::Play_Sound(string str, bool bt)
{
	if (bt == true)
	{
		LPCSTR ch;
		ch = str.c_str();
		sndPlaySoundA(ch, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		return;
	}
	else
	{
		sndPlaySoundA(NULL, SND_ASYNC);
		return;
	}
}

CPlay::CPlay(CSet *ptrSet, HWND hWnd, HINSTANCE hInst)
{
	pSet = ptrSet;
	hwnd = hWnd;
	hdc = GetDC(hwnd);
	Bullet_Manager.Bullet_Load(hInst);
	nY = 0;
}


CPlay::~CPlay()
{
	ReleaseDC(hwnd, hdc);
	
		for (list<Enemy*>::iterator iter = Play_Enemy_List.begin(); iter != Play_Enemy_List.end();)
		{
			delete (*iter);
			iter = Play_Enemy_List.erase(iter);
		}
	
}
