//window.cpp__________________________________
#include "window.h"

char keyin;
bool button_pressed;

LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
	InputStruct *input = (InputStruct*)GetWindowLongPtr(wnd,GWLP_USERDATA);
  
    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);

            return 0;
        }
		break;
		case WM_CHAR: {
			keyin = (char) wParam;
			button_pressed = true;
			
		}
					  break;

    }
	if(!button_pressed)
		keyin ='0';

    return DefWindowProc(wnd, message, wParam, lParam);
}

windowclass::windowclass(HINSTANCE &instance)
{
	RECT temp = {0, 0, 1280, 720};
	windowSize = temp;
	ZeroMemory(&this->wndClass, sizeof(WNDCLASSEX));
    
    // Create our window description.
    this->wndClass.cbSize = sizeof(WNDCLASSEX);
    this->wndClass.style = CS_HREDRAW | CS_VREDRAW;
    this->wndClass.lpfnWndProc = WndProc;
    this->wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    this->wndClass.hInstance = instance;
    this->wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    this->wndClass.lpszClassName = L"Win32-And-DirectX-11-Tutorial";

    // Register our window description.
    RegisterClassEx(&this->wndClass);

    // Make sure the window working space is the size we specificed.
    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    // Position the window in the center of the screen.
    int posX = (GetSystemMetrics(SM_CXSCREEN) - (windowSize.right - windowSize.left)) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - (windowSize.bottom - windowSize.top)) / 2;

    // Create the window.
    this->window = CreateWindowEx(NULL, L"Win32-And-DirectX-11-Tutorial", L"Win32 And DirectX 11 Tutorial Series",
                                    WS_OVERLAPPEDWINDOW, posX, posY, (windowSize.right - windowSize.left),
                                    (windowSize.bottom - windowSize.top), NULL, NULL, instance, NULL);

	CoInitializeEx(0,0);
}