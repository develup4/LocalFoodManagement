//
// 파일명 : CTitleButton.h
// 
// 설명 : 타이틀 버튼 클래스.
//		  마우스가 객체의 위치 위로 올라왔을때 이미지를 바꾼다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "CCursor.h"

class CTitleButton
{
private:
	int x;
	int y;
	int width;
	int height;
	CCursor* cursor;
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 tButtonOn;
	LPDIRECT3DTEXTURE9 tButtonOff;

protected:

public:
	CTitleButton(int x, int y, int width, int height, LPDIRECT3DDEVICE9 pd3dDevice, CCursor* cursor, LPD3DXSPRITE sprite, char* onFilePath, char* offFilePath);
	~CTitleButton();

	bool MouseOnCheck();
	void Draw();
};
