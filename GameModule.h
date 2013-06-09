#pragma once
//GameModule.h__________________________________

#include "GraphicsModule.h"
#include "window.h"


class GameModule {
public:

    GameModule(HINSTANCE &instance);
	gametimer thetimer;

    void Shutdown(void);
    void Show(bool show);
    bool MainLoop(MSG *msg);
	float time1, time2;
	InputStruct theinput;

private:
	HINSTANCE instance;
	windowclass thewindow;
	HWND window;
	GraphicsModule graphics;  
};

