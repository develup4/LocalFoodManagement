//
// 파일명 : CEvent.cpp
// 
// 설명 : 이벤트 클래스의 정의
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#include "CEvent.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4996)



//////////////////////////////////////////////////////////////////////////////////////////
// CEvent()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CEvent 클래스의 생성자.
//		  헤더에서 언급한 3가지 링크드리스트를 초기화한다.
//		  스크립트 파일을 읽어와서 명령어 링크드리스트에 저장한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CEvent::CEvent(SceneParamaterBundle sceneParam) : CScene(sceneParam)
{
	//나중에 STL쓰게되면 STL로 싹 바꾸자

	executeFlag = false;
	drawSkip = false;
	flag = false;
	cursor->SetCurState(NODRAW);

	//명령어 라인단위로 받아오기
	cHead = new CommandNode;
	cTail = new CommandNode;
	cHead->next = cTail;
	cTail->next = cTail;
	ptr = cHead;

	FILE* pScriptFile = fopen(pData->GetScriptFilePath(), "r");
	while(feof(pScriptFile) == 0)
	{	
		CommandNode* line = new CommandNode;
		fgets(line->command, sizeof(line->command), pScriptFile); //스크립트 파일에서 한 라인이 명령어 한라인으로 대응됨
		line->next = ptr->next;
		ptr->next = line;
		ptr = line;
	}
	fclose(pScriptFile);
	ptr = cHead->next;

	//파라메터 리스트 초기화
	pHead = new ParameterNode;
	pTail = new ParameterNode;
	pHead->next = pTail;
	pTail->next = pTail;

	//고정요소 바인딩
	map = new CEventMap(pd3dDevice, sprite, pFont, screenWidth, screenHeight);
	charHead = new CEventChar(pd3dDevice, sprite, pFont);
	charTail = new CEventChar(pd3dDevice, sprite, pFont);
	animImageHead = new CEventAnimImage(pd3dDevice, sprite, pFont);
	animImageTail = new CEventAnimImage(pd3dDevice, sprite, pFont);
	staticImageHead = new CEventStaticImage(pd3dDevice, sprite, pFont);
	staticImageTail = new CEventStaticImage(pd3dDevice, sprite, pFont);
	divergeHead = new CDivergeWnd(pd3dDevice, sprite, cursor, pFont, &sceneKind, pData);
	divergeTail = new CDivergeWnd(pd3dDevice, sprite, cursor, pFont, &sceneKind, pData);
	speakwnd= new CSpeakWnd(pd3dDevice, sprite, cursor, pFont);
	
	map->next = charHead;
	charHead->next = charTail;
	charTail->next = animImageHead;
	animImageHead->next = animImageTail;
	animImageTail->next = staticImageHead;
	staticImageHead->next = staticImageTail;
	staticImageTail->next = speakwnd;
	speakwnd->next = divergeHead;
	divergeHead->next = divergeTail;
	divergeTail->next = NULL;

	//고정요소는 각 객체의 구분점일뿐 실제로 드로우되지 않으므로 그려지지않도록 설정해둔다
	CEventSprite* ptr = charHead;
	while(ptr != NULL)
	{
		ptr->SetDoDraw(false);
		ptr = ptr->next;
	}

	dshow->Initialize(hWnd, "Music\\Nil.mp3");
	dshow->Run(); //일단 무음이 재생되게해서 BgmRepeatCheck()가 이루어지고 있도록 한다
}


//////////////////////////////////////////////////////////////////////////////////////////
// ~CEvent()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CEvent 클래스의 소멸자.
//		  링크드리스트들을 해제한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

