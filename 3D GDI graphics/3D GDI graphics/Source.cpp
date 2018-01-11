#include <Windows.h>
#include <stdlib.h>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#include "Base3D.h"
#include "Utils.h"
//ReSharper enable CppUnusedIncludeDirective

//draw test objects
namespace testing {
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
	testing::makeCube(mainWorld, 10, -40, 100, 30, 30, 30);
	testing::makeCube(mainWorld, 10, 10, 100, 30, 30, 30);
	testing::makeCube(mainWorld, -40, 10, 100, 30, 30, 30);
	testing::makeCube(mainWorld, -40, -40, 100, 30, 30, 30);
	

	sc.drawDiagonal(0, 0, width, height, 0x00FF00);
	sc.drawPx(width/2, height/2, 0xFF0000);
	sc.refresh();
	//camera not registering with co-ord-sys-manager
	Camera newCam = Camera(UniversalPoint(Point(0, 0, 0), &mainWorld), Angle(false, 90));
	mainWorld.cameras.push_back(newCam);
	mainWorld.setActiveCamera(&newCam);
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

