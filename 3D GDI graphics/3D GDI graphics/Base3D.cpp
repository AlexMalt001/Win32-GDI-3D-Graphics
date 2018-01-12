#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"
#include "Point.h"
#include "World.h"

using namespace base3D;
using namespace utils;




void Camera::calculateDistance(screen& sc) {
	int height = sc.getHeight()/2;
	float angle = fov.getRadsExact() / 2;

	distance = float(height / tan(angle));
}

CoOrdSysManager::CoOrdSysManager() : systems(*new vector<CoOrdinateSystem>(1)){
	globalCoOrdinateSystem = &systems[0];
	clearSpots = vector<int>(0);
	systems[0].id = 0;
	systems[0].origin = Point(0, 0, 0);
}

void Camera::setOrigin(Point p) {
	(*world).setSystemOrigin(id, p);
}

Camera::Camera(UniversalPoint _origin, Angle _fov, World* newWorld) : WorldObject(newWorld) {
	fov = _fov;
	id = (*_origin.coOrdManager).newCoOrdSys(_origin.globalPoint);
}

Camera::Camera() : WorldObject(){
}

void UniversalPoint::transform(Vec3 transformVector) {
	globalPoint.transform(transformVector);
}

childCoOrdSys::childCoOrdSys(CoOrdinateSystem* _parent, Point _origin, int _Id) : parent(_parent) {
	origin = _origin;
	id = _Id;
	//TODO: register with coOrdSysManager?
}


Face::Face(vector<UniversalPoint> _verts) {
	verts = _verts;
}


UniversalPoint::UniversalPoint(Point _globalPoint, CoOrdSysManager *_coOrdManager)
{
	coOrdManager = _coOrdManager;
	globalPoint = _globalPoint; //TODO: Pointerize
}

float keepPositive(float input) {
	if (input < 0) {
		return -input;
	}
		return input;
}

// ReSharper disable CppInconsistentNaming
Point UniversalPoint::getPoint(int _Id) {
	// ReSharper restore CppInconsistentNaming
	CoOrdinateSystem target = (*coOrdManager).getSystem(_Id);
	Point origin = target.origin;
	Point workingPoint = Point(globalPoint.coOrds[0] - origin.coOrds[0], globalPoint.coOrds[1] - origin.coOrds[1], globalPoint.coOrds[2] - origin.coOrds[2]); 


	//around z-axis
	float radius;
	Angle zOriginAngle;
	if (workingPoint.coOrds[1] > 0) {
		if (workingPoint.coOrds[0] > 0) {
			zOriginAngle = Angle(true, float(atan(keepPositive(workingPoint.coOrds[0]) / keepPositive(workingPoint.coOrds[1])) / M_PI)); //tan(theta) = x/y;
		}
		else {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[1]) / keepPositive(workingPoint.coOrds[0]))) / M_PI, true); //tan(theta) = y/x
		}
	}
	else {
		if (workingPoint.coOrds[0] < 0) {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[0]) / keepPositive(workingPoint.coOrds[1]))) / M_PI, true); //tan(theta) = x/y
		}
		else {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[1]) / keepPositive(workingPoint.coOrds[0]))) / M_PI, true); //tan(theta) = y/x
		}

	}

	//ROTATION

	if (!origin.rotation[0].getRads() == 0) {
		//x-axis
		//transformations done on a plane perpendicular to the z-axis
		Angle startZtoPt = Angle(true, (atan(workingPoint.coOrds[3] / workingPoint.coOrds[2])) / M_2_PI);
		Angle endZtoPt = Angle(true, startZtoPt.getRads() + (origin.rotation[0].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[3], 2) + pow(workingPoint.coOrds[2], 2));
		workingPoint.coOrds[1] = (cos(endZtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[2] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}

	if (!origin.rotation[1].getRads() == 0) {
		//y-axis
		Angle startXtoPt = Angle(true, (atan(workingPoint.coOrds[0] / workingPoint.coOrds[3])) / M_2_PI);
		Angle endXtoPt = Angle(true, startXtoPt.getRads() + (origin.rotation[1].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[0], 2) + pow(workingPoint.coOrds[2], 2));
		workingPoint.coOrds[2] = (cos(endXtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}

	if (!origin.rotation[1].getRads() == 0) {
		//z-axis
		//transformations done on a plane perpendicular to the z-axis
		Angle startYtoPt = Angle(true, (atan(workingPoint.coOrds[0]/workingPoint.coOrds[1]))/M_2_PI);//the original Angle between the Y-axis and the line between the origin of the co-ord system and the point
		Angle endYtoPt = Angle(true, startYtoPt.getRads() + (origin.rotation[2].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[0],2) + pow(workingPoint.coOrds[1],2));
		workingPoint.coOrds[1] = (cos(endYtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}
	
	if (children.size() <= _Id) {
		children.resize(_Id+1);
	}
	children[_Id] = workingPoint;
	return workingPoint;
}

WorldObject::WorldObject(World* newWorld) {
	world = newWorld;
}

void WorldObject::setWorld(World* newWorld) {
	world = newWorld;
}

UniversalPoint Camera::getOrigin() {
	return UniversalPoint((*world).getSystem(id).origin, world);
}

int CoOrdSysManager::newCoOrdSys(Point origin)
{
	int id;
	if (clearSpots.size() > 0) {
		id = clearSpots[0];
		systems[id] = static_cast<CoOrdinateSystem>(childCoOrdSys(globalCoOrdinateSystem, origin, id));
	} else {
		id = int(systems.size());
		systems.push_back(childCoOrdSys((globalCoOrdinateSystem), origin, id));
	}
	return id;
}

void CoOrdSysManager::removeCoOrdSys(int id)
{
	delete &systems[id];
	systems[id] = CoOrdinateSystem();
	clearSpots.push_back(id);
}

void CoOrdSysManager::setSystemOrigin(int id, Point p) {
	systems[id].origin = p;
}

CoOrdinateSystem CoOrdSysManager::getSystem(int id) {
	return systems[id];
}

CoOrdinateSystem & CoOrdinateSystem::operator=(CoOrdinateSystem newCoOrdinateSystem)
{
	CoOrdinateSystem c = CoOrdinateSystem(newCoOrdinateSystem);
	swap(id, c.id);
	swap(origin, c.origin);
	return *this;
}
