#pragma once
#include "Base3D.h"




namespace base3D {
	class Face;

	class UniversalPoint { //TODO: inherit from Point
		friend Face;
	public:
		UniversalPoint() = default;
		CoOrdSysManager *coOrdManager;
		Point globalPoint;
		void transform(Vec3 transformVector);
		UniversalPoint(Point _globalPoint, CoOrdSysManager *coOrdManager);
		Point getPoint(int id);
		void setPoint(Point newPoint, int id);
		void rotate(Point origin, Angle xyTheta, Angle yzTheta, Angle xzTheta);
	private:
		vector<Point> children;
		static Point getPointWithOrigin(Point newPoint, Point origin);
		static Point getPointFromOrigin(Point newPoint, Point origin);
	};
}
