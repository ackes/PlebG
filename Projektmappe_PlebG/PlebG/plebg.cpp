//test test
#include <iostream>
#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		// Initialize the window. 
		return 0;

	case WM_PAINT:
		// Paint the window's client area. 
		return 0;

	case WM_SIZE:
		// Set the size and position of the window. 
		return 0;

	case WM_DESTROY:
		// Clean up window-specific data objects. 
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

int CALLBACK WinMain(
	HINSTANCE Instance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow) {

	WNDCLASS WNDClass = {};

	//WNDClass.cbSize = sizeof(WNDCLASSEX);
	WNDClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	WNDClass.lpfnWndProc = WndProc;
	WNDClass.hInstance = Instance;
	//WNDClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WNDClass.lpszClassName = "PlebG";

    if (RegisterClass(&WNDClass)) {
    	HWND GameWindow = CreateWindowEx(
			WS_EX_CLIENTEDGE,
  			//WNDClass.lpszClassName,
  			"PlebG",
  			"PlebG-WindowName",
  			WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
  			0,
  			0,
			Instance,
			0);

    	//if(GameWindow) {
    	//	ShowWindow(GameWindow, SW_SHOW);
    	//	UpdateWindow(GameWindow);
    	//}

    	MSG msg;
    	while (GetMessage(&msg, NULL, 0, 0)) {
    		TranslateMessage(&msg);
    		DispatchMessage(&msg);
    	} return (int) msg.wParam;

    }
    else {
    	MessageBox(
			NULL,
			("Call to RegisterClass failed!"),
			("PlebG"),
			NULL);
		return 1;
    }
}