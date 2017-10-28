#pragma once

#include "Map.h"
#include "Brick.h"
#include "Scene.h"
#include <time.h>
#include <string.h>
#include <conio.h>

class Game
{
public:
	Game()
	{
		
	}
	~Game()
	{

	}

	void start()
	{
		InitGame();
		m_scene.Draw(m_drawMap);
		clock_t clkStart1 = clock();
		clock_t clkEnd1 = clock();
		clock_t clkStart2 = clock();
		clock_t clkEnd2 = clock();
		while (1)
		{
			clkEnd1 = clock();
			if (clkEnd1 - clkStart1 > 300)
			{
				clkStart1 = clkEnd1;
				if (OnDown() == -1)
				{
					system("cls");
					printf("Game Over!\r\n");
					break;
				}
				UpdateScene();

			}
			CheckInput();
			clkEnd2 = clock();
			if (clkEnd2 - clkStart2 > 30)
			{
				clkStart2 = clkEnd2;
				m_scene.Draw(m_drawMap);

			}
		}
	}

	void InitGame()
	{
		srand((unsigned)time(NULL));
		m_brick = new Brick(m_map);
		UpdateScene();
	}

	void UpdateScene()
	{
		int i = -1;
		m_drawMap = m_map;
		for (i = 0; i < 4; i++)
		{
			if (m_brick->getAxisRow() + i < GAME_ROW)
			{
				m_drawMap.changeRow(m_brick->getAxisRow() + i, m_map.getRow(m_brick->getAxisRow() + i)
					| ((*m_brick)[i] << (24 - m_brick->getAxisCol())));
			}
		}
	}

	int FixBrick()
	{
		int i = -1;
		for (i = 0; i < 4; i++)
		{
			m_map.changeRow(m_brick->getAxisRow() + i, m_map.getRow(m_brick->getAxisRow() + i)
				| ((*m_brick)[i] << (24 - m_brick->getAxisCol())));
		}
		m_map.TryDeleteLines();

		return 0;
	}

	int OnUp()
	{
		m_brick->TryRotate();
		return 0;
	}

	int OnDown()
	{
		if (m_brick->TryMove(0, 1))
		{
			FixBrick();
			if (m_map.getRow(0) != g_iInitBackGround[0])
			{
				return -1;
			}
			delete m_brick;
			m_brick = new Brick(m_map);
		}
		return 0;
	}

	int OnLeft()
	{
		m_brick->TryMove(-1, 0);
		return 0;
	}

	int OnRight()
	{
		m_brick->TryMove(1, 0);
		return 0;
	}

	int CheckInput()
	{
		char chInput = '\0';
		int iFlag = 0;
		while (_kbhit())
		{
			iFlag = 1;
			chInput = (char)_getch();
			switch (chInput)
			{
			case 'w':
				OnUp();
				break;
			case 's':
				OnDown();
				break;
			case 'a':
				OnLeft();
				break;
			case 'd':
				OnRight();
				break;
			}
			UpdateScene();
		}

		return iFlag;
	}
private:
	Map m_map;
	Map m_drawMap;
	Brick *m_brick;
	Scene m_scene;
};

