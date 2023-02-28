//
// 파일명 : CTitle.cpp
// 
// 설명 : 타이틀 클래스의 정의
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#include <windows.h>
#include "CTitle.h"


//////////////////////////////////////////////////////////////////////////////////////////
// CTitle()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CTitle 클래스의 생성자.
//		  타이틀 버튼 객체들을 생성한다.
//		  타이틀화면에서의 BGM을 재생한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CTitle::CTitle(SceneParamaterBundle sceneParam) : CScene(sceneParam)
{
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Title\\BackGround.jpg", &tBackGround);
	newgameButton = new CTitleButton(480, 500, 150, 33, pd3dDevice, cursor, sprite, "Texture\\Title\\NewGameOn.jpg", "Texture\\Title\\NewGameOff.jpg");
	loadgameButton = new CTitleButton(480, 533, 150, 33, pd3dDevice, cursor, sprite, "Texture\\Title\\LoadGameOn.jpg", "Texture\\Title\\LoadGameOff.jpg");
	exitButton = new CTitleButton(480, 566, 150, 34, pd3dDevice, cursor, sprite, "Texture\\Title\\ExitOn.jpg", "Texture\\Title\\ExitOff.jpg");
	dshow->Initialize(hWnd, TITLEBGMPATH);
	dshow->Run();
	FadeOut();
}


//////////////////////////////////////////////////////////////////////////////////////////
// ~CTitle()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CTitle 클래스의 소멸자.
//		  텍스쳐를 해제한다.
//		  타이틀 버튼 객체들을 해제한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CTitle::~CTitle()
{
	tBackGround->Release();
	delete newgameButton;
	delete loadgameButton;
	delete exitButton;
}


//////////////////////////////////////////////////////////////////////////////////////////
// void FrameMove()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 내부 데이터 처리를 하는 함수
//		  CScene클래스의 가상함수를 오버라이드한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CTitle::FrameMove()
{
	BgmRepeatCheck();

	//버튼에 따라 실행
	if (newgameButton->MouseOnCheck() && cursor->LButtonClick())
	{
		PlaySound("Sound\\Click.wav", 0, SND_FILENAME | SND_ASYNC);
		FadeIn();
		sceneKind = EVENT;
	}
	else if (loadgameButton->MouseOnCheck() && cursor->LButtonClick())
	{
		PlaySound("Sound\\Click.wav", 0, SND_FILENAME | SND_ASYNC);
		FadeIn();
		//아직 세이브, 로드기능 미제공. 추후 제작시엔 CData클래스에서 데이터를 가져와서 넣어주는 형식으로 제작
	}
	else if (exitButton->MouseOnCheck() && cursor->LButtonClick())
	{
		PlaySound("Sound\\Click.wav", 0, SND_FILENAME | SND_ASYNC);
		FadeIn();
		PostQuitMessage(0);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void Render()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 렌더링 처리를 하는 함수
//		  CScene클래스의 가상함수를 오버라이드한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CTitle::Render()
{
	DrawTexture(tBackGround, 0, 0, 0, 0, screenWidth, screenHeight, 100);
	newgameButton->Draw();
	loadgameButton->Draw();
	exitButton->Draw();
}