#pragma once
#include "Base3D.h"
#include "Face.h"

class Object3D {
public:
	vector<Face> faces;
	Point CoG;
	//TODO: add texturing stuff
};
