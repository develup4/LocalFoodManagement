//
// 파일명 : CWindow.h
// 
// 설명 : 윈도우를 생성, 관리하는 클래스. 모든 클래스 중 가장 상위에 존재한다.
//		  CD3D클래스의 객체를 생성한다. 
//		  게임의 현재 장면을 통제하는 CScene클래스의 객체를 생성한다.
//		  게임내 데이터를 저장하는 CData클래스의 객체를 생성한다.
//		  윈도우 프로시저 함수에서 윈도우창의 이동을 처리한다.
//		  FPS를 계산해 표시하는 기능도 한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//

#pragma once
#include <Windows.h>
#include "CD3D.h"
#include "CData.h"
#include "CScene.h"
#include "CTitle.h"
#include "CEvent.h"
#include "CBattle.h"
#include "CEnding.h"

struct WinArgsSetting
{
	LPCTSTR winName;
	UINT winStyle;
	DWORD winForm;

	const int screenWidth;
	const int screenHeight;
};

class CWindow
{

private:
	HWND hWnd;
	HINSTANCE hInst;
	LPCTSTR winName;
	CD3D* d3d; //DirectX 정보들을 모은 객체

	const int screenWidth;
	const int screenHeight;

	//게임내용 관련
	CScene* scene; //현재 장면을 통제하는 객체로 추상클래스인 CScene에 바인딩하는 클래스에 따라 장면이 결정된다.
	CData* pData;

protected:

public:
	CWindow(WinArgsSetting setting, HINSTANCE hInstance, LPSTR CmdLine, int CmdShow);
	~CWindow();

	static LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void CheckFPS(HWND hWnd); //Frame Per Second를 계산하여 표시하는 함수
	void Loop();

	//게임내용 관련
	void ChangeScene(int scene); //장면이 변할때 scene객체를 해제시키고 새로 바인딩시키는 함수

	//캡슐화 관련
	HWND GetHWnd() const;
	HINSTANCE GetHInst() const;
};