//
// 파일명 : CBattle.h
// 
// 설명 : 전투를 제어하는 클래스
//		  현재 맵과 플레이어, 추격자, 도망자 정도의 객체가 있으며,
//		  플레이어는 도망자를 잡으면(좌표가 같아지면) 승리하고,
//		  추격자가 플레이어를 잡으면(좌표가 같아지면) 패배하는 간단한 게임으로 대체되어있는 상태이다.
//		  본 클래스는 위의 각 객체를 포함하고 있다.
//		  본 클래스는 승리 혹은 패배를 체크한다.
//		  
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once
#include <stdio.h>
#include "CScene.h"
#include "CPlayer.h"
#include "CBattleMap.h"
#include "CEnemy.h"

class CBattle : public CScene
{
private:
	CBattleMap* bMap;
	CPlayer* player;
	CEnemy* bChaser;
	CEnemy* bFugitive;
protected:

public:
	CBattle(SceneParamaterBundle sceneParam);
	~CBattle();

	void FrameMove();
	void Render();

	void CheckWin();
	void CheckLose();
};