//
// 파일명 : CBattle.cpp
// 
// 설명 : CBattle 클래스의 정의
//		  
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#include "CBattle.h"
#pragma warning(disable : 4996)


//////////////////////////////////////////////////////////////////////////////////////////
// CBattle(SceneParamaterBundle sceneParam)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CBattle 클래스의 생성자.
//		  맵 객체, 플레이어 객체, 추격자 객체, 도망자 객체를 바인딩한다.
//		  스크립트파일을 읽어와서 여러가지 정보를 읽어온다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CBattle::CBattle(SceneParamaterBundle sceneParam) : CScene(sceneParam)
{
	char buffer[128];
	FILE* fp = fopen(pData->GetScriptFilePath(), "r");

	//전투맵 생성
	fscanf(fp, "%s", buffer);
	int x = atoi(buffer);
	fscanf(fp, "%s", buffer);
	int y = atoi(buffer);
	fscanf(fp, "%s", buffer);
	bMap = new CBattleMap(pd3dDevice, sprite, cursor, x, y, screenWidth, screenHeight, buffer);

	//충돌배열 입력
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			fscanf(fp, "%s", buffer);
			bMap->SetCollideMap(j, i, atoi(buffer));
		}
	}

	//플레이어 정보 입력
	fscanf(fp, "%s", buffer);
	x = atoi(buffer);
	fscanf(fp, "%s", buffer);
	y = atoi(buffer);
	fscanf(fp, "%s", buffer);
	int m = atoi(buffer);
	player = new CPlayer(pd3dDevice, sprite, bMap, x, y, m, cursor);

	//추격자 정보 입력
	fscanf(fp, "%s", buffer);
	x = atoi(buffer);
	fscanf(fp, "%s", buffer);
	y = atoi(buffer);
	fscanf(fp, "%s", buffer);
	m = atoi(buffer);
	bChaser = new CEnemy(pd3dDevice, sprite, bMap, x, y, m, player, true);

	//도망자 정보 입력
	fscanf(fp, "%s", buffer);
	x = atoi(buffer);
	fscanf(fp, "%s", buffer);
	y = atoi(buffer);
	fscanf(fp, "%s", buffer);
	m = atoi(buffer);
	bFugitive = new CEnemy(pd3dDevice, sprite, bMap, x, y, m, player, false);

	fclose(fp);

	dshow->Initialize(hWnd, "Music\\Team_2_Battle_Theme_Track08.mp3"); //현재 전투가 하나밖에 없어서 그냥 적었지만 추후 스크립트에서 배경음악정하도록 수정
	dshow->Run();
}


//////////////////////////////////////////////////////////////////////////////////////////
// ~CBattle()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CBattle 클래스의 소멸자.
//		  객체들을 해제한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CBattle::~CBattle()
{
	delete bMap;
	delete player;
	delete bChaser;
	delete bFugitive;
}


//////////////////////////////////////////////////////////////////////////////////////////
// void FrameMove()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 내부 데이터 처리를 하는 함수.
//		  CScene클래스의 가상함수를 오버라이드한다.
//		  각 객체들의 행동함수를 호출한다.
//		  승리와 패배 체크를 한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CBattle::FrameMove()
{
	BgmRepeatCheck();

	player->TakeAction();
	bChaser->TakeAction();
	bFugitive->TakeAction();
	bMap->TakeAction();
	CheckWin();
	CheckLose();
}


//////////////////////////////////////////////////////////////////////////////////////////
// void Render()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 내부 데이터 처리를 하는 함수.
//		  CScene클래스의 가상함수를 오버라이드한다.
//		  각 객체들의 렌더링 함수를 호출한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CBattle::Render()
{
	bMap->Draw();
	player->Draw();
	bChaser->Draw();
	bFugitive->Draw();
}


//////////////////////////////////////////////////////////////////////////////////////////
// void CheckWin()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 게임에서 승리했는지를 체크하는 함수.
//		  본 미니게임에서 승리란 플레이어와 도망자의 좌표가 같아지는 시점을 말한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CBattle::CheckWin()
{
	if (player->GetCharX() == bChaser->GetCharX() && player->GetCharY() == bChaser->GetCharY())
	{
		//게임클리어
		MessageBox(hWnd, "게임에서 패배하였습니다!", "패배!", MB_OK);
		PostQuitMessage(0);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void CheckLose()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 게임에서 패배했는지를 체크하는 함수.
//		  본 미니게임에서 패배란 플레이어와 추격자의 좌표가 같아지는 시점을 말한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CBattle::CheckLose()
{
	if (player->GetCharX() == bFugitive->GetCharX() && player->GetCharY() == bFugitive->GetCharY())
	{
		//게임오버
		MessageBox(hWnd, "게임에서 승리하였습니다!", "승리!", MB_OK);
		PostQuitMessage(0);
	}
}