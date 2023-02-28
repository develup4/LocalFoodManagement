//
// 파일명 : CSpeakWnd.h
// 
// 설명 : 이벤트에서 사용되는 대화창 객체를 위한 클래스이다.
//		  BOB(Bitmap OBject)의 일종이다.
//		  대화창과 일러스트, 대사완료이미지를 출력한다.
//		  대사내용을 흐르듯이 출력한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//

#pragma once
#include <string.h>
#include "CEventSprite.h"
#include "CCursor.h"

enum {FADEIN, SPEAK, SPEAKEND, FADEOUT}; //진행단계를 나타내는 상수

class CSpeakWnd: public CEventSprite
{
private:
	LPDIRECT3DTEXTURE9 tSpeakWnd;
	LPDIRECT3DTEXTURE9 tComplete;
	LPDIRECT3DTEXTURE9 tIllust;
	CCursor* cursor;

	int step; //중간임시변수
	bool speakEnd; //CEvent 클래스와 통신하기위해 사용하는 변수로 대사가 끝났는지를 저장하는 변수
	char name[32];
	char dialog[6][256]; //대사 한 라인이 256바이트를 넘지 않도록 스크립트 툴에서 제약을 가한다.

	//대사 처리 관련 함수
	int speakWndAlpha;
	int illustAlpha;
	int index; //현재 출력된 글자위치를 가리킴
	int ptr; //출력된 위치를 가리키는 변수
	int completeStep;

protected:
	
public:
	CSpeakWnd(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CCursor* cursor, LPD3DXFONT pFont);
	~CSpeakWnd();

	void Draw();

	bool GetSpeakEnd();
	void SetIllustTexture(char* textureFilePath);
	void SetName(char* name);
	void SetDialog(char* dialog1, char* dialog2, char* dialog3, char* dialog4, char* dialog5, char* dialog6);
	void SetSpeakEnd(bool end);
};

//줄넘김을 감안해서 자동으로 길이에 맞춰 대사를 자르는 기능을 생략했는데
//특별히 줄넘길때는 \n 처럼 특수 문자로 처리하면 되므로 나중에 기능을 수정하자