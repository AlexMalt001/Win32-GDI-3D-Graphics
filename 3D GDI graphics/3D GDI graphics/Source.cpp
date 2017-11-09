#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#include "Base3D.h"
#include "Utils.h"
//ReSharper enable CppUnusedIncludeDirective

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

	println(mainWorld.activeCamera.id);
	println(mainWorld.activeCamera.id);
	Point p1 = Point(20,20,100);
	println(mainWorld.activeCamera.id);
	Point p2 = Point(40, 10, 200);
	Point p3 = Point(20, 50, 100);

	vector<UniversalPoint> points;
	points.push_back(mainWorld.createPoint(p1));
	points.push_back(mainWorld.createPoint(p2));
	points.push_back(mainWorld.createPoint(p3));

	Face face = Face(points);

	Object3D obj;
	obj.faces.push_back(face);
	
	mainWorld.objects.push_back(obj);

	mainWorld.draw(sc);

	// ReSharper disable CppUnreachableCode
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return int(Msg.wParam);
	// ReSharper restore CppUnreachableCode
}
