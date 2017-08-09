#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"

using namespace utils;

vec3 vec3::operator+(vec3 &other) {
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3::vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void camera::calculateDistance(screen& sc) {
	int height = sc.getHeight();
	int angle = fov.getRads() / 2;
	distance = height / tan(angle);
}

camera::camera(UniversalPoint origin, angle fov) {
}

camera::camera() {
}

void UniversalPoint::transform(vec3 transformVector) {
	globalPoint.transform(transformVector);
	for (int i = 0; i < children.size(), i++;) {
		children[i].refresh(globalPoint);
	}
}

UniversalPoint::UniversalPoint(Point _globalPoint, CoOrdSysManager _coOrdManager) {
	coOrdManager = _coOrdManager;
	globalPoint = _globalPoint;

}

void Point::transform(vec3 transformVector) {
	coOrds[0] = coOrds[0] + transformVector.x;
	coOrds[1] = coOrds[1] + transformVector.y;
	coOrds[2] = coOrds[2] + transformVector.z;
}

Point::Point(float x, float y, float z)
{
	rotation[0] = angle(true, 0);
	rotation[1] = angle(true, 0);
	rotation[2] = angle(true, 0);
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
}

Point::Point(float x, float y, float z, angle xRot, angle yRot, angle zRot)
{
	rotation[0] = xRot;
	rotation[1] = yRot;
	rotation[2] = zRot;
	coOrds[0] = x;
	coOrds[1] = y;
	coOrds[2] = z;
}

void Point::refresh(Point globalPoint) {
	vec3 transformationVector;
	float test1 = globalPoint.coOrds[0];
	Point test2(0, 0, 0);
	float test4 = test2.coOrds[0];
	float test3 = test1 - test4;
	transformationVector.x = test3;
	//TODO work out what this is
}

vec3::vec3() {
}

childCoOrdSys::childCoOrdSys(CoOrdinateSystem _parent, Point _origin) {
	parent = _parent;
	origin = _origin;
	//TODO: register with coOrdSysManager?
}

childCoOrdSys::childCoOrdSys()
{
}

Point findIntersect() {
	//TODO: fill in this
	return Point(0, 0, 0);
}

UniversalPoint World::createPoint(float x, float y, float z)
{
	return UniversalPoint(Point(x,y,z),*this);
}

UniversalPoint World::createPoint(Point point)
{
	return UniversalPoint(point,*this);
}

void World::draw(screen sc) {
		
	//REMOVE ALL FACES IN SCENE FROM THEIR ASSOCIATED OBJECTS
	vector<Face> faces; //vector that will contain all faces in the world
	int i = 0;
	while(i<objects.size()) { //for every object in the world

		Object3D object = objects[i]; //current object variable for ease of access
		int j = 0;
		while (j < object.faces.size()) { //for every face of each object
			faces.push_back(object.faces[j]); //add each face tp the final vector
			j = j + 1;
		}
		i = i + 1;
	}

	//CODE TO BREAK DOWN EACH FACE INTO TRIANGLES
	vector<Face> improvedFaces; //vector that will contain the newly broken down face
	i = 0;
	while (i < faces.size()) { //for every face
		if (faces[i].verts.size() > 3) { //face only needs to be processed if it is not already a tri
			Face face = faces[i]; //reference to current face. readability.
			UniversalPoint firstVert = faces[i].verts[0]; //first vertex of the current face. for readability
			for (int j = 0; j < (faces[i].verts.size()) - 2, j++;) { //number of tris in a poly = number of verts-2

				//variables for the second and third verts of the new tri
				UniversalPoint secondVert = faces[i].verts[j + 1];
				UniversalPoint thirdVert = faces[i].verts[j + 2];

				//create a vector of points, then push each of the chosen vertices to it
				vector<UniversalPoint> points;
				points.push_back(firstVert);
				points.push_back(secondVert);
				points.push_back(thirdVert);

				//create a new face from the point vector, then add it to the vector of processed faces
				improvedFaces.push_back(Face(points));
			}
		}
		else {
			improvedFaces.push_back(faces[i]); //add the face to the vector of tris if it is already a tri
		}
		i = i + 1;
	}
	//VISIBILITY CHECKING CODE (low processing cost check to see if the face is in the viewport
	int cameraId = activeCamera.cameraCoOrdSys.Id;
	vector<Face> visibleFaces; //a vector that will contain all faces that can be seen by the camera
	for (int i = 0; i < improvedFaces.size(), i++;) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible

		for (int j = 0; j < visibleFaces[i].verts.size(), j++;) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked

				
				//find polar co-ordinate style angles for each point
				angle xyTheta = angle(true, atan(visibleFaces[i].verts[j].getPoint(cameraId).coOrds[1] / visibleFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI);
				angle xzTheta = angle(true, atan(visibleFaces[i].verts[j].getPoint(cameraId).coOrds[2] / visibleFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI);
				

				//check if polar co-ordinates of the point are (not) within the fov of the camera
				if ((activeCamera.fov.getRads() / 2)*(-1) > xzTheta.getRads() > activeCamera.fov.getRads() / 2 &&
					(atan(activeCamera.distance / sc.getHeight()))*(-1) > xyTheta.getRads() > atan(activeCamera.distance / sc.getHeight())) {
				}
				else { //if they are (ie not not) then a part of the face is definitely visible
					visible = true;
				}
			}
		}

		if (visible) {
			
			//if the face is visible, add the face to a list of visible faces
			visibleFaces.push_back(improvedFaces[i]);
		}
	}


	for (int i = 0; i < improvedFaces.size(), i++;) {
		Face workingFace = improvedFaces[i];
		Point facePoints[2];
		float worldXs[2], worldYs[2], worldZs[2];
		for (int j = 0; j < 2, j++;) {
			facePoints[j] = workingFace.verts[j].getPoint(cameraId);
			worldXs[j] = facePoints[j].coOrds[0];
			worldYs[j] = facePoints[j].coOrds[1];
			worldZs[j] = facePoints[j].coOrds[2];
		}

		int screenXs[2];
		int screenYs[2];
		for (int j = 0; j < 2, j++;) {
			//X
			screenXs[j] = activeCamera.distance*(worldXs[j] / worldZs[j]);
			//Y
			screenYs[j] = activeCamera.distance*(worldYs[j] / worldZs[j]);
		}

		//POINT0->POINT1
		int lowerXX, upperXX, lowerXY, upperXY;
		if (screenXs[0] > screenXs[1]) {
			lowerXX = screenXs[1];
			lowerXY = screenYs[1];
			upperXX = screenXs[0];
			upperXY = screenYs[0];
		}
		else {
			lowerXX = screenXs[0];
			lowerXY = screenYs[0];
			upperXX = screenXs[1];
			upperXY = screenYs[1];
		}
		for (int j = 0; j < (lowerXX - upperXX), i++;) {
			int deltaY = upperXY - lowerXY;
			int deltaX = upperXX - lowerXX;
			sc.drawPx(j,j*(deltaY/deltaX),0xFFFFFF);
		}


	}

	sc.refresh(); //copy the working bitmap to the actual screen
}

