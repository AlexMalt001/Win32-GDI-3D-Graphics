#include "Utils.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace utils;

Angle::Angle(): inRads(false) {} //default constructor. value of inRads does not matter at this point

Angle Angle::operator=(const Angle &other) {
	//copy other angles values to this angle
	inRads = other.inRads;
	angleDegs = other.angleDegs;
	angleRads = other.angleRads;
	return *this; //return self
}

Angle::Angle(bool _inRads, float value) {
	if (_inRads == true) { //if value in radians
		angleRads = value; //update radians
		angleDegs = value*180; //then convert and update degrees
	} else { //if value in degrees
		angleDegs = value; //update degrees
		angleRads = value / 180; //then convert and update radians
	}
	inRads = _inRads; //updates this
}

float Angle::getRadsExact() const {
	return float(getRads()*M_PI); //convert angleRads to normal radians and return
}

float Angle::getRads() const {
	return angleRads;
}

float Angle::getDegs() const {
	return angleDegs;
}

void Angle::setRadsExact(float value) { 
	setRads(float(value/M_PI)); //converts normal radians to rads/PI
}

void Angle::setRads(float value) { 
	angleRads = value; //stores new value
	angleDegs = value * 180; //convert rads/PI to degrees
	inRads = true; //updates this 
}

void Angle::setDegs(float value) {
	angleDegs = value; //stores new value
	angleRads = value / 180; //convert degrees to rads/PI
	inRads = false; //updates this
}

