//
// 파일명 : CCursor.h
// 
// 설명 : 마우스 커서를 관리하는 클래스.
//		  커서를 그린다.
//		  커서의 이미지를 변경할 수 있다.
//		  마우스의 클릭을 감지한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

enum {NCURSOR, SCURSOR, NODRAW}; //커서의 종류를 나타내는 상수, 순서대로 노멀커서, 선택커서, 그리지않음

class CCursor
{
private:
	HWND hWnd;
	LPD3DXSPRITE sprite;
	int curState;
	int cursorWidth;
	int cursorHeight;
	int animationSpeed; //동일한 프레임이 반복되는 횟수로 낮을수록 빠르게 애니메이션 된다.
	int step;
	int nFrame; //노멀 커서의 애니메이션 수
	int sFrame; //선택 커서의 애니메이션 수

	LPDIRECT3DTEXTURE9 tNCursor;
	LPDIRECT3DTEXTURE9 tSCursor;

protected:

public:
	CCursor(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, LPD3DXSPRITE sprite);
	~CCursor();

	void DrawCursor();
	bool RButtonClick();
	bool LButtonClick();

	//캡슐화 관련
	void SetCursorPoint(int x, int y);
	POINT GetCursorPoint() const;
	void SetCurState(int val);
	int GetCurState() const;
};