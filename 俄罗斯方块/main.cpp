#include <stdlib.h>

#include "Game.h"

HINSTANCE g_hIns;


int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR     lpCmdLine,
	int       nCmdShow)
{

	g_hIns = hInstance;
	HRESULT hr;

	hr = Game::Initialize(hInstance);

	if (SUCCEEDED(hr))
	{
		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			DispatchMessage(&msg);
		}
	}

	return SUCCEEDED(hr) ? EXIT_SUCCESS : EXIT_FAILURE;


}


