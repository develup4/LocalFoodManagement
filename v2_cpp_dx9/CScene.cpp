//
// ���ϸ� : CScene.cpp
// 
// ���� : CScene Ŭ���� ����
//
// �輺��(lonkth@naver.com)
// 2010.01.20 ���� ����
//


#include "CScene.h"


//////////////////////////////////////////////////////////////////////////////////////////
// CScene(SceneParamaterBundle sceneParam)
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : CScene Ŭ������ ������.
//		  ��ӹ��� Ŭ�����鿡�� ���������� ����� �����͵��� �޴´�.
//
// 2010.01.20 ��������
//////////////////////////////////////////////////////////////////////////////////////////

CScene::CScene(SceneParamaterBundle sceneParam) : screenWidth(sceneParam.screenWidth), screenHeight(sceneParam.screenHeight)
{
	hWnd = sceneParam.hWnd;
	hInst = sceneParam.hInst;
	pD3D = sceneParam.pD3D;
	pd3dDevice = sceneParam.pd3dDevice;
	sceneKind = sceneParam.sceneKind;
	pData = sceneParam.pData;
	pFont = sceneParam.pFont;

	D3DXCreateSprite(pd3dDevice, &sprite);
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\Black.jpg", &tBlack); //1024*768 ũ���� �ò��� �̹����� FadeInOut�� ����� �������� �����ϰ� ����
	cursor = new CCursor(pd3dDevice, hWnd, sprite);
	dshow = new CDXShow;
}


//////////////////////////////////////////////////////////////////////////////////////////
// ~CScene()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : CScene Ŭ������ �Ҹ���.
//		  �Ҵ��� �����͵��� �����Ѵ�.
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

CScene::~CScene()
{
	sprite->Release();
	tBlack->Release();
	delete cursor;
	delete dshow;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void DrawTexture(LPDIRECT3DTEXTURE9 pTexture, int x, int y, int x1, int y1, int x2, int y2, int alpha)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : �ؽ��ĸ� �ѷ��ִ� �����Լ�.
//		  sprite�� Draw�Լ��� �����ϰ� �̿��ϱ� ���� ���� �Լ��̴�.
//
// ���� : ������� �ؽ���, �������𼭸��� x��ǥ, �������𼭸��� y��ǥ,
//		  �ؽ��Ŀ����� �������𼭸� x��ǥ, �������𼭸� y��ǥ,
//		  �����ʾƷ��𼭸� x��ǥ, �����ʾƷ��𼭸� y��ǥ,
//		  ������(0~100)
//
// 2010.01.03 ��������
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScene::DrawTexture(LPDIRECT3DTEXTURE9 pTexture, int x, int y, int x1, int y1, int x2, int y2, int alpha)
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	RECT rc = {x1, y1, x2, y2};
	D3DXVECTOR3 pos((float) x, (float) y, 0);
	sprite->Draw(pTexture, &rc, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, 255 * alpha / 100));
	sprite->End();
}


//////////////////////////////////////////////////////////////////////////////////////////
// void FadeIn()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : ȭ���� ���̵��� ��Ű�� �Լ�
//		  �񵿱������� �۵�(Render()�� ��ġ���ʰ� ���⼭ ��� ����)
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

void CScene::FadeIn()
{
	for (int step = 0; step < 100; step++) //100�ܰ�(%)
	{
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			Render();
			DrawTexture(tBlack, 0, 0, 0, 0, screenWidth, screenHeight, step);
			pd3dDevice->EndScene();
		}
		pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
	Sleep(200);
}


//////////////////////////////////////////////////////////////////////////////////////////
// void FadeOut()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : ȭ���� ���̵�ƿ� ��Ű�� �Լ�
//		  �񵿱������� �۵�(Render()�� ��ġ���ʰ� ���⼭ ��� ����)
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

void CScene::FadeOut()
{
	for (int step = 0; step < 100; step++) //100�ܰ�(%)
	{
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			Render();
			DrawTexture(tBlack, 0, 0, 0, 0, screenWidth, screenHeight, 100 - step);
			pd3dDevice->EndScene();
		}
		pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
	Sleep(200);
}


//////////////////////////////////////////////////////////////////////////////////////////
// void BgmRepeatCheck()
//////////////////////////////////////////////////////////////////////////////////////////
//
// ���� : ��������� �ݺ��ؼ� ����ǵ��� �ϴ� �Լ�
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

void CScene::BgmRepeatCheck()
{
	long evCode;
	long param1;
	long param2;

	dshow->pEvent->GetEvent(&evCode, &param1, &param2, 0);
	if (evCode == EC_COMPLETE)
		dshow->pPosition->put_CurrentPosition(0);
}


//////////////////////////////////////////////////////////////////////////////////////////
// int GetSceneKind() const
//////////////////////////////////////////////////////////////////////////////////////////
//
// ��  �� : ���� ����� ������ �����ϴ� �Լ�
// ���ϰ� : int sceneKind(���� ����� ������ ���� ����)
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

int CScene::GetSceneKind() const
{
	return sceneKind;
}


//////////////////////////////////////////////////////////////////////////////////////////
// CCursor* GetCursor() const
//////////////////////////////////////////////////////////////////////////////////////////
//
// ��  �� : ���콺 Ŀ�� ��ü�� �����͸� �����ϴ� �Լ�
// ���ϰ� : CCursor* cursor(���콺 Ŀ�� ��ü�� ������)
//
// 2010.01.03 ��������
//////////////////////////////////////////////////////////////////////////////////////////

CCursor* CScene::GetCursor() const
{
	return cursor;
}