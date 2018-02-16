#include "Object3D.h"

using namespace base3D;

void Object3D::addFace(Face& newFace) {
	faces.push_back(newFace);
}

void Object3D::transform(Vec3 transformationVector) {
	for(int i = 0; i<faces.size(); i++) {
		faces[i].transform(transformationVector);
	}
}

void Object3D::rotate(Point origin, Angle xyTheta, Angle yzTheta, Angle xzTheta) {
	for (int i = 0; i < faces.size(); i++) {
		faces[i].rotate(origin, xyTheta, yzTheta, xzTheta);
	}
}
