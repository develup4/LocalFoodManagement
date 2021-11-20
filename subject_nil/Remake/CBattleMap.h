//
// 파일명 : CBattleMap.h
// 
// 설명 : 전투맵을 제어하는 클래스
//		  충돌배열 등 전투맵에 필요한 데이터들을 가지고 있다.
//		  맵스크롤을 처리할 수 있다.
//		  맵 텍스쳐 이미지를 렌더링한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "CCursor.h"

class CBattleMap
{
private:
	LPDIRECT3DDEVICE9 pd3dDevice;
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 map;
	CCursor* cursor;
	int sx; //맵의 가로로 스크롤된 정도
	int sy; //맵의 세로로 스크롤된 정도
	int mapX; //맵의 가로길이
	int mapY; //맵의 세로길이
	int** collideMap; //충돌배열로 갈 수 있는 셀과 갈 수 없는 셀을 구분해놓은 이차원배열
	const int screenWidth;
	const int screenHeight;

protected:

public:
	CBattleMap(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CCursor* cursor, int mapX, int mapY, int screenWidth, int screenHeight, char* battleMapTexture);
	~CBattleMap();

	void TakeAction();
	void Draw();

	void SetCollideMap(int x, int y, int value);
	int GetSX();
	int GetSY();
	int GetMapX();
	int GetMapY();
	bool GetMovable(int x, int y);
};