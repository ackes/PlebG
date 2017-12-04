#include <iostream>
#include <Windows.h>
#include <stdio.h>

//NOTE(Patrick): Jedesmal wenn kopiert wird, wird eine neue Bitmap erstellt und an unser DC angepasst -- unperformant, aber leichter zu verstehen
void CopyBITMAPToDC(int DCwidth, int DCheight, int X, int Y, RECT GameRECT, HDC DeviceContext) {
	
	//TODO(Patrick): VirtualFree den angelegten Speicher - Funktion in zwei Funktionen splitten
	
	//NOTE(Patrick): Bitmapinfo für die Farbgebung etc
	int BMwidth = DCwidth;
	int BMheight = DCheight;

	BITMAPINFO BitmapInfo;
	BitmapInfo.bmiHeader.biWidth;
	BitmapInfo.bmiHeader.biHeight;
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biSizeImage;
	BitmapInfo.bmiHeader.biClrUsed = 0;
	BitmapInfo.bmiHeader.biClrImportant = 0;
	BitmapInfo.bmiHeader.biWidth = BMwidth;
	BitmapInfo.bmiHeader.biHeight = -BMheight;

	//NOTE(Patrick): BitmapMemory muss über die VirtualAlloc Funktion zugewiesen werden. Die Größe dieses Speichers entspricht der Höhe*Breite*Anzahl der Bytes der Bitmap
	int BitmapMemorySize = BMwidth*BMheight * 4;
	void *BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	//Note(Patrick): StretchDIBits um Bitmap auf DeviceContext zu kopieren
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
	LRESULT Result = 0;

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
			0,
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

			
		

		for (;;)				//NOTE(Patrick): Solange das GameWindow offen ist, müssen die Funktionen, nach jeder Nachricht neu ausgeführt werden
		{
			MSG msg;
			//NOTE(Patrick): Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before returning.
			while (PeekMessage(&msg, GameWindow, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			HDC DeviceContext = GetDC(GameWindow);
			RECT GameRECT;
			GetClientRect(GameWindow, &GameRECT);
			int DCwidth = GameRECT.right - GameRECT.left;
			int DCheight = GameRECT.bottom - GameRECT.top;
			
			CopyBITMAPToDC(DCwidth, DCheight, 0, 0, GameRECT, DeviceContext);
			ReleaseDC(GameWindow, DeviceContext);
		}
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