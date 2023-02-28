//
// 파일명 : CSpeakWnd.cpp
// 
// 설명 : 대화창 클래스의 정의
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//


#include "CSpeakWnd.h"
#pragma warning(disable : 4996)
#define DIALOGFIRSTLINE_Y 590


//////////////////////////////////////////////////////////////////////////////////////////////////////
// CSpeakWnd(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CCursor* cursor, LPD3DXFONT pFont)
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CSpeakWnd 클래스의 생성자.
//		  초기데이터를 설정한다.
//		  필요한 텍스쳐를 등록한다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeakWnd::CSpeakWnd(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CCursor* cursor, LPD3DXFONT pFont) : CEventSprite(pd3dDevice, sprite, pFont)
{
	this->cursor = cursor;
	this->doDraw = false;
	this->speakEnd = false;
	step = FADEIN;
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\SpeakWnd.tga", &tSpeakWnd);
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\Complete.tga", &tComplete);
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\NoneIllust.tga", &tIllust);

	//초기화
	index = 0;
	ptr = 0;
	speakWndAlpha = 0;
	illustAlpha = 0;
	completeStep = 0;
}


///////////////////////////////////////////////////////////////////////////////////////
// ~CSpeakWnd()
///////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CSpeakWnd 클래스의 소멸자.
//		  등록한 텍스쳐를 해제한다.
//
// 2010.01.03 최종수정
///////////////////////////////////////////////////////////////////////////////////////

CSpeakWnd::~CSpeakWnd()
{
	tSpeakWnd->Release();
	tComplete->Release();
	tIllust->Release();
}


///////////////////////////////////////////////////////////////////////////////////////
// void Draw()
///////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 객체를 렌더링하는 함수.
//		  각 단계마다 적절한 효과를 준다.
//
// 2010.01.20 최종수정
///////////////////////////////////////////////////////////////////////////////////////

