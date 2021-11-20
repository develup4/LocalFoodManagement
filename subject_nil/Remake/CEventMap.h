//
// 파일명 : CEventMap.h
// 
// 설명 : 이벤트에서 사용되는 맵 객체를 위한 클래스
//		  맵 이미지 정보를 가지고 있다.
//		  스크롤 정보를 가지고 있다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include "CEventSprite.h"

class CEventMap : public CEventSprite
{
private:
	LPDIRECT3DTEXTURE9 tMap;
	int scrollX;
	int scrollY;

	const int screenWidth;
	const int screenHeight;

protected:
	
public:
	CEventMap(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, LPD3DXFONT pFont, int screenWidth, int screenHeight);
	~CEventMap();
	
	//캡슐화 관련
	void SetMapTexture(char* filePath);
	int GetScrollX() const;
	void SetScrollX(int val);
	int GetScrollY() const;
	void SetScrollY(int val);

	//일반함수
	void Draw();
};
