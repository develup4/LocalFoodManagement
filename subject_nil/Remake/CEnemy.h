//
// 파일명 : CEnemy.h
// 
// 설명 : 인공지능으로 움직이는 적 캐릭터의 클래스
//		  CBattleChar 클래스를 상속한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once
#include "CPlayer.h"

class CEnemy : public CBattleChar
{
private:
	CPlayer* player; //추격 혹은 도망을 해야하므로 플레이어정보가 필요
	bool chase; //추격자인지를 나타내는 변수
	int** distance; //플레이어와의 거리에 사용하는 이차원 배열

protected:

public:
	CEnemy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell, CPlayer* player, bool chase);
	~CEnemy();

	void TakeAction();
	void Draw();

	void FindDistance(int x, int y, int n);
};