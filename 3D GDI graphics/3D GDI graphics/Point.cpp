#include "Point.h"
#include <corecrt_math_defines.h>
#include <vector>
using namespace base3D;

void Point::transform(Vec3 transformVector) {
	coOrds[0] = coOrds[0] + transformVector.x;
	coOrds[1] = coOrds[1] + transformVector.y;
	coOrds[2] = coOrds[2] + transformVector.z;
}

vector<float> rotate2D(float axis1, float axis2, Angle addedTheta) {
	//rotates this point about the origin
	if(axis1<0) {
		int test = 0;
	}
	float distance = 0;
	Angle theta = Angle();
	if (axis2>0) { //if in top half
		//works for top right and top left
		theta.setRadsExact(atan(axis1 / axis2)); //find the angle in this quadrant
		//as these are in the top side, there is no missed quadrant; the angle is < 90deg
		theta.setRads(theta.getRads() + addedTheta.getRads()); //change the angle by however much we want to rotate by
	}
	else { //if in bottom half
		if (axis1>0) {//bottom right
			theta.setRadsExact(atan(axis2 / axis1)); //find the angle in this quadrant
			theta.setRads(0.5 - theta.getRads()); //correct the polarity, add the missed quadrant (90degs for top left)
			theta.setRads(theta.getRads() + addedTheta.getRads()); //change the angle by however much we want to rotate by
		}
		else { //bottom left
			theta.setRadsExact(atan(axis2 / axis1));
			theta.setRads(-0.5 - theta.getRads()); //correct the polarity, add the missed quadrant back (90degs for top right)
			theta.setRads(theta.getRads() + addedTheta.getRads()); //change the angle by however much we want to rotate by
		}
	}

	theta.setRads(theta.getRads() + 1); //convert -180<->180 to 0<->360 
	theta.setRads(fmod(theta.getRads(), 2)); //trim angle to always be between 0 and 360
	theta.setRads(theta.getRads() - 1); //convert 0<->360 to -180<->180 for easier conversion back to coOrds
	distance = sqrt(axis1*axis1+axis2*axis2);
	
	if (theta.getRads()>0) {
		if (theta.getRads()<0.5) { //top right
			axis1 = distance*sin(theta.getRadsExact());
			axis2 = distance*cos(theta.getRadsExact());
		}
		else { //bottom right
			theta.setRads(1 - theta.getRads());
			axis1 = distance*sin(theta.getRadsExact());
			axis2 = -1 * distance*cos(theta.getRadsExact());
		}
	} else {
		if (theta.getRads() > (-0.5)) { //top left
			axis1 = distance*sin(theta.getRadsExact());
			axis2 = distance*cos(theta.getRadsExact());
		}
		else { //bottom left
			theta.setRads(0.5 + theta.getRads()); //get rid of top left quadrant
			axis1 = -1* distance*cos(theta.getRadsExact());
			axis2 = distance*sin(theta.getRadsExact());
		}
	}
	return vector<float> {axis1, axis2};

}

void Point::rotate(Angle xyTheta, Angle yzTheta, Angle xzTheta) {
	float x = coOrds[0];
	float y = coOrds[1];
	float z = coOrds[2];

	if(x<1) {
		int test = 0;
	}


	//XY plane
	vector<float> xy = rotate2D(x, -y, xyTheta);
	x = xy[0];
	y = -xy[1];
	//XZ plane
	vector<float> xz = rotate2D(x, z, xzTheta);
	x = xz[0];
	z = xz[1];
	//YZ plane
	vector<float> yz = rotate2D(y, z, yzTheta);
	y = yz[0];
	z = yz[1];

	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
	
}

Point& Point::operator=(Point point) {
	Point p = Point(point);
	swap(coOrds, p.coOrds);
	swap(rotation, p.rotation);
	return *this;
}

Point::Point(const float x, const float y, const float z) {
	rotation[0] = Angle(true, 0);
	rotation[1] = Angle(true, 0);
	rotation[2] = Angle(true, 0);
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
	
}

Point::Point(float x, float y, float z, Angle xRot, Angle yRot, Angle zRot)
{
	rotation[0] = xRot;
	rotation[1] = yRot;
	rotation[2] = zRot;
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
}

Point findIntersect() {
	//TODO: fill in this
	return Point(0, 0, 0);
}