//
// 파일명 : CEnding.h
// 
// 설명 : 엔딩장면을 제어하는 클래스
//		  아직 미구현
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include "CScene.h"

class CEnding : public CScene
{
private:

protected:

public:
	CEnding(SceneParamaterBundle sceneParam);
	~CEnding();

	void FrameMove();
	void Render();
};