#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"
#include "3DComponents.h"
#include "Base3D.h"


const int tester = 2;


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	createWindowClass(hinstance);
	HWND hwnd = getHwnd(hinstance);
	if (windowSuccess(hwnd) == false) {
		return 0;
	}
	SetWindowPos(hwnd, NULL,10,10,512,512,SWP_NOMOVE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//create a class for easier window handling or something

	MSG Msg;

	
	screen sc(512, 512, hwnd);
	

	World mainWorld;

	while (1 != 0) {
		mainWorld.draw(sc);
	}

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
