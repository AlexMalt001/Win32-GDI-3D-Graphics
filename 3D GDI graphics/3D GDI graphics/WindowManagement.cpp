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

	if (y1 != y2) {
		if (x1 != x2) {
			//TODO: good AA
			int greaterX = (x1 > x2) ? x1 : x2;
			int greaterY = (x1 > x2) ? y1 : y2;
			int lesserX = (x1 > x2) ? x2 : x1;
			int lesserY = (x1 > x2) ? y2 : y1;

			float deriv = float(greaterY - lesserY) / (greaterX - lesserX);//find dy/dx of line

			float remainingY = deriv;
			int currentX = 0;
			int currentY = 0;
			while (currentX < greaterX - lesserX && currentY != greaterY - lesserY) {
				drawPx(currentX + lesserX, currentY + lesserY, colour);
				if (remainingY > 0.5) {
					currentY++;
					remainingY--;
				}
				else if (remainingY < -0.5) {

					currentY--;
					remainingY++;
				}
				else {
					currentX++;
					remainingY += deriv;
				}

			}
		} else { //if same x coord
			int greaterY = (y1 > y2) ? y1 : y2;
			int lesserY = (y1 > y2) ? y2 : y1;

			int currentY = 0;
			while(currentY+lesserY != greaterY) {
				drawPx(x1, currentY + lesserY, colour);
				currentY++;
			}
		}
	} else {
		int greaterX = (x1 > x2) ? x1 : x2;
		int lesserX = (x1 > x2) ? x2 : x1;

		int currentX = 0;
		while (currentX + lesserX != greaterX) {
			drawPx(currentX+lesserX,y1, colour);
			currentX++;
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
	for (int i = 0; i < width; i++) {
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
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biHeight = (-1)*height;
	bitmapinfo.bmiHeader.biWidth = width;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;

	// ReSharper disable once CppCStyleCast
	HBITMAP bmp = CreateDIBSection(picHDC, &bitmapinfo, DIB_RGB_COLORS, (VOID**)&pixArray, 0, 0);

	HBITMAP bmpold = (HBITMAP(SelectObject(picHDC, bmp)));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void screen::drawPx(int x, int y, DWORD colour) {
	if (y > 0 && y<height) {
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