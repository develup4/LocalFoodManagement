//
// 파일명 : CBattleChar.cpp
// 
// 설명 : CBattleChar 클래스의 정의
//
// 김성후(lonkth@naver.com)
// 2010.01.20 최종 수정
//

#include "CBattleChar.h"


/////////////////////////////////////////////////////////////////////////////////////////
// CBattleChar()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CBattleChar 클래스의 생성자
//		  캐릭터의 공통부분을 초기화하는 함수
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

CBattleChar::CBattleChar(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell)
{
	this->pd3dDevice = pd3dDevice;
	this->sprite = sprite;
	this->bMap = bMap;
	this->x = x;
	this->y = y;
	this->movableCell = movableCell;
	tp = 0;
	step = 0;
	direction = DOWN;
	state = FILLTP;
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\TPGage.jpg", &tTPGage);
	D3DXCreateTextureFromFile(pd3dDevice, "Texture\\Common\\TPG.jpg", &tTPG);

	//배열 초기화
	moveOrder = new int[movableCell];
	for (int i = 0; i < movableCell; i++)
	{
		moveOrder[i] = -1;
	}
	movableMap = new int*[bMap->GetMapY()];
	for (int i = 0; i < bMap->GetMapY() ; i++)
	{
		movableMap[i] = new int[bMap->GetMapX()];
	}
	for(int i = 0; i < bMap->GetMapX(); i++)
	{
		for(int j = 0; j < bMap->GetMapY(); j++)
		{
			movableMap[j][i] = 0;
		}
	}
	ptr = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
// ~CBattleChar()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CBattleChar 클래스의 소멸자
//		  캐릭터의 공통부분을 해제하는 함수
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

CBattleChar::~CBattleChar()
{
	for (int i = 0; i < bMap->GetMapY(); i++)
	{
		delete[] movableMap[i];
	}
	delete[] movableMap;
	delete moveOrder;
	tTPGage->Release();
	tTPG->Release();
}


/////////////////////////////////////////////////////////////////////////////////////////
// void MoveCharacter(int objX, int objY, int n)
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 최단경로를 계산하는 함수. 재귀호출을 이용한다.
//		  목표지점으로부터 FindDistance함수에서 남긴 발자취를 쫒아
//		  출발지점으로 돌아오면 그 경로의 역이 최단경로이다.
//		  여기서는 각 이동마다 방향을 moveOrder에 반대로 저장해간다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

void CBattleChar::MoveCharacter(int objX, int objY, int n)
{
	if (x == objX && y == objY) //출발 지점으로 돌아오면
	{
		return;
	}

	n++;

	if (objY - 1 >= 0 && movableMap[objY-1][objX] == n)
	{
		MoveCharacter(objX, objY - 1, n);
		moveOrder[ptr] = DOWN;
		ptr++;
	}
	else if (objY + 1 < bMap->GetMapY() && movableMap[objY+1][objX] == n)
	{
		MoveCharacter(objX, objY + 1, n);
		moveOrder[ptr] = UP;
		ptr++;
	}
	else if (objX - 1 >= 0 && movableMap[objY][objX-1] == n)
	{
		MoveCharacter(objX - 1, objY, n);
		moveOrder[ptr] = RIGHT;
		ptr++;
	}
	else if (objX + 1 < bMap->GetMapX() && movableMap[objY][objX+1] == n)
	{
		MoveCharacter(objX + 1, objY, n);
		moveOrder[ptr] = LEFT;
		ptr++;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// bool Step()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설  명 : 캐릭터 이동시 속도를 조절해가며 이동하는 함수이다.
// 리턴값 : 한 셀을 다 이동할 경우 true를, 아닌경우 false를 리턴한다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

bool CBattleChar::Step()
{
	step++;
	if (step == 4 * 10) //10은 애니메이션 스피드로 동일한 프레임이 반복되는 횟수를 의미한다(즉, 낮을수록 빠르게 애니메이션)
	{
		switch(direction)
		{
		case DOWN:
			{
				y++;
				break;
			}
		case LEFT:
			{
				x--;
				break;
			}
		case RIGHT:
			{
				x++;
				break;
			}
		case UP:
			{
				y--;
				break;
			}
		}
		step = 0;
		return true;
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////
// int GetCharX(), int GetCharY()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 캐릭터의 좌표를 반환하는 함수
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

int CBattleChar::GetCharX()
{
	return x;
}

int CBattleChar::GetCharY()
{
	return y;
}