//
// 파일명 : CWindow.cpp
// 
// 설명 : CWindow 클래스 정의
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//

#include "CWindow.h"
#pragma warning(disable : 4995)


//////////////////////////////////////////////////////////////////////////////////////////
// CWindow(WinArgsSetting setting, HINSTANCE hInstance, LPSTR CmdLine, int CmdShow)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CWindow 클래스의 생성자.
//		  윈도우를 생성한다.
//		  d3d객체와 scene객체, pData객체를 초기화시킨다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CWindow::CWindow(WinArgsSetting setting, HINSTANCE hInstance, LPSTR CmdLine, int CmdShow) : screenWidth(setting.screenWidth), screenHeight(setting.screenHeight)
{
	//윈도우 등록
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = CWindow::WndProc;
	WndClass.lpszClassName = setting.winName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = setting.winStyle;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(setting.winName, setting.winName, setting.winForm,
		(GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2, //윈도우의 초기위치를 화면중앙으로
		setting.screenWidth, setting.screenHeight, NULL, (HMENU) NULL, hInstance, NULL);

	hInst = hInstance;
	winName = setting.winName;

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
	ShowCursor(false); //윈도우 기본 커서는 숨기고 그 위치에 커서이미지를 출력

	//초기화
	d3d = new CD3D(hWnd);
	pData = new CData("Script\\ES\\e001.es"); //최초의 스크립트

	SceneParamaterBundle sceneParam = {
		hWnd, hInst, //윈도우 관련 핸들
		d3d->GetD3DPointer(), d3d->GetD3DDevicePointer(), d3d->GetFontPointer(), //D3D 관련 포인터
		screenWidth, screenHeight, //스크린의 크기
		TITLE, //현재 장면의 종류
		pData //데이터를 저장할 객체의 포인터
	};

	scene = new CTitle(sceneParam); //최초의 장면은 타이틀 화면이므로 scene객체에 CTitle클래스를 바인딩시킨다.
}


//////////////////////////////////////////////////////////////////////////////////////////
// ~CWindow()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CWindow 클래스의 소멸자.
//		  윈도우를 파괴한다.
//		  d3d객체와 scene객체, pData객체를 해제시킨다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CWindow::~CWindow()
{
	delete d3d;
	delete scene;
	delete pData;
	UnregisterClass(winName, hInst);
}


//////////////////////////////////////////////////////////////////////////////////////////
// void CheckFPS(HWND hWnd)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : FramePerSecond를 측정한다.
//		  측정된 FPS를 표시한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CWindow::CheckFPS(HWND hWnd)
{
	static DWORD tick = 0;
	static DWORD fps = 0;
	TCHAR temp[32];

	fps++;
	if (GetTickCount() - tick > 1000)
	{
		tick = GetTickCount();
		wsprintf(temp, "FPS : %d", fps);
		SetWindowText(hWnd, temp);
		fps = 0;
		return;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void Loop()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 게임루프함수
//		  게임이 진행되는한 계속해서 루프한다.
//		  각 루프마다 FrameMove()에서 렌더링을 제외한 내부데이터들을 처리한다.
//		  각 루프마다 Render()에서 오직 렌더링만을 처리한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CWindow::Loop()
{
	int prevScene = TITLE; //장면 변화를 감지하기 위하여 이전 장면의 종류를 저장하는 변수로, 첫 장면은 타이틀화면이므로 TITLE로 초기화
	MSG Message;
	ZeroMemory(&Message, sizeof(Message));

	//루프의 시작
	while(Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message); 
			DispatchMessage(&Message);
		}
		else //메시지가 없을때 처리
		{
			CheckFPS(hWnd);

			if (hWnd == GetFocus()) //현재 윈도우에 포커스가 되어있다면
				scene->FrameMove(); //내부데이터처리

			if (prevScene != scene->GetSceneKind()) //장면변화가 있었다면
				ChangeScene(scene->GetSceneKind()); //현재 장면으로 scene객체를 변경

			prevScene = scene->GetSceneKind(); //현재 장면이 다음 루프에서 이전 장면이 되므로 저장

			d3d->GetD3DDevicePointer()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			if (SUCCEEDED(d3d->GetD3DDevicePointer()->BeginScene()))
			{
				scene->Render(); //렌더링
				scene->GetCursor()->DrawCursor(); //마우스 커서 렌더링
				d3d->GetD3DDevicePointer()->EndScene();
			}
			d3d->GetD3DDevicePointer()->Present(NULL, NULL, NULL, NULL);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void ChangeScene(int sceneKind)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 장면을 바꾸는 함수
//		  기존의 scene객체를 해제시키고 새로운 장면 클래스로 바인딩한다.
//		  파라메터로 넘기는 정보는 변경되어서는 안되므로 Call By Value로 처리한다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CWindow::ChangeScene(int sceneKind)
{
	SceneParamaterBundle sceneParam = {
		hWnd, hInst, //윈도우 관련 핸들
		d3d->GetD3DPointer(), d3d->GetD3DDevicePointer(), d3d->GetFontPointer(), //D3D 관련 포인터
		screenWidth, screenHeight, //스크린의 크기
		TITLE, //현재 장면의 종류(여기선 단순히 초기화)
		pData //데이터를 저장할 객체의 포인터
	};

	switch(sceneKind)
	{
		case TITLE:
			{
				sceneParam.sceneKind = TITLE;

				delete scene;
				scene = new CTitle(sceneParam);
				break;
			}
		case EVENT:
		case NEXTEVENT:
			{
				sceneParam.sceneKind = EVENT;

				delete scene;
				scene = new CEvent(sceneParam);
				break;
			}
		case BATTLE:
		case NEXTBATTLE:
			{
				sceneParam.sceneKind = BATTLE;

				delete scene;
				scene = new CBattle(sceneParam);
				break;
			}
		case ENDING:
			{
				sceneParam.sceneKind = ENDING;

				delete scene;
				scene = new CEnding(sceneParam);
				break;
			}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 윈도우 프로시저 함수
//		  마우스 이벤트를 검사하여 윈도우창의 이동을 관장한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

LRESULT CWindow::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static bool rBtnDown = false;
	static int mouseX = 0;
	static int mouseY = 0;

	switch (iMessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_RBUTTONDOWN:
		{
			rBtnDown = true;
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			return 0;
		}
	case WM_RBUTTONUP:
		{
			rBtnDown = false;
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			RECT rect;

			if(rBtnDown && GetWindowRect(hWnd, &rect) == TRUE)
			{
				int offsetX;
				int offsetY;

				offsetX = LOWORD(lParam) - mouseX;
				offsetY = HIWORD(lParam) - mouseY;
				MoveWindow(hWnd, rect.left + offsetX, rect.top + offsetY, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			}
			return 0;
		}
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


//////////////////////////////////////////////////////////////////////////////////////////
// HWND GetHWnd() const
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 윈도우 핸들을 리턴받는 함수
// 리턴값 : HWND hWnd(윈도우 핸들)
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

HWND CWindow::GetHWnd() const
{
	return hWnd;
}


//////////////////////////////////////////////////////////////////////////////////////////
// HINSTANCE GetHInst() const
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 윈도우 인스턴스을 리턴받는 함수
// 리턴값 : HINSTANCE hInst(윈도우 인스턴스)
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE CWindow::GetHInst() const
{
	return hInst;
}