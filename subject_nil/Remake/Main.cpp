//////////////////////////////////////////////////////////////////////////////
//																			//
//	본 프로젝트는 DirectX9 SDK August 2007을 기준으로 제작되었습니다.		//
//  실행을 위해서는 이에 준하는 DirectX 버전을 설치하여 주시기 바랍니다.	//
//  또한 '휴먼아미체' 폰트가 설치되어 있어야 원활한 화면구성이 가능합니다.  //
//																			//
//////////////////////////////////////////////////////////////////////////////

//
// 파일명 : Main.cpp
// 
// 설명 : 프로그램의 시작점.
//		  필요한 라이브러리를 포함시킨다.
//		  윈도우 클래스의 객체를 생성한다.
//		  게임루프함수를 호출한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#include "CWindow.h"
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")

//DirectX Show 관련 라이브러리
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Strmiids.lib")


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//윈도우 생성에 필요한 최소한의 정보를 담은 구조체 선언
	WinArgsSetting setting = {
		"SubjectNil", //윈도우의 이름
		CS_HREDRAW | CS_VREDRAW, WS_POPUPWINDOW | WS_SYSMENU, //윈도우 옵션
		1024, 768 //윈도우의 사이즈
	};

	CWindow cWnd(setting, hInstance, lpszCmdParam, nCmdShow);
	cWnd.Loop(); //게임루프

	return 0;
}