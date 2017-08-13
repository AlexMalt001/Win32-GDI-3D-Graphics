#pragma once
#define ThreeDVertDims 3
#include "Utils.h"
#include <vector>
#include "WindowManagement.h"
#include "3DComponents.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace utils;
using namespace std;

class Point {
public:
	Point() {};
	float coOrds[ThreeDVertDims];
	angle rotation[ThreeDVertDims];
	Point(float x, float y, float z);
	Point(float x, float y, float z, angle xRot, angle yRot, angle zRot);
	void transform(vec3 transformVector);
	void refresh(Point globalPoint);

};

class CoOrdinateSystem {
	public:
		int Id;
		Point origin;
};

class childCoOrdSys : public CoOrdinateSystem{
	public:
		CoOrdinateSystem parent;
		childCoOrdSys(CoOrdinateSystem parent, Point origin);
		childCoOrdSys();
};

class CoOrdSysManager {
	public:
		CoOrdSysManager();
		int newCoOrdSys(Point origin);
		//TODO: 'updateSys(Id);
		void removeCoOrdSys(int Id);
		vector<int> clearSpots; 
		vector <CoOrdinateSystem> systems;
		CoOrdinateSystem *globalCoOrdinateSystem;
};

class UniversalPoint {
public:
	CoOrdSysManager *coOrdManager;
	Point globalPoint;
	void transform(vec3 transformVector);
	UniversalPoint() {};
	UniversalPoint(Point _globalPoint, CoOrdSysManager coOrdManager);
	Point getPoint(int Id);
private:
	vector<Point> children;
};

class camera {
public:
	//childCoOrdSys cameraCoOrdSys;
	UniversalPoint origin;
	float distance;
	angle fov;
	void calculateDistance(screen& sc);
	int Id;

	//TODO: constructors only for World
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
	UniversalPoint createPoint(float x,float y, float  z);
	UniversalPoint createPoint(Point point);
	//TODO: 'createCamera();
	vector<camera> cameras;
	vector<Object3D> objects;
	camera *activeCamera;
	int latestCoOrdId;
	void draw(screen sc);
	World();
};