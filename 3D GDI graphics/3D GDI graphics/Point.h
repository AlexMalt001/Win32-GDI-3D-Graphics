#pragma once
#include "Utils.h"
using namespace utils;

namespace base3D {
	class Point {
	public:
		Point() {}
		Point& operator=(Point point);
		float coOrds[3];
		Angle rotation[3];
		Point(float x, float y, float z);
		Point(float x, float y, float z, Angle xRot, Angle yRot, Angle zRot);
		void transform(Vec3 transformVector);
		void rotate(Angle xyTheta, Angle yzTheta, Angle xzTheta);
	};
}