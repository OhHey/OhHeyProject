//GameModule.cpp__________________________________
#include "GameModule.h"

GameModule::GameModule(HINSTANCE &instance) :thewindow(instance),window(thewindow.window),graphics(window)
{
	SetWindowLongPtr(window,GWLP_USERDATA,(LONG)&theinput);
	time1 = thetimer.gettime();
}

void GameModule::Shutdown(void) 
{
	//CleanD3D();
    graphics.Shutdown();
}

void GameModule::Show(bool show) {
    if (show == true) {
        ShowWindow(this->window, SW_SHOW);
    } else {
        ShowWindow(this->window, SW_HIDE);
    }
}

bool GameModule::MainLoop(MSG *msg) {
    while (true) {
        while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(msg);
            DispatchMessage(msg);
        }

		time2 = thetimer.gettime();
		float timeelapsed = time2 - time1;
		time1 = time2;
		graphics.UpdateFrame(timeelapsed,time2);
		graphics.RenderFrame(timeelapsed);

        if (msg->message == WM_QUIT) {
            break;
        }
    }

    return 0;
}