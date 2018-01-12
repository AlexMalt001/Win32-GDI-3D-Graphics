#include "Point.h"
using namespace base3D;

void Point::transform(Vec3 transformVector) {
	coOrds[0] = coOrds[0] + transformVector.x;
	coOrds[1] = coOrds[1] + transformVector.y;
	coOrds[2] = coOrds[2] + transformVector.z;
}

Point& Point::operator=(Point point) {
	Point p = Point(point);
	swap(coOrds, p.coOrds);
	swap(rotation, p.rotation);
	return *this;
}

Point::Point(const float x, const float y, const float z) {
	rotation[0] = Angle(true, 0);
	rotation[1] = Angle(true, 0);
	rotation[2] = Angle(true, 0);
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
}

Point::Point(float x, float y, float z, Angle xRot, Angle yRot, Angle zRot)
{
	rotation[0] = xRot;
	rotation[1] = yRot;
	rotation[2] = zRot;
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
}

Point findIntersect() {
	//TODO: fill in this
	return Point(0, 0, 0);
}