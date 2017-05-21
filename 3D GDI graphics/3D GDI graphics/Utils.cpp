#include "Utils.h"
#define _USE_MATH_DEFINES
#include <math.h>

angle::angle() {

}

angle angle::operator=(angle &other) {
	inRads = other.inRads;
	angleDegs = other.angleDegs;
	angleRads = other.angleRads;
	return *this;
}

angle::angle(bool _inRads, float value) {
	if (_inRads == true) {
		angleRads = value;
		angleDegs = value*180;
	} else {
		angleDegs = value;
		angleRads = value / 180;
	}
	inRads = _inRads;
}

float angle::getRadsExact() {
	return float(getRads()*M_PI);
}

float angle::getRads() {
	return angleRads;
}

float angle::getDegs() {
	return angleDegs;
}

void angle::setRadsExact(float value) {
	setRads(float(value/M_PI));
}

void angle::setRads(float value) {
	angleRads = value;
	angleDegs = value * 180;
	inRads = true;
}

void angle::setDegs(float value) {
	angleDegs = value;
	angleRads = value / 180;
	inRads = false;
}



