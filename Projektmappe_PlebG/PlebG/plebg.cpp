//TODO:
//Programm funktioniert nicht -- Vielleicht Denkfehler: Bitmap ist das was man auf dem Bildschirm sieht und der Buffer ist an einer anderen Stelle (obwohl Bitmapmemory Bitmapmemory heißt)
//Prüfen, ob Src/Dest bei StretchDIBits korrekt eingetragen sind und ob auch die Src mit "Farbe" gefüllt wird über unsere DrawingFunction
//DrawingFunction muss noch entsprechend korrigiert werden (Speicher Überlauf) und an korrekte Stellen eingetragen werden)

#include <iostream>
#include <Windows.h>
#include <stdio.h>

static bool Running;
static int BMwidth;
static int BMheight;
static void *BitmapMemory;
static BITMAPINFO BitmapInfo;

void DrawingFunction()
{
	unsigned int JumpNextRow = BMwidth * 4;
	unsigned char *Row = (unsigned char*)BitmapMemory; //NOTE(Patrick): Cast von Void auf Char (8 Bit Integer => 0-255 wegen unsigned)

	for (int x = 0; x < BMheight; x++) {
		unsigned char *Pixel = (unsigned char*)Row;
		for (int y = 0; y < BMwidth; y++) {
			*Pixel = 0;
			++Pixel;									//Durch ++Pixel springt man wegen Typ Char 8 Bit weiter
			*Pixel = 0;
			++Pixel;
			*Pixel = 255;
			++Pixel;
			*Pixel = 0;
			++Pixel;
		}
		Row = Row + JumpNextRow;
	}
}
//&Bitmapmemory Addresse Bitmapmemory 0x000a4252
//*Bitmapmemory			0x00000000
//Bitmapmemory (void)	0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

// *Row = *Bitmapmemory = 0x00000000
// *Pixel = *Row = *Bitmapmemory = 0x00000000
// a = Bitmapmemory

void CreateBitmap(int width, int height) {
	//TODO(Patrick): VirtualFree den angelegten Speicher
	if (BitmapMemory)
	{
		VirtualFree(BitmapMemory, 0, MEM_RELEASE);
	}

	//NOTE(Patrick): Bitmapinfo für die Farbgebung etc
	BMwidth = width;
	BMheight = height;

	BitmapInfo.bmiHeader.biWidth = BMwidth;
	BitmapInfo.bmiHeader.biHeight = -BMheight;
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	

	//NOTE(Patrick): BitmapMemory muss über die VirtualAlloc Funktion zugewiesen werden. Die Größe dieses Speichers entspricht der Höhe*Breite*Anzahl der Bytes der Bitmap
	int BitmapMemorySize = BMwidth*BMheight * 4;
	BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void CopyBitmapToDC(HDC DeviceContext, RECT *GameRECT, int width, int height, int x, int y) {		//Pointer des RECTS um nicht den kompletten Stack mit einem Rectangle vollzumachen
	int WWidth = GameRECT->right - GameRECT->left;
	int WHeight = GameRECT->bottom - GameRECT->top;
																										//Note(Patrick): StretchDIBits um DC Parameter auf Repaintregion (Bitmap) zu kopieren
	StretchDIBits(
		DeviceContext,
		0, 0, WWidth, WHeight,
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
		case WM_CLOSE:
		{
			OutputDebugStringA("CLOSE\n");
			Running = FALSE;
		}break;
		case WM_CREATE:
		{
			OutputDebugStringA("CREATE\n");
			RECT GameRECT;											//Hier werden die Größen vom WindowsWindow übernommen und in die Bitmap geschrieben (Beim Start des Programms
			GetClientRect(hwnd, &GameRECT);
			int width = GameRECT.right - GameRECT.left;
			int height = GameRECT.bottom - GameRECT.top;
			CreateBitmap(width, height);
		}break;
		case WM_PAINT:
		{
			OutputDebugStringA("PAINT\n");
			DrawingFunction();
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			
			int x = Paint.rcPaint.left;
			int y = Paint.rcPaint.top;
			int Paintwidth = Paint.rcPaint.right - Paint.rcPaint.left;
			int Paintheight = Paint.rcPaint.bottom - Paint.rcPaint.top;

			RECT GameRECT;
			GetClientRect(hwnd, &GameRECT);
			CopyBitmapToDC(DeviceContext, &GameRECT, Paintwidth, Paintheight, x, y);
			EndPaint(hwnd, &Paint);
			
		}break;
		case WM_SIZE:
		{ 
			OutputDebugStringA("SIZE\n");
		}break;
		case WM_DESTROY:
		{
			OutputDebugStringA("DESTROY\n");
			if (BitmapMemory)
			{
				VirtualFree(BitmapMemory, 0, MEM_RELEASE);
			}

			Running = FALSE;			
		}break;
		default: 
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}break;
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
    	HWND WindowsWindow = CreateWindowEx(
			0,
  			//WNDClass.lpszClassName,
  			"PlebG",
  			"PlebG-WindowName",
			WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
  			0,
  			0,
			Instance,
			0);
		Running = TRUE;
			
		while(Running)				//NOTE(Patrick): Solange das GameWindow offen ist, müssen die Funktionen, nach jeder Nachricht neu ausgeführt werden
		{
			MSG msg;
			//NOTE(Patrick): Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before returning.
			while (PeekMessage(&msg, WindowsWindow, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) {
					Running = FALSE;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}


			HDC DeviceContext = GetDC(WindowsWindow);
			RECT GameRECT;
			GetClientRect(WindowsWindow, &GameRECT);
			int RECTwidth = GameRECT.right - GameRECT.left;
			int RECTheight = GameRECT.bottom - GameRECT.top;
			
			CopyBitmapToDC(DeviceContext, &GameRECT, RECTwidth, RECTheight, 0, 0);
			ReleaseDC(WindowsWindow, DeviceContext);
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
	return 0;
}