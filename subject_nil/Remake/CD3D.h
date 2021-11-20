//
// 파일명 : CD3D.h
// 
// 설명 : DirectX 3D 관련 클래스
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include <d3d9.h>
#include <d3dx9.h>

//매크로 함수
#define SAFE_RELEASE(p) if(p){(p)->Release(); (p)=NULL;}
#define SAFE_DELETE(p) if(p){DELETE (p); (p)=NULL;}

class CD3D
{
private:
	LPDIRECT3D9 pD3D; //Direct3D 객체
	LPDIRECT3DDEVICE9 pd3dDevice; //Direct3D Device 객체
	LPD3DXFONT pFont;

protected:

public:
	CD3D(HWND hWnd);
	~CD3D();

	//캡슐화 관련
	LPDIRECT3D9 GetD3DPointer() const;
	LPDIRECT3DDEVICE9 GetD3DDevicePointer() const;
	LPD3DXFONT GetFontPointer() const;
};