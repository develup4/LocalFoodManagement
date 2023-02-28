//
// 파일명 : CEventAnimImage.h
// 
// 설명 : 이벤트에서 사용되는 애니메이션 이미지 객체를 위한 클래스.
//		  애니메이션 이미지 정보를 가지고 있다.
//		  애니메이션 이미지 텍스쳐를 가지고 있다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include "CEventSprite.h"

class CEventAnimImage : public CEventSprite
{
private:
	int step;
	int x;
	int y;
	int frameWidth;
	int frameHeight;
	int numFrame; //애니메이션 스프라이트 프레임의 개수
	int animSpeed; //동일한 프레임이 반복해서 재생되는 횟수(즉, 낮을수록 빠르다)
	LPDIRECT3DTEXTURE9 tImage;
protected:

public:
	CEventAnimImage(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, LPD3DXFONT pFont);
	~CEventAnimImage();

	void Draw();

	//캡슐화
	void SetX(int val);
	void SetY(int val);
	void SetFrameWidth(int val);
	void SetFrameHeight(int val);
	void SetNumFrame(int val);
	void SetAnimSpeed(int val);
	void SetImage(char* FilePath);
};