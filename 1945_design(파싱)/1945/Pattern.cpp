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
			//�Ѿ� �߻��ϱ����� ��Ÿ���� ���� �����ĺ��� �߻縦 �Ұ��� 
			if ((*iter)->reload != 0)
			{
				Unit_Shoot(iter);
			}
			else
			{				
				(*iter)->reload = GetTickCount()+1000; // ��Ÿ���� 2���Ŀ� ����ֵ���
			}

			if((*iter)->nUnit_Number >=UNIT_ONE && (*iter)->nUnit_Number <= UNIT_THREE)
			{ 
				// ���� �̵�����
				switch ((*iter)->nMove_Pattern)
				{
					case 1: //���� �ϰ�
					{
						(*iter)->fLocation_Y++;
					}
					break;
					case 2: //�������
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
				case 3: //������� ���ٰ� ����
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
				case 4: //�������ٰ� �밢������ �̵�
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
				case 5: // �� �� ������ �ϰ� 
				{
					(*iter)->fLocation_Y+=1.5;
				}
				break;
				case 6: // ���� �׸��鼭 �ϰ�
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
	float fMuzzle_x, fMuzzle_y; //���� ���ؿ��� �Ѿ��� ������ ��ǥ
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
	default: //������
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
		case 1: // ���� �߻�
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <5) //0.15�ʴ� �߻��ϰ� �ѹ��� 5ȸ �߻��ϰ� 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X +fMuzzle_x , (*unit)->fLocation_Y +fMuzzle_y+ nY, 0, 1);
			}
			else if((*unit)->reload +1000<GetTickCount()) // 5�� ��� ��� �ٽ� ���������� 2�ʰɸ���
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 2: //3���� �߻�
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <3) //0.15�ʴ� �߻��ϰ� �ѹ��� 3ȸ �߻��ϰ� 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0, 1);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, -0.2, 1);
				Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, 0.2, 1);
			}
			else if ((*unit)->reload + 1000<GetTickCount()) // 3�� ��� ��� �ٽ� ���������� 2�ʰɸ���
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 3: // �� ��� �߻�
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
			else if ((*unit)->reload + 1000<GetTickCount()) //5�� ��� ��� �ٽ� ���������� 0.8�ʰɸ���
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 4: // �� �߻�
		{
			
			if ((*unit)->reload + 100 < GetTickCount() && (*unit)->nShoot_Count <1) //0.1�ʴ� �߻��ϰ� �ѹ��� 5ȸ �߻��ϰ� 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				for (int nIndex = 0; nIndex < 44; nIndex++)
				{
					Bullet_Manager.Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + nY, sin(nIndex)/2, cos(nIndex)/2);
				}
			}
			else if ((*unit)->reload + 1500<GetTickCount()) // 5�� ��� ��� �ٽ� ���������� 2�ʰɸ���
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 5: // ���� �������� �߻�
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count <6) //0.15�ʴ� �߻��ϰ� �ѹ��� 6ȸ �߻��ϰ� 
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
			else if ((*unit)->reload + 1500<GetTickCount()) // 30�� ��� ��� �ٽ� ���������� 2�ʰɸ���
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 6: // ���� �׸��鼭 �߻�
		{
			if ((*unit)->reload + 50 < GetTickCount())
			{
				if ((*unit)->nShoot_Count++ >= 90)
				{
					(*unit)->nShoot_Count = 0;

				}
				(*unit)->reload = GetTickCount();
				//�ð� ����
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