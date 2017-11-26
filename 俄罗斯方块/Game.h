#pragma once

#include "Map.h"
#include "Brick.h"
#include "Scene.h"

#include <time.h>
#include <string.h>
#include <conio.h>
#include <string>

#include "resource.h"
extern HINSTANCE g_hIns;

class Game;
extern Game game;

class Game
{
	public:
	Game()
	{
		goal = 0;
		m_redrawNext = true;
	}
	~Game()
	{

	}

	void start()
	{
		InitGame();
	}

	void restart()
	{
		delete m_brick;
		delete m_nextBrick;
		InitGame();
	}

	void InitGame()
	{
		srand((unsigned)time(NULL));
		m_brick = new Brick(m_map);
		m_nextBrick = new Brick(m_map);
		m_redrawNext = true;
		goal = 0;
		m_redrawNext = true;
		m_map.reset();
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

	//返回删除的行数
	int FixBrick()
	{
		int i = -1;
		for (i = 0; i < 4; i++)
		{
			m_map.changeRow(m_brick->getAxisRow() + i, m_map.getRow(m_brick->getAxisRow() + i)
				| ((*m_brick)[i] << (24 - m_brick->getAxisCol())));
		}
		return m_map.TryDeleteLines();
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
			calculateGoal(FixBrick());
			if (m_map.getRow(0) != g_iInitBackGround[0])
			{
				return -1;
			}
			delete m_brick;
			m_brick = m_nextBrick;
			m_nextBrick = new Brick(m_map);
			m_redrawNext = true;
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

	void calculateGoal(int lines)
	{
		switch (lines)
		{
		case 1:
			goal += 1;
			break;
		case 2:
			goal += 3;
			break;
		case 3:
			goal += 6;
			break;
		case 4:
			goal += 10;
			break;
		default:
			break;
		}
	}

	static HRESULT Initialize(HINSTANCE hInstance)
	{
		WNDCLASS wcex;

		HRESULT hr = S_OK;

		ATOM atom;

		// Register window class.
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Game::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = (HBRUSH)COLOR_HIGHLIGHTTEXT;
		wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
		wcex.hIcon = LoadIcon(
			NULL,
			IDI_APPLICATION);
		wcex.hCursor = LoadCursor(
			NULL,
			IDC_ARROW);
		wcex.lpszClassName = TEXT("Tetris-SDK");

		atom = RegisterClass(
			&wcex
		);

		hr = atom ? S_OK : E_FAIL;

		// Create window.
		HWND hwnd = CreateWindow(
			TEXT("Tetris-SDK"),
			TEXT("Tetris-SDK"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		if (SUCCEEDED(hr))
		{
			hr = hwnd ? S_OK : E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			ShowWindow(
				hwnd,
				SW_SHOW
			);


			UpdateWindow(
				hwnd
			);
		}

		return hr;
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			game.start();
			SetTimer(hwnd, 1, 1000, NULL);
			game.UpdateScene();
			//PostMessage(hwnd, WM_USER, NULL, 1);

			//InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		}
		case WM_COMMAND:
		{
			WORD wHiWord = HIWORD(wParam); //0表示是菜单
			WORD wLwWord = LOWORD(wParam); //菜单项的ID

			if (wHiWord == 0)
			{
				switch (wLwWord)
				{
				case ID_EASYLEVEL:
					KillTimer(hwnd, 1);
					SetTimer(hwnd, 1, 1000, NULL);
					InvalidateRect(hwnd, NULL, FALSE); 
					game.restart();
					break;
				case ID_NORMALLEVEL:
					KillTimer(hwnd, 1);
					SetTimer(hwnd, 1, 500, NULL);
					InvalidateRect(hwnd, NULL, FALSE);
					game.restart();
					break;
				case ID_HARDLEVEL:
					KillTimer(hwnd, 1);
					SetTimer(hwnd, 1, 300, NULL);
					InvalidateRect(hwnd, NULL, FALSE);
					game.restart();
					break;
				}
			}
			return 0;
		}
		
		case WM_USER:
		{
			HDC hDC = GetDC(hwnd);

			static Map oldMap = game.m_drawMap;
			static bool first = true;

			if (first)
			{
				for (int i = 0; i < GAME_ROW; i++)
				{
					for (int j = 0; j < GAME_COL; j++)
					{
						if ((game.m_drawMap.getRow(i) << j) & 0x80000000)
						{
							DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON1)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
						}
						else
						{
							DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON2)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
						}
					}
				}
				first = false;
			}
			else
			{
				for (int i = 0; i < GAME_ROW; i++)
				{
					for (int j = 0; j < GAME_COL; j++)
					{
						if (((game.m_drawMap.getRow(i) << j) & 0x80000000) != ((oldMap.getRow(i) << j) & 0x80000000))
						{
							if ((game.m_drawMap.getRow(i) << j) & 0x80000000)
							{
								DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON1)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
							}
							else
							{
								DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON2)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
							}
						}
					}
				}
			}
			oldMap = game.m_drawMap;
			if(game.m_redrawNext)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (((*game.m_nextBrick)[i] >> (j + 4)) & 0x1)
						{
							DrawIconEx(hDC, (7 - j + GAME_COL) * 16, (i + 4) * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON1)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
						}
						else
						{
							DrawIconEx(hDC, (7 - j + GAME_COL) * 16, (i + 4) * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON2)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
						}
					}
				}
				game.m_redrawNext = false;
			}
			std::wstring strPrint(L"分数：");
			wchar_t wszGoal[5];
			_itow(game.goal, wszGoal, 10);
			strPrint += wszGoal;
			DrawText(hDC, strPrint.c_str(), -1, &RECT({ (GAME_COL) * 16, 0, (GAME_COL + 2) * 32, 2 * 32 }), DT_CENTER);

			ReleaseDC(hwnd, hDC);//与BeginPaint配对使用

			return 0;
		}
		case WM_PAINT: //当程序需要绘图时,OS和其他程序会发送此消息
		{
			//PAINTSTRUCT ps;
			//HDC hDC = BeginPaint(hwnd, &ps);
			HDC hDC = GetDC(hwnd);

			for (int i = 0; i < GAME_ROW; i++)
			{
				for (int j = 0; j < GAME_COL; j++)
				{
					if ((game.m_drawMap.getRow(i) << j) & 0x80000000)
					{
						DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON1)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
					}
					else
					{
						DrawIconEx(hDC, j * 16, i * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON2)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
					}
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (((*game.m_nextBrick)[i] >> (j + 4)) & 0x1)
					{
						DrawIconEx(hDC, (7 - j + GAME_COL) * 16, (i + 4) * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON1)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
					}
					else
					{
						DrawIconEx(hDC, (7 - j + GAME_COL) * 16, (i + 4) * 16, LoadIcon(g_hIns, MAKEINTRESOURCE(IDI_ICON2)), 16, 16, 0, NULL, DI_COMPAT | DI_NORMAL  );
					}
				}
			}
			std::wstring strPrint(L"分数：");
			wchar_t wszGoal[5];
			_itow(game.goal, wszGoal, 10);
			strPrint += wszGoal;
			DrawText(hDC, strPrint.c_str(), -1, &RECT({ (GAME_COL) * 16, 0, (GAME_COL + 2) * 32, 2 * 32 }), DT_CENTER);

			ReleaseDC(hwnd, hDC);//与BeginPaint配对使用
			ValidateRect(hwnd, NULL);
			//EndPaint(hwnd, &ps);

			return 0;
		}
		case WM_TIMER:
		{
			if (game.OnDown() == -1)
			{
				return -1;
				// TODO: Game Over
			}
			game.UpdateScene();
			PostMessage(hwnd, WM_USER, NULL, 1);
			//InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case 'W':
				game.OnUp();
				break;
			case 'S':
				game.OnDown();
				break;
			case 'A':
				game.OnLeft();
				break;
			case 'D':
				game.OnRight();
				break;
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			game.UpdateScene();
			PostMessage(hwnd, WM_USER, NULL, 1);
			//InvalidateRect(hwnd, NULL, FALSE);
		}
		}


		return DefWindowProc(
			hwnd,
			message,
			wParam,
			lParam
		);

	}
	private:
	Map m_map;
	Map m_drawMap;
	Brick *m_brick;
	Brick *m_nextBrick;
	Scene m_scene;
	int goal;
	bool m_redrawNext;
};

