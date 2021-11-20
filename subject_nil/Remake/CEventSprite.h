//
// 파일명 : CEventSprite.h
// 
// 설명 : 이벤트에서 사용되는 BOB(Bitmap OBject)들의 Base 클래스
//		  굳이 공통형을 지정하는 이유는 다형성을 이용해서 여러가지 이득을 얻기 위해서이다.
//		  상속받는 클래스들이 공통으로 갖는 요소들을 가지고 있다.
//		  상속받는 클래스들이 공통으로 사용하는 함수들을 가지고 있다.
//		  next라는 이름의 포인터를 가지고 있어서 그 자체로 링크드리스트가 된다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

class CEventSprite
{
private:
	
protected:
	LPDIRECT3DDEVICE9 pd3dDevice;
	LPD3DXSPRITE sprite;
	LPD3DXFONT pFont;

	bool doDraw;
	
public:
	CEventSprite* next; //CEventSprite형끼리 리스트로 연결될 수 있다

	CEventSprite(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, LPD3DXFONT pFont);
	~CEventSprite();

	void SetDoDraw(bool doing);
	void TextRender(char* text, int x1, int y1, int x2, int y2, int alpha);

	//순수가상함수(인터페이스)
	virtual void Draw() = 0;
};

// 지금은 같은 텍스쳐가 중복되어 사용되는 경우가 거의 없어서 상관없지만
// 만약에 그런 경우라면 객체가 각각 텍스쳐를 갖는거보다 텍스쳐들만 모아놓는 객체를 만들어서 불러다가 쓰면
// 메모리가 낭비되지 않는다. 생각보다 커서 심각해지면 제대로 진행이 안될수도 있음. 나중에 고려해볼것
// -> Texture Pool