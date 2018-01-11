#include "Utils.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace utils;

Angle::Angle(): inRads(false) {}

Angle Angle::operator=(const Angle &other) {
	inRads = other.inRads;
	angleDegs = other.angleDegs;
	angleRads = other.angleRads;
	return *this;
}

Angle::Angle(bool _inRads, float value) {
	if (_inRads == true) {
		angleRads = value;
		angleDegs = value*180;
	} else {
		angleDegs = value;
		angleRads = value / 180;
	}
	inRads = _inRads;
}

float Angle::getRadsExact() const {
	return float(getRads()*M_PI);
}

float Angle::getRads() const {
	return angleRads;
}

float Angle::getDegs() const {
	return angleDegs;
}

void Angle::setRadsExact(float value) {
	setRads(float(value/M_PI));
}

void Angle::setRads(float value) {
	angleRads = value;
	angleDegs = value * 180;
	inRads = true;
}

void Angle::setDegs(float value) {
	angleDegs = value;
	angleRads = value / 180;
	inRads = false;
}

