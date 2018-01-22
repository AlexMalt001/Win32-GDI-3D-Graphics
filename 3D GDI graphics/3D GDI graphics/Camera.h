#pragma once
#include "Base3D.h"
#include "UniversalPoint.h"

using namespace base3D;

class Camera : WorldObject {
public: //TODO: the encapsulation in this is actually painful
		//childCoOrdSys cameraCoOrdSys;

	UniversalPoint getOrigin();
	float distance = 0.0;
	Angle fov;
	void calculateDistance(screen& sc);
	int id = 0;
	void setOrigin(Point p);

	//TODO: constructors only for World
	/*protected*/Camera(UniversalPoint origin, Angle fov, World* newWorld);
	/*protected*/Camera();
};