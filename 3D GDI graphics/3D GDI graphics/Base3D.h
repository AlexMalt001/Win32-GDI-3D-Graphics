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

using namespace base3D;
using namespace utils;
using namespace std;

class World;

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

class WorldObject {
	protected:
		World* world;
		WorldObject(World* newWorld);
		WorldObject() : world(nullptr) {} ;
		void setWorld(World* newWorld);
};

class Camera : WorldObject{
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

class World : public CoOrdSysManager {
public:
	void setActiveCamera(Camera *newActiveCamera);
	UniversalPoint createPoint(float x,float y, float  z);
	UniversalPoint createPoint(Point point);
	//TODO: 'createCamera();
	vector<Camera> cameras;
	vector<Object3D> objects;
	Camera &activeCamera;
	void draw(screen sc);
	World();
};
