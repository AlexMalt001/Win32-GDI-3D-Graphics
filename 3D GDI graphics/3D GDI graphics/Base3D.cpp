#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"

vec3 vec3::operator+(vec3 &other) {
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3::vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
z = _z;
}

void camera::calculateDistance() {
	int height = 1080; //TODO: get height from window
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

UniversalPoint::UniversalPoint(Point _globalPoint) {

	globalPoint = _globalPoint;

}

void Point::transform(vec3 transformVector) {

}

Point::Point(float x, float y, float z)
{
}

void Point::refresh(Point globalPoint) {
	vec3 transformationVector;
	float test1 = globalPoint.coOrds[0];
	Point test2(0, 0, 0);
	float test4 = test2.coOrds[0];
	float test3 = test1 - test4;
	transformationVector.x = test3;
}

vec3::vec3() {

}

childCoOrdSys::childCoOrdSys(CoOrdinateSystem _parent, Point _origin) {
	parent = _parent;
	origin = _origin;
}

childCoOrdSys::childCoOrdSys()
{
}

Point findIntersect() {
	return Point(0, 0, 0);
}

void World::draw(screen sc) {

	//REMOVE ALL FACES IN SCENE FROM THEIR ASSOCIATED OBJECTS
	vector<Face> faces; //vector that will contain all faces in the world
	for (int i = 0; i < objects.size(), i++;) { //for every object in the world
		Object3D object = objects[i]; //current object variable for ease of access

		for (int j = 0; j < object.faces.size(), i++;) { //for every face of each object
			faces.push_back(object.faces[j]); //add each face tp the final vector
		}
	}

	//CODE TO BREAK DOWN EACH FACE INTO TRIANGLES
	vector<Face> improvedFaces; //vector that willcontain the newly broken down faces
	for (int i = 0; i < faces.size(), i++;) { //for every face
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
	}

	//VISIBILITY CHECKING CODE (low processing cost check to see if the face is in the viewport
	int cameraId = activeCamera.cameraCoOrdSys.Id;
	vector<Face> visibleFaces; //a vector that will contain all faces that can be seen by the camera
	for (int i = 0; i < improvedFaces.size(), i++;) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible

		for (int j = 0; j < visibleFaces[i].verts.size(), j++;) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked

				//find polar co-ordinate style angles for each point
				angle wTheta = angle(true, visibleFaces[i].verts[j].children[cameraId].coOrds[0] / visibleFaces[i].verts[j].children[cameraId].coOrds[2]);
				angle hTheta = angle(true, visibleFaces[i].verts[j].children[cameraId].coOrds[1] / visibleFaces[i].verts[j].children[cameraId].coOrds[2]);

				//check if polar co-ordinates of the point are (not) within the fov of the camera
				if ((2 - (activeCamera.fov.getRads() / 2)) > wTheta.getRads() > activeCamera.fov.getRads() / 2 &&
					((2 - atan((sc.getHeight() / 2) / activeCamera.distance)) > hTheta.getRads() > activeCamera.fov.getRads() / 2)) {
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


	sc.refresh(); //copy the working bitmap to the actual screen
}

World::World() {
}

Face::Face(vector<UniversalPoint> _verts) {
	verts = _verts;
}


Point UniversalPoint::getPoint(int Id) {
	if (children.size() < Id) {

	}
	return Point(0, 0, 0);
}