#define WIN32_LEAN_AND_MEAN
//main.cpp

#include <Windows.h>

#include "GameModule.h"




int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow) {
    MSG msg;
    
    // Initialize our game module.
    GameModule game(instance);

    // Show the game window.
    game.Show(true);

    // Main loop goes here.
    if (!game.MainLoop(&msg)) {
        //MessageBox(NULL, L"Game loop ended.\nProgram will now close.", L"QUIT", MB_OK | MB_ICONERROR);

        game.Show(false);
        game.Shutdown();

        return (int)msg.wParam;
    }

    return 0;
}