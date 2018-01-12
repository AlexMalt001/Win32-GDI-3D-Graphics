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

class UniversalPoint { //TODO: inherit from Point
public:
	UniversalPoint() = default;
	CoOrdSysManager *coOrdManager;
	Point globalPoint;
	void transform(Vec3 transformVector);
	UniversalPoint(Point _globalPoint, CoOrdSysManager *coOrdManager);
	Point getPoint(int id);
private:
	vector<Point> children;
};

class WorldObject { //class from which all types that can exist in the world derive
	protected:
		World* world;
		WorldObject(base3D::World* newWorld);
		WorldObject() : world(nullptr) {} ;
		void setWorld(base3D::World* newWorld);
};



class Face {
	//TODO: add a back reference to the object the face belongs to for texturing
	public:
		vector<UniversalPoint> verts;//TODO:Pointerise
		Face(vector<UniversalPoint> verts);

};

class Object3D {
	public:
		vector<Face> faces;
		Point CoG;
		//TODO: add texturing stuff
};
