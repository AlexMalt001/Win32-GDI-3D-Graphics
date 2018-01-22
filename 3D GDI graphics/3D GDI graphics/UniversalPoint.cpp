#include "UniversalPoint.h"
#include "Utils.h"

void UniversalPoint::transform(Vec3 transformVector) {
	globalPoint.transform(transformVector);
}

UniversalPoint::UniversalPoint(Point _globalPoint, CoOrdSysManager *_coOrdManager)
{
	coOrdManager = _coOrdManager;
	globalPoint = _globalPoint; //TODO: Pointerize
}

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
		Angle startYtoPt = Angle(true, (atan(workingPoint.coOrds[0] / workingPoint.coOrds[1])) / M_2_PI);//the original Angle between the Y-axis and the line between the origin of the co-ord system and the point
		Angle endYtoPt = Angle(true, startYtoPt.getRads() + (origin.rotation[2].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[0], 2) + pow(workingPoint.coOrds[1], 2));
		workingPoint.coOrds[1] = (cos(endYtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}

	if (children.size() <= _Id) {
		children.resize(_Id + 1);
	}
	children[_Id] = workingPoint;
	return workingPoint;
}

