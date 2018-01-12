#pragma once
#include <Windows.h>

WNDCLASSEX& createWindowClass(HINSTANCE hinstance);
HWND getHwnd(HINSTANCE hinstance);
bool windowSuccess(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class screen {
	public:
		void drawDiagonal(int x1, int y1 ,int x2, int y2, DWORD colour);
		screen(int width, int height, HWND hwnd);
		void refresh();
		void drawPx(int x, int y, DWORD colour);
		int getHeight() const;
		int getWidth() const;
		void clear();
	private:
		int width, height;
		DWORD* pixArray;
		DWORD* blank;
		HDC screenHDC;
		HDC picHDC;
		HDC blankHDC;
};