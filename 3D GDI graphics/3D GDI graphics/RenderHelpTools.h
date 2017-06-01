#pragma once
#include <vector>
#include "Base3D.h"

using namespace std;

class ItemPixel {
	public:
		vector<Object3D*> collidingObjs;
		bool collision;
};

class Point2D{
	public:
		int x;
		int y;
		short vertex;
		Point2D(int x, int y, short vertex);
};

class Face2D {
	public:
		vector<Point2D> points;
		Face* faceReference;
};