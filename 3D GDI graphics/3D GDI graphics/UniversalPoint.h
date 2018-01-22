#pragma once
#include "Base3D.h"


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