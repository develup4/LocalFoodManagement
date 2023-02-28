//
// 파일명 : CPlayer.h
// 
// 설명 : 사용자가 조작하는 플레이어 캐릭터의 클래스
//		  CBattleChar 클래스를 상속한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once
#include "CBattleChar.h"
#include "CCursor.h"

class CPlayer : public CBattleChar
{
private:
	CCursor* cursor;
	LPDIRECT3DTEXTURE9 tBlueCell;

protected:

public:
	CPlayer(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell, CCursor* cursor);
	~CPlayer();

	void TakeAction();
	void Draw();

	void FindDistance(int x, int y, int n);
};