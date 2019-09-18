#include "stdafx.h"
#include "Set.h"
#include "CEnemy_Manager.h"
#include "resource.h"
#include "Cursor.h"
#include "Play.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void CSet::Progress()
{
	Cursor.Move(pEnemy_Manager);
	Cursor.Put(pEnemy_Manager);	
	Cursor.Cut(pEnemy_Manager); //배치 유닛 지우기
	return;
}

void CSet::Render()
{
	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_LENGTH);
	SelectObject(MainDC, hMainBitmap);

	HDC BackDC = CreateCompatibleDC(hdc);
	HBITMAP hBackBitmap = CreateCompatibleBitmap(hdc, Back_Ground.nWidth, Back_Ground.nLength);
	SelectObject(BackDC, hBackBitmap);

	HDC MemDC = CreateCompatibleDC(hdc);
	SelectObject(MemDC, Back_Ground.hBit);

	BitBlt(BackDC, 0, (Cursor.nMap_Y%Back_Ground.nLength), Back_Ground.nWidth, Back_Ground.nLength, MemDC, 0, 0, SRCCOPY);
	BitBlt(BackDC, 0, 0, Back_Ground.nWidth, (Cursor.nMap_Y%Back_Ground.nLength), MemDC, 0, Back_Ground.nLength - (Cursor.nMap_Y%Back_Ground.nLength), SRCCOPY);

	StretchBlt(MainDC, 0, 0, CLIENT_WIDTH, CLIENT_LENGTH, BackDC, 0, 0, Back_Ground.nWidth, Back_Ground.nLength, SRCCOPY);

	//커서
	Cursor.Paint(MainDC);

	//배경에 깔린 유닛들
	pEnemy_Manager->Paint(MainDC);

	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_LENGTH, MainDC, 0, 0, SRCCOPY);


	DeleteDC(BackDC);
	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);
	DeleteObject(hBackBitmap);
	DeleteDC(MemDC);

return;
}

void CSet::Setting(HWND hWnd, HINSTANCE hInst, CEnemy_Manager *Enemy_Manager, CPlay &Play)
{
	hwnd = hWnd;
	hinst = hInst;
	hdc = GetDC(hwnd);
	Cursor.Unit_Image_Set(hinst);
	pPlay = &Play;
	pEnemy_Manager = Enemy_Manager;
	pEnemy_Manager->Unit_Image_Setting(hInst, hWnd);

	Back_Ground.hBit = (HBITMAP)LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
	BITMAP Image_Rect;
	GetObject(Back_Ground.hBit, sizeof(BITMAP), &Image_Rect);
	Back_Ground.nWidth = Image_Rect.bmWidth;
	Back_Ground.nLength = Image_Rect.bmHeight;
	return;
}

void CSet::Play_Start()
{
	LPCSTR ch;
	ch = Bgm_File_Name.c_str();
	sndPlaySoundA(ch, SND_ASYNC | SND_NODEFAULT | SND_LOOP);

	//play 클래스 초기화
	pPlay->Play_Init(pEnemy_Manager);

	b_Ex = true;
	return;
}

void CSet::Play_End()
{
	pPlay->Play_Exit();
	b_Ex = false;
	return;
}

