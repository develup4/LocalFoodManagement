//
// 파일명 : CBattleChar.h
// 
// 설명 : 전투캐릭터들의 공통부분을 묶은 클래스.
//		  전투캐릭터들은 본 클래스를 상속받아서 사용한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "CBattleMap.h"

class CBattleChar
{
private:

protected:
	enum {FILLTP, MOVABLE, MOVE};
	enum {DOWN, LEFT, RIGHT, UP};

	LPDIRECT3DDEVICE9 pd3dDevice;
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 tChar;
	LPDIRECT3DTEXTURE9 tTPGage; //TP게이지바 전체
	LPDIRECT3DTEXTURE9 tTPG; //TP게이지만

	CBattleMap* bMap;

	//캐릭터 공통 데이터
	int x;
	int y;
	int tp;
	int movableCell;
	int step;
	int state;
	int direction;

	//캐릭터 공통 계산용 배열
	int** movableMap; //장애물 정보를 담기위한 2차원배열
	int* moveOrder; //이동경로를 저장하기 위한 배열
	int ptr;

	//공통 사용 함수
	void MoveCharacter(int objX, int objY, int n); //최단경로 계산함수
	bool Step(); //캐릭터 이동단계 처리

public:
	CBattleChar(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell);
	~CBattleChar();

	int GetCharX();
	int GetCharY();

	//인터페이스
	virtual void TakeAction() = 0;
	virtual void Draw() = 0;
	virtual void FindDistance(int x, int y, int n) = 0; //이동가능범위를 계산하는 함수
};