//
// 파일명 : CTitle.h
// 
// 설명 : 타이틀 화면을 컨트롤하는 클래스
//		  타이틀 버튼 클래스를 포함한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#pragma once
#include "CScene.h"
#include "CTitleButton.h"

#define TITLEBGMPATH "Music\\Team_1_Sadness_Theme_sadness.mp3"

class CTitle : public CScene
{
private:
	LPDIRECT3DTEXTURE9 tBackGround;
	CTitleButton* newgameButton;
	CTitleButton* loadgameButton;
	CTitleButton* exitButton;

protected:

public:
	CTitle(SceneParamaterBundle sceneParam);
	~CTitle();

	void FrameMove();
	void Render();
};