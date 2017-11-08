#pragma once
#define ThreeDVertDims 3
#include "Utils.h"
#include <vector>
#include "WindowManagement.h"
// ReSharper disable CppUnusedIncludeDirective
#include "3DComponents.h"
#define _USE_MATH_DEFINES
#include <math.h>
// ReSharper restore CppUnusedIncludeDirective

using namespace utils;
using namespace std;

class Point {
public:
	Point() {}
	Point& operator=(Point point);
	float coOrds[ThreeDVertDims];
	Angle rotation[ThreeDVertDims];
	Point(float x, float y, float z);
	Point(float x, float y, float z, Angle xRot, Angle yRot, Angle zRot);
	void transform(Vec3 transformVector);
	void refresh(Point globalPoint) const;
};

class CoOrdinateSystem {
	public:
		int id;
		Point origin;
		CoOrdinateSystem& operator=(CoOrdinateSystem newCoOrdinateSystem);
};

class childCoOrdSys : public CoOrdinateSystem{
	public:
		CoOrdinateSystem &parent;
		childCoOrdSys(CoOrdinateSystem &parent, Point origin, int Id);
};

class CoOrdSysManager {
	public:
		CoOrdSysManager();
		int newCoOrdSys(Point origin);
		//TODO: 'updateSys(id);
		void removeCoOrdSys(int id);
		vector<int> clearSpots; 
		vector <CoOrdinateSystem> systems;
		CoOrdinateSystem &globalCoOrdinateSystem;
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

class Camera {
public: //TODO: the encapsulation in this is actually painful
	//childCoOrdSys cameraCoOrdSys;
	UniversalPoint origin;
	float distance = 0.0; 
	Angle fov;
	void calculateDistance(screen& sc);
	int id = 0;

	//TODO: constructors only for World
	Camera(UniversalPoint origin, Angle fov);
	Camera();
};

class Face {
	//TODO: add a back reference to the object the face belongs to for texturing
	public:
		vector<UniversalPoint> verts;
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
	UniversalPoint createPoint(float x,float y, float  z);
	UniversalPoint createPoint(Point point);
	//TODO: 'createCamera();
	vector<Camera> cameras;
	vector<Object3D> objects;
	Camera &activeCamera;
	void draw(screen sc);
	World();
};
//test