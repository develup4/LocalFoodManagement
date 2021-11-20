#include "CEnemy.h"
#define MOVABLECOUNT 300 //현재는 아니지만 나중에 플레이어의 능력치 중 하나가 되므로 멤버변수가 된다. HP 등등 여러 능력치를 만들때 집어넣자.



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEnemy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell, CPlayer* player, bool chase)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CEnemy 클래스의 생성자
//		  적 캐릭터만의 데이터를 등록시킨다.
//
// 2010.01.20 최종수정
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXSPRITE sprite, CBattleMap* bMap, int x, int y, int movableCell, CPlayer* player, bool chase) : CBattleChar(pd3dDevice, sprite, bMap, x, y, movableCell)
{
	this->player = player;
	this->chase = chase;
	chase ? D3DXCreateTextureFromFile(pd3dDevice, "Texture\\BattleChar\\StrangeGuy.tga", &tChar) : D3DXCreateTextureFromFile(pd3dDevice, "Texture\\BattleChar\\Riel.tga", &tChar);
	distance = new int*[bMap->GetMapY()];
	for (int i = 0; i < bMap->GetMapY() ; i++)
	{
		distance[i] = new int[bMap->GetMapX()];
	}
	for(int i = 0; i < bMap->GetMapX(); i++)
	{
		for(int j = 0; j < bMap->GetMapY(); j++)
		{
			distance[j][i] = 0;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// ~CEnemy()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : CEnemy 클래스의 소멸자
//		  적 캐릭터만의 데이터를 해제시킨다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

CEnemy::~CEnemy()
{
	for (int i = 0; i < bMap->GetMapY(); i++)
	{
		delete[] distance[i];
	}
	delete[] distance;
}


/////////////////////////////////////////////////////////////////////////////////////////
// void TakeAction()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 객체의 행동함수이다.
//		  렌더링을 제외한 객체의 모든 처리를 이 함수에서 한다.
//		  본 객체에서는 적 캐릭터의 이동 전반에 관한 부분을 처리한다.
//		  적 캐릭터의 인공지능을 이 함수에서 맡는다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

void CEnemy::TakeAction()
{
	switch(state)
	{
	case FILLTP:
		{
			tp++;
			if (tp == MOVABLECOUNT)
			{
				FindDistance(x, y, movableCell);
				state = MOVABLE;
			}
			break;
		}
	case MOVABLE:
		{
			int objX = 0;
			int objY = 0;
			if (chase)
			{
				//추격자라면 플레이어와 가장 가까운 지점을 목표로 도망자라면 가장 먼 지점을 목표로 한다.
				//따라서 이동가능한 셀들과 플레이어의 위치사이의 거리를 정렬한다.

				int min = 999999;

				for(int i = 0; i < bMap->GetMapX(); i++)
				{
					for(int j = 0; j < bMap->GetMapY(); j++)
					{
						if (distance[j][i] > 0 && distance[j][i] < min)
						{
							min = distance[j][i];
							objX = i;
							objY = j;
						}
					}
				}
			}
			else
			{
				int max = 0;

				for(int i = 0; i < bMap->GetMapX(); i++)
				{
					for(int j = 0; j < bMap->GetMapY(); j++)
					{
						if (distance[j][i] > max)
						{
							max = distance[j][i];
							objX = i;
							objY = j;
						}
					}
				}
			}

			//최단경로 구하기
			MoveCharacter(objX, objY, movableMap[objY][objX]);

			//초기화
			ptr = 0;
			for(int i = 0; i < bMap->GetMapX(); i++)
			{
				for(int j = 0; j < bMap->GetMapY(); j++)
				{
					movableMap[j][i] = 0;
					distance[j][i] = 0;
				}
			}
			state = MOVE;
			break;
		}
	case MOVE:
		{
			direction = moveOrder[ptr]; //moveOrder에는 최단경로의 각 이동방향이 저장된다
			if (Step()) //Step은 1셀이동이 끝날때 true를 반환한다
			{
				ptr++;
			}

			if (moveOrder[ptr] == -1) //목적지까지 움직이면
			{
				//초기화
				for (int i = 0; i < movableCell; i++)
				{
					moveOrder[i] = -1;
				}
				ptr = 0;
				tp = 0;
				state = FILLTP;
			}
			break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// void Draw()
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 객체의 렌더링 함수이다.
//		  객체의 텍스쳐 이미지를 렌더링한다.
//		  캐릭터의 이동 애니메이션을 렌더링한다.
//		  TP 게이지를 렌더링한다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

void CEnemy::Draw()
{
	//전체 맵에서의 좌표에서 스크롤 좌표를 빼면 현재 스크린에서의 좌표를 구할 수 있다.

	//캐릭터 렌더링
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	RECT rc = {(step / 10) * 32, direction * 48, (step / 10) * 32 + 32, direction * 48 + 48};
	D3DXVECTOR3 pos;
	pos.z = 0.0f;
	switch(direction)
	{
	case DOWN:
		{
			pos.x = (float)x * 32 - bMap->GetSX();
			pos.y = (float)y * 32 + ((step / 10) * 8) - 20 - bMap->GetSY(); //20 : 캐릭터가 세로로 더 길기 때문
			break;
		}
	case LEFT:
		{
			pos.x = (float)x * 32 - ((step / 10) * 8) - bMap->GetSX();
			pos.y = (float)y * 32 - 20 - bMap->GetSY();
			break;
		}
	case RIGHT:
		{
			pos.x = (float)x * 32 + ((step / 10) * 8) - bMap->GetSX();
			pos.y = (float)y * 32 - 20 - bMap->GetSY();
			break;
		}
	case UP:
		{
			pos.x = (float)x * 32 - bMap->GetSX();
			pos.y = (float)y * 32 - ((step / 10) * 8) - 20 - bMap->GetSY();
			break;
		}
	}
	sprite->Draw(tChar, &rc, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite->End();

	//TP게이지 렌더링
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	RECT rctp = {0, 0, 52, 5};
	pos.x -= 10;
	pos.y -= 10;
	sprite->Draw(tTPGage, &rctp, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, 255));
	RECT rct = {0, 0, (int)(50 * ((float)tp / 300.0f)), 3};
	pos.x++;
	pos.y++;
	sprite->Draw(tTPG, &rct, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite->End();
}


/////////////////////////////////////////////////////////////////////////////////////////
// void FindDistance(int x, int y, int n)
/////////////////////////////////////////////////////////////////////////////////////////
//
// 설명 : 이동가능범위를 계산하는 함수. 재귀호출을 이용한다.
//		  계산하면서 동시에 이차원배열에 자취를 남겨 나중에 최단경로 계산에 이용하게한다.
//		  거리를 기록하는 이차원배열에 그 셀과 플레이어의 거리의 제곱을 기록한다.
//
// 2010.01.20 최종수정
/////////////////////////////////////////////////////////////////////////////////////////

void CEnemy::FindDistance(int x, int y, int n)
{
	movableMap[y][x] = n; //자취를 남긴다
	distance[y][x] = (x - player->GetCharX()) * (x - player->GetCharX()) + (y - player->GetCharY()) * (y - player->GetCharY());

	if (n == 0)
	{
		return;
	}

	if (y - 1 >= 0 && bMap->GetMovable(x, y - 1) && movableMap[y-1][x] < n - 1) //위로 이동할 수 있다
	{
		FindDistance(x, y - 1, n - 1); //나머지를 판단
	}
	if (y + 1 < bMap->GetMapY() && bMap->GetMovable(x, y + 1) && movableMap[y+1][x] < n - 1) //아래로 이동할 수 있다
	{
		FindDistance(x, y + 1, n - 1); //나머지를 판단
	}
	if (x + 1 < bMap->GetMapX() && bMap->GetMovable(x + 1, y) && movableMap[y][x+1] < n - 1) //오른쪽으로 이동할 수 있다
	{
		FindDistance(x + 1, y, n - 1); //나머지를 판단
	}
	if (x - 1 >= 0 && bMap->GetMovable(x - 1, y) && movableMap[y][x-1] < n - 1) //왼쪽으로 이동할 수 있다
	{
		FindDistance(x - 1, y, n - 1); //나머지를 판단
	}
}