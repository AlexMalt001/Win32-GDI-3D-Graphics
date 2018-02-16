#pragma once
#include "Base3D.h"
#include "UniversalPoint.h"



namespace base3D {
	class Object3D;
	class Face {
		friend Object3D;
		//TODO: add a back reference to the object the face belongs to for texturing
	public:
		vector<UniversalPoint> verts;//TODO:Pointerise
		Face(vector<UniversalPoint> verts);
		void rotate(Point origin, Angle xyTheta, Angle yzTheta, Angle xzTheta);
	protected:
		void transform(Vec3 transformationVector);
	};
}
