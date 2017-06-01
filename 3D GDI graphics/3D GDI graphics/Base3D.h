#pragma once
#define ThreeDVertDims 3
#include "Utils.h"
#include <vector>
#include "WindowManagement.h"
#include "3DComponents.h"
#define _USE_MATH_DEFINES
#include <math.h>


using namespace std;

class Point {
public:
	Point() {};
	float coOrds[ThreeDVertDims];
	float rotation[ThreeDVertDims];
	Point(float x, float y, float z);
	void transform(vec3 transformVector);
	void refresh(Point globalPoint);

};

class CoOrdinateSystem {
	public:
		int Id;
};

class childCoOrdSys : public CoOrdinateSystem{
	public:
		CoOrdinateSystem parent;
		Point origin;
		childCoOrdSys(CoOrdinateSystem parent, Point origin);
		childCoOrdSys();
};

class CoOrdSysManager {
	vector <CoOrdinateSystem> systems;
	CoOrdinateSystem globalCoOrdinateSystem;
};

class UniversalPoint {
public:
	CoOrdSysManager coOrdManager;
	vector<Point> children;
	Point globalPoint;
	void transform(vec3 transformVector);
	UniversalPoint() {};
	UniversalPoint(Point _globalPoint);
	Point getPoint(int Id);
};

class camera {
public:
	childCoOrdSys cameraCoOrdSys;
	UniversalPoint origin;
	float distance;
	angle fov;
	void calculateDistance();
	int Id;
	camera(UniversalPoint origin, angle fov);
	camera();
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
	vector<camera> cameras;
	vector<Object3D> objects;
	camera activeCamera;
	int latestCoOrdId;
	void draw(screen sc);
	World();
};