World::World() {
}

Face::Face(vector<UniversalPoint> _verts) {
	verts = _verts;
}


Point UniversalPoint::getPoint(int Id) {
	CoOrdinateSystem target = coOrdManager.systems[Id];
	Point origin = target.origin;
	Point workingPoint = Point(globalPoint.coOrds[0] - origin.coOrds[0], globalPoint.coOrds[1] - origin.coOrds[1], globalPoint.coOrds[2] - origin.coOrds[2]); 


	//rotation

	//z-axis
	//transformations done on a plane perpendicular to the z-axis
	angle startYtoPt = angle(true, (atan(workingPoint.coOrds[0]/workingPoint.coOrds[1]))/M_2_PI);//the original angle between the Y-axis and the line between the origin of the co-ord system and the point
	angle endYtoPt = angle(true, startYtoPt.getRads() + (origin.rotation[2].getRads()));
	float radius = sqrt(pow(workingPoint.coOrds[0],2) + pow(workingPoint.coOrds[1],2));
	workingPoint.coOrds[1] = (cos(endYtoPt.getRadsExact()))*radius;
	workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	

	//x-axis
	//transformations done on a plane perpendicular to the z-axis
	angle startZtoPt = angle(true, (atan(workingPoint.coOrds[3] / workingPoint.coOrds[2])) / M_2_PI);
	angle endZtoPt = angle(true, startZtoPt.getRads() + (origin.rotation[0].getRads()));
	radius = sqrt(pow(workingPoint.coOrds[3], 2) + pow(workingPoint.coOrds[2], 2));
	workingPoint.coOrds[1] = (cos(endZtoPt.getRadsExact()))*radius;
	workingPoint.coOrds[2] = sqrt(pow( radius, 2) - pow(workingPoint.coOrds[1], 2));

	//y-axis
	angle startXtoPt = angle(true, (atan(workingPoint.coOrds[0] / workingPoint.coOrds[3])) / M_2_PI);
	angle endXtoPt = angle(true, startXtoPt.getRads() + (origin.rotation[1].getRads()));
	radius = sqrt(pow(workingPoint.coOrds[0], 2) + pow(workingPoint.coOrds[2], 2));
	workingPoint.coOrds[2] = (cos(endXtoPt.getRadsExact()))*radius;
	workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));

	if (children.size() - 1 > Id) {
		children.resize(Id+1);
	}
	children[Id] = workingPoint;
	return workingPoint;
}

int CoOrdSysManager::newCoOrdSys(Point origin)
{
	int Id;
	if (clearSpots.size() != 0) {
		Id = clearSpots[0];
	} else {
		Id = systems.size();
	}
	systems[Id] = childCoOrdSys(globalCoOrdinateSystem, origin);
	return Id;
}

void CoOrdSysManager::removeCoOrdSys(int Id)
{
	delete &systems[Id];
	systems[Id] = CoOrdinateSystem();
	clearSpots.push_back(Id);
}