void CSet::Set_Back()
{	
	OPENFILENAME OFN;
	char File_Path[256] = "", File_Name[256] = "";
	ZeroMemory(File_Path, sizeof(File_Path)); ZeroMemory(&OFN, sizeof(OFN));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = "BITMAP Files(*.*)\0*.bmp\0All Files (*.*)\0*.*\0";
	OFN.lpstrFile = File_Path;
	OFN.nMaxFile = sizeof(File_Path);
	if (!GetOpenFileName(&OFN) == 0)
	{
		Map_File_Name = File_Path;
		Back_Ground.hBit = (HBITMAP)LoadImage(NULL, TEXT(Map_File_Name.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		BITMAP Image_Rect;
		GetObject(Back_Ground.hBit, sizeof(BITMAP), &Image_Rect);
		Back_Ground.nWidth = Image_Rect.bmWidth;
		Back_Ground.nLength = Image_Rect.bmHeight;

	}
	return;
}

void CSet::Set_Bgm()
{
	OPENFILENAME OFN;
	char File_Path[256] = "", File_Name[256] = "";
	ZeroMemory(&OFN, sizeof(OFN));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = "WAV Files(*.*)\0*.wav\0All Files (*.*)\0*.*\0";
	OFN.lpstrFile = File_Path;
	OFN.nMaxFile = sizeof(File_Path);
	if (!GetOpenFileName(&OFN) == 0)
	{
		Bgm_File_Name = File_Path;
	}
	return;
}

void CSet::Save()
{
	TCHAR File_Path[260] = _T("");
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);

	ofn.hwndOwner = hwnd;

	ofn.lpstrFile = File_Path;

	ofn.nMaxFile = sizeof(File_Path);

	ofn.lpstrFilter = _T("XML Files(*.xml)\0*.xml\0All Files (*.*)\0*.*\0");

	ofn.nFilterIndex = 1;

	ofn.lpstrFileTitle = NULL;

	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = "xml"; //디폴트 확장자
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn) == 0)
	{
		ofstream outFile;
		string stage_file_name;
		stage_file_name = File_Path; //저장할 폴더의 경로
		string replaceString = "txt";

		int pos;
		while ((pos = stage_file_name.find("xml")) != string::npos)
		{
			stage_file_name.replace(pos, replaceString.length(), replaceString);
		}

		outFile.open(stage_file_name);


		outFile << Map_File_Name << endl;
		outFile << Bgm_File_Name << endl;

		outFile.close();
		//여기까지 txt파일로 배경음, 배경이미지 경로 저장



		tinyxml2::XMLDocument doc;

		tinyxml2::XMLNode* m_pNode;
		m_pNode = doc.NewElement("root");
		doc.InsertFirstChild(m_pNode);

		list<Enemy*> Save_Enemy_List;
		pEnemy_Manager->Enemy_Copy(&Save_Enemy_List);
		outFile << Save_Enemy_List.size()<<endl;

		for (list<Enemy*>::iterator iter = Save_Enemy_List.begin(); iter != Save_Enemy_List.end();)
		{
			XMLElement* Par_Element = doc.NewElement("enemy");

			XMLElement* m_pElement = doc.NewElement("Unit_Number");
			m_pElement->SetText((*iter)->nUnit_Number);
			Par_Element->InsertEndChild(m_pElement);
			Par_Element->LinkEndChild(m_pElement);

			m_pElement = doc.NewElement("Location_X");
			m_pElement->SetText((*iter)->fLocation_X);
			Par_Element->InsertEndChild(m_pElement);
			Par_Element->LinkEndChild(m_pElement);

			m_pElement = doc.NewElement("Location_Y");
			m_pElement->SetText((*iter)->fLocation_Y);
			Par_Element->InsertEndChild(m_pElement);
			Par_Element->LinkEndChild(m_pElement);

			m_pElement = doc.NewElement("Move_Pattern");
			m_pElement->SetText((*iter)->nMove_Pattern);
			Par_Element->InsertEndChild(m_pElement);
			Par_Element->LinkEndChild(m_pElement);

			m_pElement = doc.NewElement("Shoot_Pattern");
			m_pElement->SetText((*iter)->nShoot_Pattern);
			Par_Element->InsertEndChild(m_pElement);
			Par_Element->LinkEndChild(m_pElement);

			Par_Element->InsertEndChild(Par_Element);

			m_pNode->LinkEndChild(Par_Element);

			delete (*iter);
			iter = Save_Enemy_List.erase(iter);
		}
		
		doc.SaveFile(File_Path);
	}

	return;
}

void CSet::Load()
{
	OPENFILENAME OFN;
	char File_Path[256]="";
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = _T("XML Files(*.xml)\0*.xml\0All Files (*.*)\0*.*\0");
	OFN.lpstrFile = File_Path;
	OFN.nMaxFile = sizeof(File_Path);
	if (!GetOpenFileName(&OFN) == 0)
	{
		string stage_file_name;
		stage_file_name = File_Path; //저장할 폴더의 경로
		
		//유닛 리스트를 로드
		pEnemy_Manager->Unit_Load(stage_file_name); //여기는  xml파싱



		string replaceString = "txt";

		int pos;
		while ((pos = stage_file_name.find("xml")) != string::npos)
		{
			stage_file_name.replace(pos, replaceString.length(), replaceString);
		}

		ifstream inFile;
		inFile.open(stage_file_name);

		if (!inFile) // 입력한 텍스트 파일이 없을경우 
		{
			inFile.close();
			return;
		}
		else
		{	//입력한 텍스트 파일이 있다면 배경맵과 배경음을 로드한다.	
			Map_File_Name.clear();
			Bgm_File_Name.clear();
			char chMap_File_Name[256] = "", chBgm_File_Name[256]="";
			inFile.getline(chMap_File_Name, sizeof(chMap_File_Name));
			inFile.getline(chBgm_File_Name, sizeof(chBgm_File_Name));
			Map_File_Name = chMap_File_Name;
			Bgm_File_Name = chBgm_File_Name;

			Back_Ground.hBit = (HBITMAP)LoadImage(NULL, TEXT(Map_File_Name.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (!Back_Ground.hBit)
			{
				Back_Ground.hBit = (HBITMAP)LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP1));
			}
			BITMAP Image_Rect;
			GetObject(Back_Ground.hBit, sizeof(BITMAP), &Image_Rect);
			Back_Ground.nWidth = Image_Rect.bmWidth;
			Back_Ground.nLength = Image_Rect.bmHeight;
			
			inFile.close();
		}	
	}
	
	return;
}

CSet::CSet()
{
	Map_File_Name.clear();
	Bgm_File_Name.clear();
	b_Ex = false;

}


CSet::~CSet()
{
	ReleaseDC(hwnd, hdc);
}
