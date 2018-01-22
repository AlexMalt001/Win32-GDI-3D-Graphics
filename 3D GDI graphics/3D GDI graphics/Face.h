#pragma once
#include "Base3D.h"
#include "UniversalPoint.h"


class Face {
	//TODO: add a back reference to the object the face belongs to for texturing
public:
	vector<UniversalPoint> verts;//TODO:Pointerise
	Face(vector<UniversalPoint> verts);

};