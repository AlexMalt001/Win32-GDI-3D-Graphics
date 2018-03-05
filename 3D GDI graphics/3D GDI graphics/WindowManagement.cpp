#include <Windows.h>
// ReSharper disable CppUnusedIncludeDirective
#include <stdlib.h>
#include "WindowManagement.h"
#include <math.h>
#include "Utils.h"
//ReSharper enable CppUnusedIncludeDirective


const LPCWCHAR g_szClassName = L"myWindowClass"; //name of window class - 'sz' = string, zero terminated

// ReSharper disable once CppInconsistentNaming
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void screen::drawDiagonal(int x1, int y1, int x2, int y2, DWORD colour) {
	int upperX = (x1>x2) ? x1:x2; //the leftmost coord
	int lowerX = (x1>x2) ? x2:x1; //the rightmost coord
	int upperY = (x1>x2) ? y1:y2; //the x coord that matches upperX
	int lowerY = (x1>x2) ? y2:y1; //the y coord that matches lowerX

	double dydx = 0; //to hold the gradient
	if(upperX-lowerX == 0) { //if the line is vertical
		if(upperY < lowerY) { //sort the y coords, regardless of their x partner
			int temp = lowerY;
			lowerY = upperY;
			upperY = temp;
		}

		for(int i = lowerY; i<upperY; i++) { //for each y between the 2 coords
			drawPx(lowerX, i, colour); //draw at the current coord
		}
	} else {
		dydx = (float(upperY-lowerY))/(float(upperX-lowerX)); //gradient = deltaY/deltaX
	}

	float c = upperY - (dydx*upperX); //find term not in x, y=mx+c -> c=y-mx

	if(upperY < lowerY) { //sort the y coords, regardless of their x partner
		int temp = lowerY;
		lowerY = upperY;
		upperY = temp;
	}
	for(int currentX = lowerX; currentX <= upperX; currentX++) { //for each x coord in the row
		int lowerYBound = int((dydx*(currentX - 0.5) + c >= lowerY) ? dydx*(currentX - 0.5) + c : lowerY); //lower bound covered by this x-pixel
		int upperYBound = int((dydx*(currentX + 0.5) + c <= upperY) ? dydx*(currentX + 0.5) + c : upperY); //upper bound covered by this x-pixel

		if(lowerYBound > upperYBound) { //sort bounds (messed up by -ve gradients)
			int temp = lowerYBound;
			lowerYBound = upperYBound;
			upperYBound = temp;
		}

		if(lowerYBound <lowerY) { //constrain lower bound above provided minimum y value
			lowerYBound = lowerY;
		}
		if(upperYBound > upperY) { //constrain upper bound below provided maximum y value
			upperYBound =upperY;
		}

		for(int currentY = lowerYBound; currentY <= upperYBound; currentY++) { //for each y-pixel
			drawPx(currentX, currentY, colour); //draw a pixel at the current position
		}
	}
}

WNDCLASSEX& createWindowClass(HINSTANCE hinstance) {
	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hinstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = HBRUSH(COLOR_WINDOW); //sets colour of window to default
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = g_szClassName;
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&windowClass))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
	return windowClass;
}

//TODO: Create window objects with OOP
HWND getHwnd(HINSTANCE hinstance) {
	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"MaltFXv1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hinstance, NULL);
	
	return hwnd;
}

bool windowSuccess(HWND hwnd) {
	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	else {
		return true;
	}
}

void screen::clear() {
	for (int i = 0; i < width;i++) {
		for(int j = 0; j<height;j++) {
			drawPx(i, j, 0x000000);
		}
	}
}

screen::screen(int _width, int _height, HWND hwnd) {
	width = _width;
	height = _height;
	screenHDC = GetDC(hwnd);
	picHDC = CreateCompatibleDC(NULL);
	blankHDC = CreateCompatibleDC(NULL);
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biHeight = (-1)*height;
	bitmapinfo.bmiHeader.biWidth = width;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;

	// ReSharper disable once CppCStyleCast
	HBITMAP bmp = CreateDIBSection(picHDC, &bitmapinfo, DIB_RGB_COLORS, (VOID**)&pixArray, 0, 0);
	// ReSharper disable once CppCStyleCast
	HBITMAP blankbmp = CreateDIBSection(blankHDC, &bitmapinfo, DIB_RGB_COLORS, (VOID**)&blank, 0, 0);
	HBITMAP bmpblankold = (HBITMAP(SelectObject(blankHDC, blankbmp)));
	HBITMAP bmpold = (HBITMAP(SelectObject(picHDC, bmp)));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void screen::drawPx(int x, int y, DWORD colour) {
	if (y > 0 && y<height && x>0 && x<width) {
		pixArray[(y*width) + x] = colour;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void screen::refresh() {
	BitBlt(screenHDC, 0, 0, width, height, picHDC, 0, 0, SRCCOPY);
}

int screen::getHeight() const {
	return height;
}

int screen::getWidth() const {
	return width;
}