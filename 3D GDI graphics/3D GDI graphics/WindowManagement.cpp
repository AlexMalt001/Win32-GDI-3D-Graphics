#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"


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

screen::screen(int _width, int _height, HWND hwnd) {
	width = _width;
	height = _height;
	screenHDC = GetDC(hwnd);
	picHDC = CreateCompatibleDC(NULL);
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biHeight = -512;
	bitmapinfo.bmiHeader.biWidth = 512;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;

	// ReSharper disable once CppCStyleCast
	HBITMAP bmp = CreateDIBSection(picHDC, &bitmapinfo, DIB_RGB_COLORS, (VOID**)&pixArray, 0, 0);

	HBITMAP bmpold = (HBITMAP(SelectObject(picHDC, bmp)));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void screen::drawPx(int x, int y, DWORD colour) {
	pixArray[(y*width) + x] = colour;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void screen::refresh() {
	BitBlt(screenHDC, 0, 0, 512, 512, picHDC, 0, 0, SRCCOPY);
}

int screen::getHeight() const {
	return height;
}

int screen::getWidth() const {
	return width;
}