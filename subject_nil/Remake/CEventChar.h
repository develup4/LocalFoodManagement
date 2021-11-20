//
// 파일명 : CEventChar.h
// 
// 설명 : 이벤트에서 사용되는 캐릭터 객체를 위한 클래스.
//		  캐릭터 정보를 가지고 있다.
//		  캐릭터 텍스쳐를 가지고 있다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include "CEventSprite.h"

enum {DOWN, LEFT, RIGHT, UP}; //순서엄수! 텍스쳐 이미지의 정보와 동일한 순서

class CEventChar : public CEventSprite
{
private:
	char id[64];
	int x;
	int y;
	int direction;
	LPDIRECT3DTEXTURE9 tImage;
	char texturePath[128];
	int step;

protected:

public:
	CEventChar(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, LPD3DXFONT pFont);
	~CEventChar();

	void Draw();
	void Step();
	void ChangeDirection(int direction);

	//캡슐화 관련
	void SetID(char* id);
	void SetX(int x);
	void SetY(int y);
	void SetDirection(char* direction);
	void SetTexturePath(char* path);
	char* GetID();
	int GetX();
	int GetY();
	int GetDirection();
	char* GetTexturePath();
};