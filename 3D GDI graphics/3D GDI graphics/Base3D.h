#pragma once

#include "Utils.h"
#include <vector>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Point.h"

// ReSharper restore CppUnusedIncludeDirective

namespace base3D {
	class World;//forward declare base3D::World; this class is necessarily loaded before it
	//class UniversalPoint;
}

using namespace base3D;
using namespace utils;
using namespace std;

//class base3D::World;

class CoOrdinateSystem {
	public:
		int id;
		Point origin;
		CoOrdinateSystem& operator=(CoOrdinateSystem newCoOrdinateSystem);
};

class childCoOrdSys : public CoOrdinateSystem{
	public:
		CoOrdinateSystem* parent;
		childCoOrdSys(CoOrdinateSystem* parent, Point origin, int Id);
};

class CoOrdSysManager {
	public:
		CoOrdSysManager();
		int newCoOrdSys(Point origin);
		//TODO: 'updateSys(id);
		void removeCoOrdSys(int id);
		CoOrdinateSystem* globalCoOrdinateSystem;
		void setSystemOrigin(int id, Point p);
		CoOrdinateSystem getSystem(int id);
	protected:
		vector<int> clearSpots;
		vector <CoOrdinateSystem> systems;
};

class WorldObject { //class from which all types that can exist in the world derive
	protected:
		World* world;
		WorldObject(base3D::World* newWorld);
		WorldObject() : world(nullptr) {} ;
		void setWorld(base3D::World* newWorld);
};





