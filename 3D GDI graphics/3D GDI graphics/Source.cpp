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
	//TODO: center of camera is in top left. this is NFG.
	Point p1 = Point(25,23,100);
	Point p2 = Point(42, 12, 100);
	Point p3 = Point(20, 50, 100);

	Point p4 = Point(5, 15, 50);
	Point p5 = Point(10, 10, 50);
	Point p6 = Point(20, 12, 50);
	Point p7 = Point(20, 20, 50);
	Point p8 = Point(10, 20, 50);
	
	

	vector<UniversalPoint> points;
	points.push_back(mainWorld.createPoint(p1));
	points.push_back(mainWorld.createPoint(p2));
	points.push_back(mainWorld.createPoint(p3));

	Face face = Face(points);

	vector<UniversalPoint> points2;
	points2.push_back(mainWorld.createPoint(p4));
	points2.push_back(mainWorld.createPoint(p5));
	points2.push_back(mainWorld.createPoint(p6));
	points2.push_back(mainWorld.createPoint(p7));
	points2.push_back(mainWorld.createPoint(p8));

	Face face2 = Face(points2);

	Object3D obj;
	obj.faces.push_back(face);

	Object3D obj2;
	obj.faces.push_back(face2);
	
	mainWorld.objects.push_back(obj);
	mainWorld.objects.push_back(obj2);

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
