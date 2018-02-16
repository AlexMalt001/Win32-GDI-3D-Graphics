#pragma once
#include "Base3D.h"
#include "Face.h"

namespace base3D {

	class Object3D : WorldObject {
	public:
		void addFace(Face& newFace);
		void transform(Vec3 transformationVector);

	public:
		void rotate(Point origin, Angle xyTheta, Angle yzTheta, Angle xzTheta);
		vector<Face> faces;
		Point CoG;

		//TODO: add texturing stuff
	};
}