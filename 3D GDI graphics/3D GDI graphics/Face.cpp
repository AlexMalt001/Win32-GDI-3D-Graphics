#include "UniversalPoint.h"
#include "Face.h"

using namespace base3D;
Face::Face(vector<UniversalPoint> _verts) {
	verts = _verts;
}

void Face::transform(Vec3 transformationVector) {
	for (int i = 0; i < verts.size(); i++) {
		verts[i].transform(transformationVector);
	}
}
