#pragma once
#include <Windows.h>

WNDCLASSEX& createWindowClass(HINSTANCE hinstance);
HWND getHwnd(HINSTANCE hinstance);
bool windowSuccess(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class screen {
	public:
		screen(int width, int height, HWND hwnd);
		void refresh();
		void drawPx(int x, int y, DWORD colour);
		int getHeight();
		int getWidth();
	private:
		int width, height;
		DWORD* pixArray;
		HDC screenHDC;
		HDC picHDC;
		HBITMAP bmp;
		HBITMAP bmpold;
};