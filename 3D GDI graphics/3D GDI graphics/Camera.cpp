#include "Camera.h"
#include "Base3D.h"
#include "World.h"
#include "UniversalPoint.h"


using namespace base3D;

UniversalPoint Camera::getOrigin() {
	return UniversalPoint((*world).getSystem(id).origin, world);
}


void Camera::setOrigin(Point p) {
	(*world).setSystemOrigin(id, p);
}

Camera::Camera(UniversalPoint _origin, Angle _fov, World* newWorld) : WorldObject(newWorld) {
	fov = _fov;
	id = (*_origin.coOrdManager).newCoOrdSys(_origin.globalPoint);
}

Camera::Camera() : WorldObject() {
}

void Camera::calculateDistance(screen& sc) {
	int height = sc.getHeight() / 2;
	float angle = fov.getRadsExact() / 2;

	distance = float(height / tan(angle));
}