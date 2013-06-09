#pragma once
//window.h__________________________________

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "structsandincludes.h"

class windowclass{
public:
	WNDCLASSEX wndClass;
	HWND window;
	windowclass::windowclass(HINSTANCE &instance);
	RECT windowSize;
};

LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);

