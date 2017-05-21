#include "Base3D.h"
#include <math.h>

vec3 vec3::operator+(vec3 &other) {
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3::vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

camera::camera(angle &fov) {
	angle test = fov;
	calculateDistance();
}

void camera::calculateDistance() {
	int height = 1080; //TODO: get height from window
	int angle = fov.getRads() / 2;
	distance = height / tan(angle);
}