void CSpeakWnd::Draw()
{
	if (doDraw)
	{
		int lineY = DIALOGFIRSTLINE_Y; //첫 대사라인의 Y값

		sprite->Begin(D3DXSPRITE_ALPHABLEND);

		//일러스트 렌더링
		RECT rcIllust = {0, 0, 480, 640};
		D3DXVECTOR3 posIllust(0, 128, 0);
		sprite->Draw(tIllust, &rcIllust, NULL, &posIllust, D3DCOLOR_RGBA(255, 255, 255, illustAlpha));

		//대화창 렌더링
		RECT rcSpeak = {0, 0, 982, 200};
		D3DXVECTOR3 posSpeak(21, 555, 0);
		sprite->Draw(tSpeakWnd, &rcSpeak, NULL, &posSpeak, D3DCOLOR_RGBA(255, 255, 255, speakWndAlpha));

		sprite->End();

		//이름 출력
		TextRender(name, 910, 565, 1024, 620, speakWndAlpha);

		if (step == FADEIN)
		{
			//페이드인 효과
			speakWndAlpha += 8;
			if (speakWndAlpha > 128) illustAlpha += 8;
			if (speakWndAlpha > 255) speakWndAlpha = 255;
			if (illustAlpha > 255) illustAlpha = 255;

			//페이드인 효과가 끝나면
			if (speakWndAlpha == 255 && illustAlpha == 255) step = SPEAK;
		} 
		else if (step == SPEAK)
		{
			//대사를 한글자씩 출력하는 효과
			
			//인덱스보다 앞 라인은 그대로 다 찍음
			for (int i = 0; i < index; i++)
			{
				if (strcmp(dialog[i], "NONE") != 0) //해당 라인에 대사가 없을때 스크립트에 NONE이 표시되어있다
					TextRender(dialog[i], 40, lineY + i * 25, 980, 740, 255);
			}

			//인덱스 라인은 한글자씩 출력
			if (strcmp(dialog[index], "NONE") != 0)
			{
				char temp[256];

				strncpy(temp, dialog[index], ptr);
				temp[ptr+1] = '\0';
				TextRender(temp, 40, lineY + index * 25, 980, 740, 255);
				ptr++;

				//현재 처리하는 라인이 다 끝나면
				if (ptr == strlen(dialog[index]))
				{
					ptr = 0;
					index++;

					//이게 마지막 라인이라면 종료
					if (index == 6 || strcmp(dialog[index], "NONE") == 0)
					{
						index = 0;
						step = SPEAKEND;
					}
				}
			}

			//딜레이
			if (cursor->LButtonClick() || cursor->RButtonClick())
				Sleep(5); //클릭하고있을경우 글이 빠르게 나오도록
			else Sleep(50);

		}
		else if (step == SPEAKEND)
		{
			//대사 출력
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(dialog[i], "NONE") != 0)
					TextRender(dialog[i], 40, lineY + i * 25, 980, 740, 255);
			}

			//대사완료표식 렌더링
			RECT rect = {16, 0, 32, 16};
			if ((completeStep / 10) % 2)
			{
				rect.left -= 16;
				rect.right -= 16;
			}

			sprite->Begin(D3DXSPRITE_ALPHABLEND);
			
			D3DXVECTOR3 pos(960, 715, 0);
			sprite->Draw(tComplete, &rect, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, 255));
			sprite->End();

			completeStep++;

			if(cursor->LButtonClick() || completeStep > 600) //마우스 왼쪽버튼이 눌러지거나 600프레임이 지나면 끝
			{
				completeStep = 0;
				step = FADEOUT;
				PlaySound("Sound\\TalkOk.wav", 0, SND_FILENAME | SND_ASYNC);
			}
		}
		else
		{
			cursor->SetCurState(NODRAW);

			//페이드아웃 효과
			illustAlpha -= 8;
			if (illustAlpha < 128) speakWndAlpha -= 8;
			if (illustAlpha < 0) illustAlpha = 0;
			if (speakWndAlpha < 0) speakWndAlpha = 0;

			//대사 출력
			for (int i = 0; i < 6; i++)
			{
				if (strcmp(dialog[i], "NONE") != 0)
					TextRender(dialog[i], 40, lineY + i * 25, 980, 740, illustAlpha);
			}

			//페이드아웃 효과가 끝나면
			if (speakWndAlpha == 0 && illustAlpha == 0)
			{
				speakEnd = true;

				//초기화
				tIllust->Release();
				D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\NoneIllust.tga", &tIllust);
				doDraw = false;
				step = FADEIN;

				strcpy(name, "");
				for (int i = 0; i < 6; i++)
				{
					strcpy(dialog[i], "");
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////
// void SetIllustTexture(char* textureFilePath)
///////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 일러스트 이미지를 등록하는 함수
//		  부분생성방식이 이런건가?
// 인자 : char* textureFilePath(일러스트 이미지의 파일 경로 문자열)
//
// 2010.01.03 최종수정
///////////////////////////////////////////////////////////////////////////////////////

void CSpeakWnd::SetIllustTexture(char* textureFilePath)
{
	tIllust->Release();
	D3DXCreateTextureFromFile(pd3dDevice, textureFilePath, &tIllust);
}


///////////////////////////////////////////////////////////////////////////////////////
// void SetName(char* name)
///////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 대화하는 캐릭터의 이름을 등록하는 함수
// 인자 : char* name(캐릭터의 이름 문자열)
//
// 2010.01.03 최종수정
///////////////////////////////////////////////////////////////////////////////////////

void CSpeakWnd::SetName(char* name)
{
	strcpy(this->name, name);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void SetDialog(char* dialog1, char* dialog2, char* dialog3, char* dialog4, char* dialog5, char* dialog6)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 대사를 등록하는 함수
// 인자 : 대사는 6라인까지 가능하며 인자들의 인덱스와 라인이 대응된다.
//
// 2010.01.03 최종수정
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeakWnd::SetDialog(char* dialog1, char* dialog2, char* dialog3, char* dialog4, char* dialog5, char* dialog6)
{
	strcpy(this->dialog[0], dialog1);
	strcpy(this->dialog[1], dialog2);
	strcpy(this->dialog[2], dialog3);
	strcpy(this->dialog[3], dialog4);
	strcpy(this->dialog[4], dialog5);
	strcpy(this->dialog[5], dialog6);
}


///////////////////////////////////////////////////////////////////////////////////////
// void SetSpeakEnd(bool end)
///////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 대사가 끝났는지를 저장하는 speakEnd변수를 조작하는 함수
//		  대사가 다 끝나고 CEvent 클래스에서 다음 명령어를 진행할때,
//		  다시 값을 false로 돌려놓기위해 사용한다.
// 인자 : bool end
//
// 2010.01.03 최종수정
///////////////////////////////////////////////////////////////////////////////////////

void CSpeakWnd::SetSpeakEnd(bool end)
{
	speakEnd = end;
}


///////////////////////////////////////////////////////////////////////////////////////
// bool GetSpeakEnd()
///////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 대사가 끝났는지를 저장하는 speakEnd변수를 얻어오는 함수
//			이 함수를 통해 CEvent 클래스와 통신한다.
// 리턴값 : bool speakEnd
//
// 2010.01.03 최종수정
///////////////////////////////////////////////////////////////////////////////////////

bool CSpeakWnd::GetSpeakEnd()
{
	return speakEnd;
}