CEvent::~CEvent()
{
	//명령어라인 해제
	CommandNode* cTempStore;
	CommandNode* cTempPtr;

	cTempPtr = cHead->next;
	while (cTempPtr != cTail)
	{
		cTempStore = cTempPtr;
		cTempPtr = cTempPtr->next;
		delete cTempStore;
	}
	delete cHead;
	delete cTail;

	//파라메터 리스트 해제
	ParameterNode* pTempStore;
	ParameterNode* pTempPtr;

	pTempPtr = pHead->next;
	while (pTempPtr != pTail)
	{
		pTempStore = pTempPtr;
		pTempPtr = pTempPtr->next;
		delete pTempStore;
	}
	delete pHead;
	delete pTail;

	//스프라이트 해제
	CEventSprite* tempStore;
	CEventSprite* tempPtr = map; //map이 Head

	while (tempPtr != NULL)
	{
		tempStore = tempPtr;
		tempPtr = tempPtr->next;
		delete tempStore;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void FrameMove()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 내부 데이터 처리를 하는 함수
//		  CScene클래스의 가상함수를 오버라이드한다.
//		  배경음악이 반복해서 재생되도록 BgmRepeatCheck()를 호출한다.
//		  현재 명령어가 다 실행되었는지 executeFlag로 조사한뒤 다음 명령어를 읽어온다.
//		  현재 명령어를 실행시키고 실행이 완료되었는지를 executeFlag에 반환한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CEvent::FrameMove()
{
	BgmRepeatCheck();
	if (!executeFlag) ReadScript(ptr->command);
	executeFlag = ExecuteScript();
}


//////////////////////////////////////////////////////////////////////////////////////////
// void Render()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 렌더링 처리를 하는 함수
//		  CScene클래스의 가상함수를 오버라이드한다.
//		  drawSkip 플래그가 참이 아닌경우에 렌더링한다.
//		  그려질 객체들은 모두 CEventSprite를 상속받으므로 다형성을 이용한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CEvent::Render()
{
	if (!drawSkip)
	{
		//Draw
		CEventSprite* ptr = map;
		while (ptr != NULL)
		{
			ptr->Draw();
			ptr = ptr->next;
		}
	}
	else
	{
		drawSkip = false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// void ReadScript(char* line)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 명령어 한 라인을 파라메터 단위로 분리해 파라메터 리스트에 저장하는 함수
// 인자 : char* line(명령어 리스트에서 받아옴)
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

void CEvent::ReadScript(char* line)
{
	ParameterNode* pointer;
	ParameterNode* tempStore;

	//일단 이전 파라메터 정보 삭제(링크드 리스트 비우기)
	pointer = pHead->next;
	while (pointer != pTail)
	{
		tempStore = pointer;
		pointer = pointer->next;
		delete tempStore;
	}
	pHead->next = pTail;
	pTail->next = pTail;

	//토큰으로 끊어 보관
	ParameterNode* command = new ParameterNode;
	command->parameter = strtok(line, "#");
	pHead->next = command;
	command->next = pTail;
	pointer = command;

	char* chunk = "initialize"; //이렇게 안하면 아래 반복문을 무시하게 되므로
	while (chunk != NULL)
	{
		chunk = strtok(NULL, "#");
		if(chunk != NULL)
		{
			ParameterNode* param = new ParameterNode;
			param->parameter = chunk;
			param->next = pointer->next;
			pointer->next = param;
			pointer = param;
		}
	}
	ptr = ptr->next;
}


//////////////////////////////////////////////////////////////////////////////////////////
// char* PARAM(int index)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 인자로 받은 인덱스의 스크립트 명령어 파라메터를 문자열로 반환한다.
// 인  자 : int index(스크립트 명령어의 파라메터 인덱스)
// 리턴값 : 스크립트 명령어의 파라메터 문자열
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

char* CEvent::PARAM(int index)
{
	ParameterNode* temp = pHead;
	for(int i = 0; i < index; i++)
	{
		temp = temp->next;
	}
	return temp->parameter;
}


//////////////////////////////////////////////////////////////////////////////////////////
// int NPARAM(int index)
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 인자로 받은 인덱스의 스크립트 명령어 파라메터를 정수형으로 반환한다.
// 인  자 : int index(스크립트 명령어의 파라메터 인덱스)
// 리턴값 : 스크립트 명령어의 파라메터 문자열을 정수화 시킨 숫자
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

int CEvent::NPARAM(int index)
{
	ParameterNode* temp = pHead;
	for(int i = 0; i < index; i++)
	{
		temp = temp->next;
	}
	return atoi(temp->parameter);
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool ExecuteScript()
//////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 현재 가리키는 스크립트 명령어를 실행시킨다.
// 리턴값 : 현재 명령어가 한 사이클에 끝날경우 false를 반환
//			한 사이클에 다 끝나지 않는 경우 true를 반환
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////

bool CEvent::ExecuteScript()
{
	//계속해서 실행해야하는경우(즉, 한 루프에 실행이 다 끝나지 않는 경우) true를 반환한다.
	//많이 사용되는 순으로 정렬하자(최적화)

	//아래서부터 주석형식
	// #스크립트 명령어 형식
	// 스크립트 명령어 설명


	// #LoadMap#ImageFilePath#ScrollX#ScrollY#
	// 맵을 불러오는 스크립트 명령어
	if (strcmp(PARAM(1), "LoadMap") == 0) 
	{
		map->SetMapTexture(PARAM(2));
		map->SetScrollX(NPARAM(3));
		map->SetScrollY(NPARAM(4));
		drawSkip = true; //깜빡임 현상 방지

		return false;
	}

	// #FadeIn#
	// 화면을 페이드인 시키는 스크립트 명령어
	else if (strcmp(PARAM(1), "FadeIn") == 0)
	{
		FadeIn();
		drawSkip = true;

		return false;
	}

	// #FadeOut#
	// 화면을 페이드아웃 시키는 스크립트 명령어
	else if (strcmp(PARAM(1), "FadeOut") == 0)
	{
		FadeOut();

		return false;
	}

	// #PlayMusic#MusicFilePath#
	// 배경음악을 재생시킨다.
	else if (strcmp(PARAM(1), "PlayMusic") == 0)
	{
		dshow->Stop();
		dshow->Initialize(hWnd, PARAM(2));
		dshow->Run();

		return false;
	}

	// #StopMusic#
	// 배경음악을 정지시킨다.
	else if (strcmp(PARAM(1), "StopMusic") == 0)
	{
		dshow->Stop();
		drawSkip = true;

		return false;
	}

	// #PlaySound#SoundFilePath#
	// 효과음을 재생시킨다.
	else if (strcmp(PARAM(1), "PlaySound") == 0)
	{
		PlaySound(PARAM(2), 0, SND_FILENAME | SND_ASYNC);

		return false;
	}

	// #Delay#MiliSec#
	// 인자로 받아온 숫자만큼의 밀리초만큼 대기한다.
	else if (strcmp(PARAM(1), "Delay") == 0)
	{
		Sleep(NPARAM(2));

		return false;
	}

	// #FrameSkip#NumberOfFrames#
	// 인자로 받아온 숫자만큼의 프레임만큼 대기한다.
	else if (strcmp(PARAM(1), "FrameSkip") == 0)
	{
		static int count = 0;
		if (count < NPARAM(2))
		{
			count++;

			return true;
		}
		count = 0;

		return false;
	}

	// #Scroll#NewScrollX#NewScrollY#
	// 맵을 스크롤 시킨다.
	else if (strcmp(PARAM(1), "Scroll") == 0)
	{
		if (NPARAM(2) > map->GetScrollX()) map->SetScrollX(1);
		else if (NPARAM(2) < map->GetScrollX()) map->SetScrollX(-1);
		if (NPARAM(3) > map->GetScrollY()) map->SetScrollY(1);
		else if (NPARAM(3) < map->GetScrollY()) map->SetScrollY(-1);
			
		if (NPARAM(2) == map->GetScrollX() && NPARAM(3) == map->GetScrollY()) return false;

		return true;
	}

	// #ExitScript#NextScriptFilePath#KindOfScript(BSorES)#
	// 현재 스크립트를 종료시키고 인자로 받아온 경로의 스크립트를 실행시킨다.
	// BS는 Battle Script의 줄임말, ES는 Event Script의 줄임말이다.
	else if (strcmp(PARAM(1), "ExitScript") == 0)
	{
		FadeIn();
		pData->SetScriptFilePath(PARAM(2));
		(strcmp(PARAM(3), "BS") == 0) ? sceneKind = NEXTBATTLE : sceneKind = NEXTEVENT;
		Sleep(1000);

		return false;
	}

	// #StaticImage#X#Y#Width#Height#NumberOfDurationFrame(60FramePer1Sec)#ImageFilePath#
	// 애니메이션 되지않는 이미지를 그려준다.
	else if (strcmp(PARAM(1), "StaticImage") == 0)
	{
		CEventStaticImage* temp = new CEventStaticImage(pd3dDevice, sprite, pFont);
		CEventSprite* ptr = staticImageHead;
		temp->SetX(NPARAM(2));
		temp->SetY(NPARAM(3));
		temp->SetWidth(NPARAM(4));
		temp->SetHeight(NPARAM(5));
		temp->SetDuration(NPARAM(6));
		temp->SetImage(PARAM(7));

		while(ptr->next != staticImageTail)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
		temp->next = staticImageTail;

		return false;
	}

	// #AnimImage#X#Y#FrameWidth#FrameHeight#NumberOfFrame#AnimSpeed#ImageFilePath#
	// 애니메이션되는 이미지를 그려준다.
	else if (strcmp(PARAM(1), "AnimImage") == 0)
	{
		CEventAnimImage* temp = new CEventAnimImage(pd3dDevice, sprite, pFont);
		CEventSprite* ptr = animImageHead;
		temp->SetX(NPARAM(2));
		temp->SetY(NPARAM(3));
		temp->SetFrameWidth(NPARAM(4));
		temp->SetFrameHeight(NPARAM(5));
		temp->SetNumFrame(NPARAM(6));
		temp->SetAnimSpeed(NPARAM(7));
		temp->SetImage(PARAM(8));

		while(ptr->next != animImageTail)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
		temp->next = animImageTail;

		return false;
	}

	// #Speak#Name#illustImageFilePath(NONE)#Dialog1#Dialog2(NONE)#Dialog3(NONE)#Dialog4(NONE)#Dialog5(NONE)#Dialog6(NONE)#
	// 캐릭터의 대사를 출력시킨다.
	else if (strcmp(PARAM(1), "Speak") == 0)
	{
		if(!flag) //첫 루프에서는 등록
		{
			cursor->SetCurState(NCURSOR);
			speakwnd->SetName(PARAM(2));
			if (strcmp(PARAM(3), "NONE") != 0) speakwnd->SetIllustTexture(PARAM(3));
			speakwnd->SetDialog(PARAM(4), PARAM(5), PARAM(6), PARAM(7), PARAM(8), PARAM(9));
			speakwnd->SetDoDraw(true);
			flag = true;

			return true;
		}
		else //두번째 루프부터는 실행만한다
		{
			if (speakwnd->GetSpeakEnd())
			{
				speakwnd->SetSpeakEnd(false);
				flag = false;
				return false;
			}
			return true;
		}
	}

	// #Diverge#Number#Text1#Script1FilePath#KindOfScript1#Text2#Script2FilePath#KindOfScript2#......#
	// 분기를 발생시킨다.
	else if (strcmp(PARAM(1), "Diverge") == 0)
	{
		if(!flag) //첫 루프에서는 등록
		{
			cursor->SetCurState(SCURSOR);
			for (int i = 0; i < NPARAM(2); i++)
			{
				CDivergeWnd* temp = new CDivergeWnd(pd3dDevice, sprite, cursor, pFont, &sceneKind, pData);
				temp->SetX((screenWidth - 400) / 2); //400은 선택창의 너비
				temp->SetY((screenHeight - NPARAM(2) * 100) / 2 + i * 100);
				temp->SetText(PARAM(3 * i + 3)); //3, 6, 9,...번째의 파라메터들
				temp->SetScriptFilePath(PARAM(3 * i + 4)); //4, 7, 10,...번째의 파라메터들
				(strcmp(PARAM(3 * i + 5), "BS") == 0) ? temp->SetKindScript(NEXTBATTLE) : temp->SetKindScript(NEXTEVENT); //5, 8, 11,...번째의 파라메터들
				temp->next = divergeHead->next;
				divergeHead->next = temp;
			}
			flag = true;

			return true;
		}
		else //두번째 루프부터는 실행만한다.
		{
			CDivergeWnd* ptr = divergeHead;
			while (ptr->next != divergeTail)
			{
				ptr = (CDivergeWnd*)ptr->next;

				if (ptr->MouseOnCheck() && cursor->LButtonClick())
				{
					FadeIn();
					pData->SetScriptFilePath(ptr->GetScriptFilePath());
					sceneKind = ptr->GetKindScript();
					Sleep(1000);
					return false;
				}	
			}
			return true;
		}
	}

	// #RegChar#ID(String)#X#Y#Direction(RIGHT, LEFT, UP, DOWN)#SpriteFilePath#
	// 캐릭터를 등록시킨다.
	else if(strcmp(PARAM(1), "RegChar") == 0)
	{
		CEventChar* dummy = new CEventChar(pd3dDevice, sprite, pFont);
		dummy->SetID(PARAM(2));
		dummy->SetX(NPARAM(3));
		dummy->SetY(NPARAM(4));
		dummy->SetDirection(PARAM(5));
		dummy->SetTexturePath(PARAM(6));
		dummy->next = charHead->next;
		charHead->next = dummy;

		drawSkip = true;

		return false;
	}

	// #RemoveChar#ID(String)#
	// 캐릭터를 제거한다.
	else if(strcmp(PARAM(1), "RemoveChar") == 0)
	{
		CEventChar* ptr = charHead;
		while (ptr->next != charTail)
		{
			ptr = (CEventChar*)ptr->next; //next는 CEventSprite형이므로 형변환이 필요하다
			
			if (strcmp(PARAM(2), ptr->GetID()) == 0) //ID로 해당하는 캐릭터를 찾아서
				ptr->SetDoDraw(false);
		}
		drawSkip = true; //깜빡임 현상 방지

		return false;
	}

	// #ChangeCharDir#Number#ID1(String)#Direction1(RIGHT, LEFT, UP, DOWN)#ID2(String)#Direction2(RIGHT, LEFT, UP, DOWN)#......#
	// 캐릭터가 바라보는 방향을 변경한다.
	else if(strcmp(PARAM(1), "ChangeCharDir") == 0)
	{
		for (int i = 0; i < NPARAM(2); i++)
		{
			CEventChar* ptr = charHead;
			while (ptr->next != charTail)
			{
				ptr = (CEventChar*)ptr->next;

				if (strcmp(PARAM(2*i+3), ptr->GetID()) == 0) //ID로 해당하는 캐릭터를 찾아서
					ptr->SetDirection(PARAM(2*i+4));
			}
		}
		return false;
	}

	// #MoveChar#Number#ID1(String)#Direction1(RIGHT, LEFT, UP, DOWN)#ID2(String)#Direction2(RIGHT, LEFT, UP, DOWN)#......#
	// 캐릭터를 이동시킨다.
	else if(strcmp(PARAM(1), "MoveChar") == 0)
	{
		static int moveStep = 0;
		for (int i = 0; i < NPARAM(2); i++)
		{
			CEventChar* ptr = charHead;
			while (ptr->next != charTail)
			{
				ptr = (CEventChar*)ptr->next;
				if (strcmp(PARAM(2*i+3), ptr->GetID()) == 0) //ID로 해당하는 캐릭터를 찾아서
				{
					ptr->SetDirection(PARAM(2*i+4)); //등차수열
					ptr->Step();
				}
			}
		}
		moveStep++;
		if (moveStep == 4 * 10) //AnimSpeed : 캐릭터 스프라이트가 다음 프레임으로 넘어갈때까지의 간격으로 여기서는 10을 쓰고있다.
		{
			moveStep = 0;
			return false;
		}
		return true;
	}

	// #RelocateChar#ID(String)#X#Y#Direction(RIGHT, LEFT, UP, DOWN)
	// 캐릭터의 위치를 변경한다.
	else if(strcmp(PARAM(1), "RelocateChar") == 0)
	{
		CEventChar* ptr = charHead;
		while (ptr->next != charTail)
		{
			ptr = (CEventChar*)ptr->next;
			if (strcmp(PARAM(2), ptr->GetID()) == 0) //ID로 해당하는 캐릭터를 찾아서
			{
				ptr->SetX(NPARAM(3));
				ptr->SetY(NPARAM(4));
				ptr->SetDirection(PARAM(5));
			}
		}
		drawSkip = true;

		return false;
	}

	// 이 모든 경우에 해당하지 않는경우 제대로 작성되지 않은 스크립트 언어이므로 종료시킨다.
	// 이때 잘못된 스크립트파일명과 잘못된 라인을 로그로 기록하도록 추후 추가시키자.
	else
	{
		MessageBox(hWnd, "잘못된 스크립트명령어로 인해 종료합니다.", "Error!", MB_OK);
		PostQuitMessage(0);

		return false;
	}
}