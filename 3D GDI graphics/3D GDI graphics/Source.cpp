#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#include "Base3D.h"
#include "Utils.h"
//ReSharper enable CppUnusedIncludeDirective



int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	createWindowClass(hinstance);
	HWND hwnd = getHwnd(hinstance);
	if (windowSuccess(hwnd) == false) {
		return 0;
	}
	SetWindowPos(hwnd, NULL,10,10,700,700,SWP_NOMOVE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//create a class for easier window handling or something

	MSG Msg;

	
	screen sc(700, 700, hwnd);
	
	World mainWorld;
	//TODO: center of camera is in top left. this is NFG.
	//Point p1 = Point(25,23,100);
	//Point p2 = Point(42, 12, 100);
	//Point p3 = Point(20, 50, 100);

	Point p4 = Point(10, 30, 100);
	Point p5 = Point(50, 30, 100);
	Point p6 = Point(50, 80, 100);
	Point p7 = Point(10, 80, 100);

	Point p8 = Point(10, 30, 200);
	Point p9 = Point(50, 30, 200);
	Point p10 = Point(50, 80, 200);
	Point p11 = Point(10, 80, 200);

	Point p12 = Point(10, 30, 100); 
	Point p13 = Point(10, 30, 200); 
	Point p14 = Point(10, 80, 200);
	Point p15 = Point(10, 80, 100);

	Point p16 = Point(50, 30, 100);
	Point p17 = Point(50, 30, 200);
	Point p18 = Point(50, 80, 200);
	Point p19 = Point(50, 80, 100);

	Point p20 = Point(10, 30, 100);
	Point p21 = Point(10, 30, 200);
	Point p22 = Point(50, 30, 200);
	Point p23 = Point(50, 30, 100);

	Point p24 = Point(10, 80, 100);
	Point p25 = Point(10, 80, 200);
	Point p26 = Point(50, 80, 200);
	Point p27 = Point(50, 80, 100);
	

	vector<UniversalPoint> points2;
	points2.push_back(mainWorld.createPoint(p4));
	points2.push_back(mainWorld.createPoint(p5));
	points2.push_back(mainWorld.createPoint(p6));
	points2.push_back(mainWorld.createPoint(p7));

	vector<UniversalPoint> points3;
	points3.push_back(mainWorld.createPoint(p8));
	points3.push_back(mainWorld.createPoint(p9));
	points3.push_back(mainWorld.createPoint(p10));
	points3.push_back(mainWorld.createPoint(p11));

	vector<UniversalPoint> points4;
	points4.push_back(mainWorld.createPoint(p12));
	points4.push_back(mainWorld.createPoint(p13));
	points4.push_back(mainWorld.createPoint(p14));
	points4.push_back(mainWorld.createPoint(p15));

	vector<UniversalPoint> points5;
	points5.push_back(mainWorld.createPoint(p16));
	points5.push_back(mainWorld.createPoint(p17));
	points5.push_back(mainWorld.createPoint(p18));
	points5.push_back(mainWorld.createPoint(p19));

	vector<UniversalPoint> points6;
	points6.push_back(mainWorld.createPoint(p20));
	points6.push_back(mainWorld.createPoint(p21));
	points6.push_back(mainWorld.createPoint(p22));
	points6.push_back(mainWorld.createPoint(p23));

	vector<UniversalPoint> points7;
	points7.push_back(mainWorld.createPoint(p24));
	points7.push_back(mainWorld.createPoint(p25));
	points7.push_back(mainWorld.createPoint(p26));
	points7.push_back(mainWorld.createPoint(p27));

	Face face2 = Face(points2);
	Face face3 = Face(points3);
	Face face4 = Face(points4);
	Face face5 = Face(points5);
	Face face6 = Face(points6);
	Face face7 = Face(points7);

	//Object3D obj;
	//obj.faces.push_back(face);

	Object3D obj2;
	obj2.faces.push_back(face2);
	obj2.faces.push_back(face3);
	obj2.faces.push_back(face4);
	obj2.faces.push_back(face5);
	obj2.faces.push_back(face6);
	obj2.faces.push_back(face7);


	
	//mainWorld.objects.push_back(obj);
	mainWorld.objects.push_back(obj2);

	/*Camera newCam = Camera(UniversalPoint(Point(0, 0, 0), &mainWorld), Angle(false, 90));
	mainWorld.cameras.push_back(newCam);
	mainWorld.setActiveCamera(&newCam);*/
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
