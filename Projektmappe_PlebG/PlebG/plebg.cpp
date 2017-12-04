#include <iostream>
#include <Windows.h>
#include <stdio.h>

//Jedesmal wenn kopiert wird, wird eine neue Bitmap erstellt und an unser DC angepasst -- unperformant, aber leichter zu verstehen
void CopyBITMAPToDC(int DCwidth, int DCheight, int X, int Y, RECT GameRECT, HDC DeviceContext) {
	//Der BITMAPInfoHeader für die Rahmenbedingungen der Bitmap
	tagBITMAPINFOHEADER BitmapInfoHeader;
	//Bitmapinfo für die Farbgebung etc.
	tagBITMAPINFO BitmapInfo;
	BitmapInfo.bmiHeader = BitmapInfoHeader;
	
	BitmapInfoHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfoHeader.biPlanes = 1;
	BitmapInfoHeader.biBitCount = 32;
	BitmapInfoHeader.biCompression = BI_RGB;
	BitmapInfoHeader.biSizeImage;
	BitmapInfoHeader.biClrUsed = 0; // 0 bedeutet, dass die Bitmap die maximale Anzahl an möglichen Farben nutzt
	BitmapInfoHeader.biClrImportant = 0;
	int BMwidth = BitmapInfoHeader.biWidth;
	int BMheight = -BitmapInfoHeader.biHeight;

	//BitmapMemory muss über die VirtualAlloc Funktion zugewiesen werden. Die Größe dieses Speichers entspricht der Höhe*Breite*Anzahl der Bytes der Bitmap
	int BitmapMemorySize = BMwidth*BMheight * 4;
	void *BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	//StretchDIBits um Bitmap auf DeviceContext zu kopieren
	StretchDIBits(
						DeviceContext,
						0, 0, DCwidth, DCheight,
						0, 0, BMwidth, BMheight,
						BitmapMemory,
						&BitmapInfo,
						DIB_RGB_COLORS,
						SRCCOPY);	
}

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
			} return (int)msg.wParam;

			HDC DeviceContext = GetDC(GameWindow);
			RECT GameRECT;
			int DCwidth = GameRECT.right - GameRECT.left;
			int DCheight = GameRECT.bottom - GameRECT.top;
			GetClientRect(GameWindow, &GameRECT);
			CopyBITMAPToDC(DCwidth, DCheight, 0, 0, GameRECT, DeviceContext);
			ReleaseDC(GameWindow, DeviceContext);
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