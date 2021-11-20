//
// 파일명 : CEvent.h
// 
// 설명 : 이벤트 화면을 컨트롤하는 클래스
//		  CEventSprite형 비트맵 오브젝트들을 가지고 있으며,
//		  스크립트파일에서 명령어를 읽어와 그 객체들을 적절하게 조작한다.
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#pragma once

#include "CScene.h"
#include "CEventSprite.h"
#include "CEventMap.h"
#include "CSpeakWnd.h"
#include "CDivergeWnd.h"
#include "CEventStaticImage.h"
#include "CEventAnimImage.h"
#include "CEventChar.h"

typedef struct CommandNode
{
	char command[1024]; //스크립트 명령어 한 라인이 1024 바이트를 넘지 않도록 스크립트 제작툴 제작시 제약을 가하도록 하자
	CommandNode* next;
} CommandNode;

typedef struct ParameterNode
{
	char* parameter;
	ParameterNode* next;
} ParameterNode;

//3개의 링크드리스트가 존재하게된다.
//명령어라인 링크드리스트, 파라메터 링크드리스트 그리고 BitmapOBject끼리 연결된 리스트(연결순서는 Draw되는 순서와 같다)
//스프라이트 리스트는 드로우에서 이용

class CEvent : public CScene
{
private:
	//리스트요소
	CommandNode* cHead;
	CommandNode* cTail;
	CommandNode* ptr;
	ParameterNode* pHead;
	ParameterNode* pTail;

	//연결된 BOB를 구분할 기준점들로 고정적인 요소이므로 생성자에서 바로 바인딩시킨다
	CEventMap* map;
	CEventChar* charHead;
	CEventChar* charTail;
	CEventAnimImage* animImageHead;
	CEventAnimImage* animImageTail;
	CEventStaticImage* staticImageHead;
	CEventStaticImage* staticImageTail;
	CDivergeWnd* divergeHead;
	CDivergeWnd* divergeTail;
	CSpeakWnd* speakwnd;

	//만약 이런 Boolean타입 변수가 늘어난다면 MS에서 하는거처럼 비트로 묶어서 보내볼까?
	bool executeFlag; //현재 명령어가 실행중인지를 나타내는 변수
	bool drawSkip; //이번 루프에서 렌더링을 스킵할 것인지를 나타내는 변수
	bool flag; //내부처리용 임시변수

	//본 클래스 내부적으로만 사용할 함수
	char* PARAM(int index); //스크립트 명령어에서 인자로 받은 숫자번째의 파라메터를 문자열로 받아오는 함수
	int NPARAM(int index); //스크립트 명령어에서 인자로 받은 숫자번째의 파라메터를 정수형으로 받아오는 함수

protected:

public:
	CEvent(SceneParamaterBundle sceneParam);
	~CEvent();

	void FrameMove();
	void Render();

	void ReadScript(char* line); //다음 스크립트 명령어를 불러와서 파라메터 단위로 끊어주는 함수
	bool ExecuteScript(); //현재 가리키고있는 스크립트 명령어를 실행시키는 함수
};