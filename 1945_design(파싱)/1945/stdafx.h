// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include<string>
#include<mmsystem.h>
#include<list>
#include<math.h>
#include<iostream>
#include<fstream>
#include<ctime>

using namespace std;

enum Client_Rect
{
	CLIENT_WIDTH = 800,
	CLIENT_LENGTH = 1000,
};

enum Unit_Number
{
	UNIT_ONE=1,
	UNIT_TWO=2,
	UNIT_THREE=3,
	BOSS_ONE=4,
	BOSS_TWO=5,
	BOSS_THREE=6,
	BOSS_FOUR=7,
	BOSS_FIVE=8,
};
