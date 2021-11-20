//
// 파일명 : CScene.h
// 
// 설명 : 현재 장면을 관리, 통제하는 클래스.
//		  추상클래스로 이를 상속하는 구체클래스들은 각 장면에 대한 Control Class에 해당한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//

#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "CDXShow.h"
#include "CCursor.h"
#include "CData.h"

enum {TITLE, EVENT, BATTLE, ENDING, NEXTEVENT, NEXTBATTLE}; //장면의 종류를 나타내는 상수

struct SceneParamaterBundle
{
	HWND hWnd;
	HINSTANCE hInst;
	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pd3dDevice;
	LPD3DXFONT pFont;
	const int screenWidth;
	const int screenHeight;
	int sceneKind;
	CData* pData;
};

class CScene
{
private:

protected:
	//핸들
	HWND hWnd;
	HINSTANCE hInst;
	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pd3dDevice;

	const int screenWidth;
	const int screenHeight;
	int sceneKind; //현재 씬 종류를 담는 변수

	//공통사용객체
	LPDIRECT3DTEXTURE9 tBlack;
	CCursor* cursor;
	LPD3DXSPRITE sprite;
	LPD3DXFONT pFont;
	CDXShow* dshow;
	CData* pData;

	//상속받는 클래스에서 공통으로 사용가능한 함수
	void BgmRepeatCheck(); //BGM 반복재생 함수
	void FadeIn();
	void FadeOut();
	void DrawTexture(LPDIRECT3DTEXTURE9 pTexture, int x, int y, int x1, int y1, int x2, int y2, int alpha);

public:
	CScene(SceneParamaterBundle sceneParam);
	~CScene();

	//캡슐화 관련
	int GetSceneKind() const;
	CCursor* GetCursor() const;

	//순수가상함수(인터페이스)
	virtual void FrameMove() = 0;
	virtual void Render() = 0;
};
