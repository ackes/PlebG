//TODO:
//Programm funktioniert nicht -- Vielleicht Denkfehler: Bitmap ist das was man auf dem Bildschirm sieht und der Buffer ist an einer anderen Stelle (obwohl Bitmapmemory Bitmapmemory heißt)
//Prüfen, ob Src/Dest bei StretchDIBits korrekt eingetragen sind und ob auch die Src mit "Farbe" gefüllt wird über unsere DrawingFunction
//DrawingFunction muss noch entsprechend korrigiert werden (Speicher Überlauf) und an korrekte Stellen eingetragen werden)


// x entspricht der Anzahl der Zeilen
// y entspricht der Anzahl der Spalten

#include <Windows.h>
#include <stdint.h>

static bool Running;
static int BMwidth;
static int BMheight;
static void *BackgroundMemory;
static void *ForegroundMemory;
static void *MergedMemory;
static BITMAPINFO BitmapInfo;

struct pleb {
	int rowPos = BMheight / 2;
	int colPos = BMwidth / 2;
	int plebWidth = BMwidth / 20 ;
	int plebHeight = BMheight / 15;
};

void CreateBackgroundBuffer(int width, int height) {
	//TODO(Patrick): VirtualFree den angelegten Speicher
	if (BackgroundMemory)
	{
		VirtualFree(BackgroundMemory, 0, MEM_RELEASE);
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
	BackgroundMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void DrawingBackground()
{
	unsigned int JumpNextRow = BMwidth * 4;
	unsigned char *Row = (unsigned char*)BackgroundMemory; //NOTE(Patrick): Cast von Void auf Char (8 Bit Integer => 0-255 wegen unsigned)

	for (int x = 0; x < BMheight; x++) {
		unsigned char *Pixel = (unsigned char*)Row;
		//Wir malen BB GG RR xx
		for (int y = 0; y < BMwidth; y++) {
			*Pixel = 100;
			++Pixel;									//Durch ++Pixel springt man wegen Typ Char 8 Bit weiter
			*Pixel = 100;
			++Pixel;
			*Pixel = 0;
			++Pixel;
			*Pixel = 0;
			++Pixel;
		}
		Row = Row + JumpNextRow;
	}
}

void CreateForegroundBuffer(int width, int height) {
	//TODO(Patrick): VirtualFree den angelegten Speicher
	if (ForegroundMemory)
	{
		VirtualFree(ForegroundMemory, 0, MEM_RELEASE);
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
	ForegroundMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void DrawingForeground()
{
	pleb pleb;
	
	unsigned int JumpNextRow = BMwidth * 4;
	unsigned char *Row = (unsigned char*)ForegroundMemory; //NOTE(Patrick): Cast von Void auf Char (8 Bit Integer => 0-255 wegen unsigned)
	for (int x = 0; x < BMheight; x++) {
		unsigned char *Pixel = (unsigned char*)Row;
		for (int y = 0; y < BMwidth; y++) {
			if (x >= (pleb.rowPos - pleb.plebHeight)) {
				if (y >= pleb.colPos - pleb.plebWidth) {
					if (x < pleb.rowPos + pleb.plebHeight) {
						if (y < pleb.colPos + pleb.plebWidth) {
							*Pixel = 0;
							++Pixel;
							*Pixel = 255;
							++Pixel;
							*Pixel = 0;
							++Pixel;
							*Pixel = 0;
							++Pixel;
						}
					}	
				}
			}
			else {
				*Pixel = 0;
				++Pixel;
				*Pixel = 0;
				++Pixel;
				*Pixel = 0;
				++Pixel;
				*Pixel = 0;
				++Pixel;
			}
		}
		Row = Row + JumpNextRow;
	}
}

/*void CreateMergeBuffer(int width, int height) {
	if (MergedMemory)
	{
		VirtualFree(MergedMemory, 0, MEM_RELEASE);
	}

	int MergedWidth = width;
	int MergedHeight = height;
	int BitmapMemorySize = MergedWidth*MergedHeight * 4;
	MergedMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}*/
void CreateMergedBuffer(int width, int height) {
	//TODO(Patrick): VirtualFree den angelegten Speicher
	if (MergedMemory)
	{
		VirtualFree(MergedMemory, 0, MEM_RELEASE);
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
	MergedMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void MergeBuffer()
//Reminder für Dummies: ++FPixel entspricht FPixel = FPixel + 1 und weist einen neuen Wert zu, anders als FPixel + 1 :]
{
	unsigned int JumpNextRow = BMwidth * 4;
	unsigned char *FRow = (unsigned char*)ForegroundMemory; //NOTE(Patrick): Cast von Void auf Char (8 Bit Integer => 0-255 wegen unsigned)
	unsigned char *BRow = (unsigned char*)BackgroundMemory;
	unsigned char *MRow = (unsigned char*)MergedMemory;

	for (int x = 0; x < BMheight; x++) {
		unsigned char *FPixel = (unsigned char*)FRow;
		unsigned char *BPixel = (unsigned char*)BRow;
		unsigned char *MPixel = (unsigned char*)MRow;
		for (int y = 0; y < BMwidth; y++) {		
			if ( (*FPixel != 0) || (*(FPixel + 1) != 0) || (*(FPixel + 2) != 0 ) ){				// || (*FPixel + 1 > 0) || (*FPixel + 2 > 0) ) {
				*MPixel = *FPixel;
				++MPixel;
				++FPixel;
				*MPixel = *FPixel;
				++MPixel;
				++FPixel;
				*MPixel = *FPixel;
				++MPixel;
				++FPixel;
				*MPixel = 0;
				++MPixel;
				++FPixel;
			}
			else {
				*MPixel = *BPixel;
				++MPixel;
				++BPixel;
				*MPixel = *BPixel;
				++MPixel;
				++BPixel;
				*MPixel = *BPixel;
				++MPixel;
				++BPixel;
				*MPixel = 0;
				++MPixel;
				++BPixel;
			}
		}
		FRow = FRow + JumpNextRow;
		BRow = BRow + JumpNextRow;
		MRow = MRow + JumpNextRow;
	}
}

void CopyBitmapToWindow(HDC DeviceContext, RECT *GameRECT, int width, int height, int x, int y) {		//Pointer des RECTS um nicht den kompletten Stack mit einem Rectangle vollzumachen
	int WWidth = GameRECT->right - GameRECT->left;
	int WHeight = GameRECT->bottom - GameRECT->top;
																										//Note(Patrick): StretchDIBits um DC Parameter auf Repaintregion (Bitmap) zu kopieren
	StretchDIBits(
		DeviceContext,
		0, 0, WWidth, WHeight,
		0, 0, BMwidth, BMheight,
		MergedMemory,
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
			CreateBackgroundBuffer(width, height);
			CreateForegroundBuffer(width, height);
			CreateMergedBuffer(width, height);
		}break;
		case WM_PAINT:
		{
			OutputDebugStringA("PAINT\n");
			DrawingBackground();
			DrawingForeground();
			MergeBuffer();
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			
			int x = Paint.rcPaint.left;
			int y = Paint.rcPaint.top;
			int Paintwidth = Paint.rcPaint.right - Paint.rcPaint.left;
			int Paintheight = Paint.rcPaint.bottom - Paint.rcPaint.top;

			RECT GameRECT;
			GetClientRect(hwnd, &GameRECT);
			CopyBitmapToWindow(DeviceContext, &GameRECT, Paintwidth, Paintheight, x, y);
			EndPaint(hwnd, &Paint);
			
		}break;
		case WM_SIZE:
		{ 
			OutputDebugStringA("SIZE\n");
		}break;
		case WM_DESTROY:
		{
			OutputDebugStringA("DESTROY\n");
			if (BackgroundMemory)
			{
				VirtualFree(BackgroundMemory, 0, MEM_RELEASE);
			}
			if (ForegroundMemory)
			{
				VirtualFree(ForegroundMemory, 0, MEM_RELEASE);
			}
			if (MergedMemory)
			{
				VirtualFree(MergedMemory, 0, MEM_RELEASE);
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
			
			CopyBitmapToWindow(DeviceContext, &GameRECT, RECTwidth, RECTheight, 0, 0);
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