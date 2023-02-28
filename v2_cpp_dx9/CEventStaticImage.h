//
// 파일명 : CEventStaticImage.h
// 
// 설명 : 이벤트에서 사용되는 정적인(애니메이션되지않는) 이미지 객체를 위한 클래스
//		  BOB(Bitmap OBject)의 일종이다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#pragma once
#include "CEventSprite.h"

class CEventStaticImage : public CEventSprite
{
private:
	int step;
	int x;
	int y;
	int width;
	int height;
	int duration;
	LPDIRECT3DTEXTURE9 tImage;

protected:

public:
	CEventStaticImage(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, LPD3DXFONT pFont);
	~CEventStaticImage();

	void Draw();

	//캡슐화
	void SetX(int val);
	void SetY(int val);
	void SetWidth(int val);
	void SetHeight(int val);
	void SetDuration(int val);
	void SetImage(char* FilePath);
};
