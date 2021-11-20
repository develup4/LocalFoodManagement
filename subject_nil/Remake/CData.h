//
// 파일명 : CData.h
// 
// 설명 : 게임내 정보를 저장하는 Entity 클래스
//		  현재는 사용중인 스크립트파일의 경로밖에 저장하지않고있지만
//		  앞으로 캐릭터의 정보나 이벤트 진행상황 등을 저장해둘 클래스이다.
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//


#pragma once

class CData
{
private:
	char* curScriptFilePath;

protected:

public:
	CData(char* scriptFilePath);
	~CData();

	//캡슐화 관련
	void SetScriptFilePath(char* FilePath);
	char* GetScriptFilePath() const;
};
