#pragma once
//window.h__________________________________

#define WIN32_LEAN_AND_MEAN

//#include <d3d11.h>
#include <Windows.h>
#include <d3d.h>

class GameModule;

class windowclass{
public:
	GameModule *gamemodule;
	WNDCLASSEX wndClass;
	HWND window;
	windowclass::windowclass(HINSTANCE &instance);
};

LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);

static RECT windowSize = {0, 0, 1280, 720};