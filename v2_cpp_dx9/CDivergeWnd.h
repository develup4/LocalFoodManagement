//
// 파일명 : CDivergeWnd.h
// 
// 설명 : 이벤트에서 사용되는 분기창 객체를 위한 클래스.
//		  분기에 관련된 데이터(분기개수나 내용 등)를 가지고 있다.
//		  분기창 이미지 텍스쳐를 가지고 있다.
//		  분기에 따라 새로운 스크립트로 CData 클래스의 정보를 변경한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once
#include "CEventSprite.h"
#include "CData.h"
#include "CCursor.h"

class CDivergeWnd : public CEventSprite
{
private:
	int x;
	int y;
	char* text;
	char* scriptFilePath;
	int kindScript;

	LPDIRECT3DTEXTURE9 tDiverge;
	LPDIRECT3DTEXTURE9 tDivergeSelect;

	CCursor* cursor;
	LPD3DXFONT pFont;
	int* sceneKind;
	CData* pData;

protected:

public:
	CDivergeWnd(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CCursor* cursor, LPD3DXFONT pFont, int* sceneKind, CData* pData);
	~CDivergeWnd();

	void Draw();
	bool MouseOnCheck();

	//캡슐화
	void SetX(int val);
	void SetY(int val);
	void SetText(char* str);
	void SetScriptFilePath(char* path);
	void SetKindScript(int kind);
	char* GetScriptFilePath();
	int GetKindScript();
};