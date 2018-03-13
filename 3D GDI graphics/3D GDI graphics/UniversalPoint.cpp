#include "UniversalPoint.h"
#include "Utils.h"

void UniversalPoint::transform(Vec3 transformVector) {
	globalPoint.transform(transformVector);
}

UniversalPoint::UniversalPoint(Point _globalPoint, CoOrdSysManager *_coOrdManager)
{
	coOrdManager = _coOrdManager;
	globalPoint = _globalPoint;
}

Point UniversalPoint::getPoint(int _Id) {
	// ReSharper restore CppInconsistentNaming
	CoOrdinateSystem target = (*coOrdManager).getSystem(_Id);
	Point origin = target.origin;
	Point workingPoint = getPointWithOrigin(globalPoint, origin);
	return workingPoint;
}

void UniversalPoint::setPoint(Point newPoint, int _Id) {
	globalPoint = getPointFromOrigin(newPoint, (*coOrdManager).getSystem(_Id).origin);
}

Point UniversalPoint::getPointWithOrigin(Point newPoint, Point origin) {
	return Point(newPoint.coOrds[0] - origin.coOrds[0], //change the x
				 newPoint.coOrds[1] - origin.coOrds[1],
				 newPoint.coOrds[2] - origin.coOrds[2]);
}

Point UniversalPoint::getPointFromOrigin(Point newPoint, Point origin) {
	return Point(newPoint.coOrds[0] + origin.coOrds[0], //change the x
				 newPoint.coOrds[1] + origin.coOrds[1],
				 newPoint.coOrds[2] + origin.coOrds[2]);
}

void UniversalPoint::rotate(Point origin, Angle xyTheta, Angle yzTheta, Angle xzTheta) {
	Point workingPoint = getPointWithOrigin(globalPoint, origin);
	workingPoint.rotate(xyTheta,yzTheta,xzTheta);
	globalPoint = getPointFromOrigin(workingPoint, origin);
}



