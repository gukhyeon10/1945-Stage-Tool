#include"stdafx.h"
#include"Play.h"

void CPlay::Unit_Move()
{
	BITMAP Image_Rect;
	
	for (list<Enemy*>::iterator iter = Play_Enemy_List.begin(); iter != Play_Enemy_List.end();)
	{
		GetObject(pImage[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);

		if ((*iter)->fLocation_Y + Image_Rect.bmHeight + nY < 0)
		{
			break;
		}
		else
		{
			//총알 발사하기전에 나타나고 나서 몇초후부터 발사를 할건지 
			if ((*iter)->reload != 0)
			{
				Unit_Shoot(iter);
			}
			else
			{				
				(*iter)->reload = GetTickCount()+1000; // 나타나고 2초후에 쏠수있도록
			}

			if((*iter)->nUnit_Number >=UNIT_ONE && (*iter)->nUnit_Number <= UNIT_THREE)
			{ 
				// 유닛 이동패턴
				switch ((*iter)->nMove_Pattern)
				{
					case 1: //수직 하강
					{
						(*iter)->fLocation_Y++;
					}
					break;
					case 2: //지그재그
					{	
						(*iter)->fLocation_X += (*iter)->a;
						if ((*iter)->a > 0)
						{
							if ((*iter)->fCenter_X + 40 < (*iter)->fLocation_X)
							{
								(*iter)->a = -1;
							}
						}
						else
						{
							if ((*iter)->fCenter_X -40 > (*iter)->fLocation_X)
							{
								(*iter)->a = 1;
							}
						}
					}
					break; 
				case 3: //어느정도 오다가 멈춤
				{
					(*iter)->fLocation_Y += (*iter)->a;
					if ((*iter)->a > -0.5f)
					{
						(*iter)->a -= 0.005f;
					}
					else
					{
						(*iter)->a = -0.5f;
					}
				}
				break;
				case 4: //내려오다가 대각선으로 이동
				{
					(*iter)->fLocation_Y += (*iter)->a;
					if ((*iter)->a > -0.5f)
					{
						(*iter)->a -= 0.005f;
					}
					else
					{
						if ((*iter)->fCenter_X > CLIENT_WIDTH / 2)
						{
							(*iter)->fLocation_X-=2;
							(*iter)->fLocation_Y++;
						}
						else
						{
							(*iter)->fLocation_X+=2;
							(*iter)->fLocation_Y ++;
						}
					}
				}
				break;
				case 5: // 좀 더 빠르게 하강 
				{
					(*iter)->fLocation_Y+=1.5;
				}
				break;
				case 6: // 원을 그리면서 하강
				{
					if ((*iter)->fLocation_Y+nY<30)
					{
						(*iter)->fCenter_X = (*iter)->fLocation_X - 90;
						(*iter)->fCenter_Y = (*iter)->fLocation_Y;
					}
					else
					{
						float r = (*iter)->nMove_Angle*3.14 / 180;
						(*iter)->fLocation_X = (*iter)->fCenter_X + (cos(r) * 90);
						(*iter)->fLocation_Y = (*iter)->fCenter_Y + (sin(r) * 90);
						(*iter)->nMove_Angle++;
					}
				}
				break;
				}
			}
			else if ((*iter)->nUnit_Number >= BOSS_ONE && (*iter)->nUnit_Number <= BOSS_FIVE)
			{
				(*iter)->fLocation_Y += (*iter)->a;
				if ((*iter)->a > -0.5f)
				{
					(*iter)->a -= 0.005f;
				}
				else
				{
					(*iter)->a = -0.5f;
				}

				switch ((*iter)->nUnit_Number)
				{
				case BOSS_ONE:
				{
				}
				break;
				case BOSS_TWO:
				{
				}
				break;
				case BOSS_THREE:
				{
				}
				break;
				case BOSS_FOUR:
				{
				}
				break;
				case BOSS_FIVE:
				{
				}
				break;
				}
			}
			++iter;
		}
	}
	return;
}

void CPlay::Unit_Shoot(list<Enemy*>::iterator unit)
{
	float fMuzzle_x, fMuzzle_y; //유닛 기준에서 총알이 나가는 좌표
	switch ((*unit)->nUnit_Number)
	{
	case UNIT_ONE:
	{
		fMuzzle_x = 40;
		fMuzzle_y = 47;
	}
	break;
	case UNIT_TWO:
	{
		fMuzzle_x = 53;
		fMuzzle_y = 78;
	}
	break;
	case UNIT_THREE:
	{
		fMuzzle_x = 64;
		fMuzzle_y = 108;
	}
	break;
	default: //보스들
	{
		fMuzzle_x = -1;
		fMuzzle_y = -1;
	}
	break;
	}

	if ((*unit)->nUnit_Number >= UNIT_ONE && (*unit)->nUnit_Number <= UNIT_THREE)
	{
		switch ((*unit)->nShoot_Pattern)
		{
		case 1: // 직선 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <5) //0.15초당 발사하고 한번씩 5회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X +fMuzzle_x , (*unit)->fLocation_Y +fMuzzle_y+ nY, 0, 1);
			}
			else if((*unit)->reload +1000<GetTickCount()) // 5번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 2: //3방향 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <3) //0.15초당 발사하고 한번씩 3회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0, 1);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, -0.2, 1);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0.2, 1);
			}
			else if ((*unit)->reload + 1000<GetTickCount()) // 3번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 3: // 좌 우로 발사
		{
			if ((*unit)->reload + 200 < GetTickCount() && (*unit)->nShoot_Count < 2)
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				if ((*unit)->fLocation_X < CLIENT_WIDTH / 2)
				{
					for (int nIndex = 0; nIndex < 3; nIndex++)
					{
						float xx = (rand() % 5) + 10;
						float yy = (rand() % 6) - 2;
						Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, xx/20, yy/20);
					}
				}
				else
				{
					for (int nIndex = 0; nIndex < 3; nIndex++)
					{
						float xx = (rand() % 5) + 10;
						float yy = (rand() % 6) - 2;
						Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, (xx / 20)*-1, yy / 20);
					}
				}
			}
			else if ((*unit)->reload + 1000<GetTickCount()) //5번 모두 쏘면 다시 재장전까지 0.8초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 4: // 원 발사
		{
			
			if ((*unit)->reload + 100 < GetTickCount() && (*unit)->nShoot_Count <1) //0.1초당 발사하고 한번씩 5회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				for (int nIndex = 0; nIndex < 44; nIndex++)
				{
					Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, sin(nIndex)/2, cos(nIndex)/2);
				}
			}
			else if ((*unit)->reload + 1500<GetTickCount()) // 5번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 5: // 육각 방향으로 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <6) //0.15초당 발사하고 한번씩 6회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, -0.3, 0.5);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0.3, 0.5); 
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, -0.5, 0);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0.5, 0);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, -0.3, -0.5);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0.3, -0.5);
			}
			else if ((*unit)->reload + 1500<GetTickCount()) // 30번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 6: // 원을 그리면서 발사
		{
			if ((*unit)->reload + 50 < GetTickCount())
			{
				if ((*unit)->nShoot_Count++ >= 90)
				{
					(*unit)->nShoot_Count = 0;

				}
				(*unit)->reload = GetTickCount();
				//시계 패턴
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, cos((*unit)->nShoot_Count * 6), sin((*unit)->nShoot_Count * 6));
				
			}
		}
		break;
		}
	}
	else if((*unit)->nUnit_Number >= BOSS_ONE && (*unit)->nUnit_Number <= BOSS_FIVE)
	{
		switch ((*unit)->nUnit_Number)
		{
		case BOSS_ONE:
		{

		}
		break;
		case BOSS_TWO:
		{

		}
		break;
		case BOSS_THREE:
		{

		}
		break;
		case BOSS_FOUR:
		{

		}
		break;
		case BOSS_FIVE:
		{

		}
		break;
		}
	}
	return;
}