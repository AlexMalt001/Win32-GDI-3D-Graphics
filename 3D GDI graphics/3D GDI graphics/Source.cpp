#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#include "Base3D.h"
#include "Utils.h"
#include "World.h"
#include "Camera.h"
#include "Face.h"
#include "Object3D.h"


//ReSharper enable CppUnusedIncludeDirective

//draw test objects
namespace testing {
	void runTests(World& mainWorld, screen& sc) {
		while (true) {
			Sleep(10);
			sc.clear();
			mainWorld.activeCamera.setOrigin(Point(mainWorld.activeCamera.getOrigin().globalPoint.coOrds[0] + 1, 0, 0));
			mainWorld.draw(sc);
			
		}

	}

	void makeCube(World &mainWorld, int x, int y, int z, int width, int height, int depth) {
		Point frontTopLeft		= Point(x,		 y,			z);
		Point frontTopRight		= Point(x+width, y,			z);
		Point frontBottomRight	= Point(x+width, y+height,	z);
		Point frontBottomLeft	= Point(x,		 y+height,	z);

		Point backTopLeft		= Point(x,		 y,			z+depth);
		Point backTopRight		= Point(x+width, y,			z+depth);
		Point backBottomRight	= Point(x+width, y+height,	z+depth);
		Point backBottomLeft	= Point(x,		 y+height,	z+depth);

		//front
		vector<UniversalPoint> frontFace;
		frontFace.push_back(mainWorld.createPoint(frontTopLeft));
		frontFace.push_back(mainWorld.createPoint(frontTopRight));
		frontFace.push_back(mainWorld.createPoint(frontBottomRight));
		frontFace.push_back(mainWorld.createPoint(frontBottomLeft));
		//back
		vector<UniversalPoint> backFace;
		backFace.push_back(mainWorld.createPoint(backTopLeft));
		backFace.push_back(mainWorld.createPoint(backTopRight));
		backFace.push_back(mainWorld.createPoint(backBottomRight));
		backFace.push_back(mainWorld.createPoint(backBottomLeft));
		//left
		vector<UniversalPoint> leftFace;
		leftFace.push_back(mainWorld.createPoint(frontTopLeft));
		leftFace.push_back(mainWorld.createPoint(backTopLeft));
		leftFace.push_back(mainWorld.createPoint(backBottomLeft));
		leftFace.push_back(mainWorld.createPoint(frontBottomLeft));

		vector<UniversalPoint> rightFace;
		rightFace.push_back(mainWorld.createPoint(frontTopRight));
		rightFace.push_back(mainWorld.createPoint(backTopRight));
		rightFace.push_back(mainWorld.createPoint(backBottomRight));
		rightFace.push_back(mainWorld.createPoint(frontBottomRight));
		//bottom
		vector<UniversalPoint> bottomFace;
		bottomFace.push_back(mainWorld.createPoint(frontBottomLeft));
		bottomFace.push_back(mainWorld.createPoint(backBottomLeft));
		bottomFace.push_back(mainWorld.createPoint(backBottomRight));
		bottomFace.push_back(mainWorld.createPoint(frontBottomRight));

		vector<UniversalPoint> topFace;
		topFace.push_back(mainWorld.createPoint(frontTopLeft));
		topFace.push_back(mainWorld.createPoint(backTopLeft));
		topFace.push_back(mainWorld.createPoint(backTopRight));
		topFace.push_back(mainWorld.createPoint(frontTopRight));

		Object3D obj;
		obj.faces.push_back(Face(frontFace));
		obj.faces.push_back(Face(backFace));
		obj.faces.push_back(Face(leftFace));
		obj.faces.push_back(Face(rightFace));
		obj.faces.push_back(Face(bottomFace));
		obj.faces.push_back(Face(topFace));

		mainWorld.objects.push_back(obj);
	}
}

typedef struct MyData{
	screen* sc;
	World* mainWorld;
	MyData(screen* _sc, World* _mainWorld) {
		sc = _sc;
		mainWorld = _mainWorld;
	}
}MYDATA;

void clear(screen& sc) {
	sc.clear();
}

DWORD WINAPI mythread(__in LPVOID lpParameter) {
	MYDATA data = *((MYDATA*)lpParameter);
	while(true) {
		Sleep(100);
		(*data.sc).clear();
		(*data.mainWorld).objects[0].rotate(Point(30,10,50), Angle(false, 0), Angle(false, 0),Angle(false,0));
		clear(*data.sc);
		(*data.mainWorld).draw(*data.sc);
		
		(*data.sc).refresh();
	}
	return 0;
}


// ReSharper disable once CppInconsistentNaming
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int width = 500;
	int height = 500;

	createWindowClass(hinstance);
	HWND hwnd = getHwnd(hinstance);
	if (windowSuccess(hwnd) == false) {
		return 0;
	}
	//SetWindowPos(hwnd, NULL, 0, 0, width+50, height+50, SWP_NOMOVE);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	SetWindowPos(hwnd, NULL, 10, 10, width+20, height+43, 0);
	//create a class for easier window handling or something
	MSG Msg;
	screen sc(width, height, hwnd);

	World mainWorld;
	//testing::makeCube(mainWorld, 40, -30, 100, 30, 30, 30);
	//testing::makeCube(mainWorld, 40, 0, 130, 30, 30, 30);
	//testing::makeCube(mainWorld, 20, 0, 160, 30, 30, 30);
	//testing::makeCube(mainWorld, 20, -30, 190, 30, 30, 30);
	//testing::makeCube(mainWorld, 40, -30, 220, 30, 30, 30);
	testing::makeCube(mainWorld, 30, 30, 100, 20, 20, 20);

	//camera not registering with co-ord-sys-manager
	Camera newCam = Camera(UniversalPoint(Point(0, 0, -100 ,Angle(false,0), Angle(false, 0), Angle(false,0)), &mainWorld), Angle(false, 90), &mainWorld);
	
	mainWorld.cameras.push_back(newCam);
	mainWorld.setActiveCamera(&newCam);
	mainWorld.draw(sc);

	sc.drawPx((width / 2), height / 2, 0xFF0000);
	sc.refresh();
	// ReSharper disable CppUnreachableCode

	MYDATA dataptr(&sc, &mainWorld);
	
	DWORD mythreadid;
	CreateThread(0, 0, mythread, &dataptr, 0, &mythreadid);
	
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		
		/*Sleep(100);
		sc.clear();
		mainWorld.activeCamera.setOrigin(Point(mainWorld.activeCamera.getOrigin().globalPoint.coOrds[0] + 1, 0, 0));
		mainWorld.draw(sc);
		sc.clear();
		sc.refresh();*/
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return int(Msg.wParam);
	// ReSharper restore CppUnreachableCode

}

