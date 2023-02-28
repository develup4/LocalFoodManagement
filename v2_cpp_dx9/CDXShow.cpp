//
// ���ϸ� : CDXShow.cpp
// 
// ���� : CDXShow Ŭ������ ����
//
// �輺��(lonkth@naver.com)
// 2010.01.03 ���� ����
//


#include "CDXShow.h"
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)


//////////////////////////////////////////////////////////////////////////////////////////
// CDXShow(), ~CDXShow()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : ������, �Ҹ���
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

CDXShow::CDXShow(){}

CDXShow::~CDXShow()
{
	if(pControl) pControl->Release();
	pControl=NULL;
	if(pEvent) pEvent->Release();
	pEvent=NULL;
	if(pPosition) pPosition->Release();
	pPosition=NULL;
	if(pVolume) pVolume->Release();
	pVolume=NULL;
	if(pGraph) pGraph->Release();
	pGraph=NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool Initialize(HWND hWnd, char* FilePath)
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : MP3 ������ ��ο� ������ �ڵ��� ���ڷ� �޾ƿͼ� �ʱ�ȭ ��Ų��.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::Initialize(HWND hWnd, char* FilePath)
{
	//�����ڵ�� ��ȯ
	size_t nlen = strlen(FilePath) + 1;
	WCHAR* wFileName = (WCHAR*)malloc(sizeof(WCHAR)*nlen);
	mbstowcs(wFileName, FilePath, nlen);

	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, reinterpret_cast<void **>(&pGraph));
	pGraph->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>(&pControl));
	pGraph->QueryInterface(IID_IMediaEvent, reinterpret_cast<void **>(&pEvent));
	pGraph->QueryInterface(IID_IMediaPosition, reinterpret_cast<void **>(&pPosition));
	pGraph->QueryInterface(IID_IBasicAudio, reinterpret_cast<void **> (&pVolume));
	pGraph->RenderFile(wFileName, NULL);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool Run()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : MP3 ������ �����Ų��.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::Run()
{
	pControl->Run();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool Stop()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : MP3 ����� ������Ų��.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::Stop()
{
	pControl->Stop();
	pPosition->put_CurrentPosition(0);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool Pause()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : MP3 ����� �Ͻ�������Ų��.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::Pause()
{
	pControl->Pause();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool SetVolume()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : MP3 ��������� �����Ѵ�.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::SetVolume(float Volume)
{
	pVolume->put_Volume(100 * Volume - 10000);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
// bool SetBalance()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : ������ ����Ǵ� �¿���ġ�� �����Ѵ�.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

bool CDXShow::SetBalance(float Balance)
{
	pVolume->put_Balance(100 * Balance);
	return true;
}