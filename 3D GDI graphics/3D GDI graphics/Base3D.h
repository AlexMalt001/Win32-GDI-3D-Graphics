#pragma once
#define ThreeDVertDims 3
#include "Utils.h"


class threeDEnvironment {
	public:
		static const int tester = 2;

};

class camera : public threeDEnvironment {
	public:
		angle fov; //fov in degrees
		camera(angle &fov);
	private:
		float distance;
		void calculateDistance();
};

class vec3 {
public:
	float x;
	float y;
	float z;
	vec3(float x, float y, float z);
	vec3 operator+(vec3 &other);
};

class Point : public threeDEnvironment {
public:
	float verts[ThreeDVertDims];

};