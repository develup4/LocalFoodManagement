//
// 파일명 : CDXShow.h
// 
// 설명 : DirectX Show 관련 클래스
//
// 김성후(lonkth@naver.com)
// 2010.01.03 최종 수정
//

#pragma once
#include "DShow.h"

class CDXShow
{
private:

protected:

public:
	IBasicAudio *pVolume;
	IMediaPosition *pPosition;
	IGraphBuilder * pGraph;
	IMediaControl *pControl;
	IMediaEvent *pEvent;

	CDXShow();
	virtual ~CDXShow();

	bool Initialize(HWND hWnd, char* FilePath);
	bool Pause();
	bool Stop();
	bool Run();
	bool SetBalance(float Balance); //음향의 좌우방향을 결정, 범위 -10000 ~ 10000
	bool SetVolume(float Volume); //범위 -10000 ~ 0
};