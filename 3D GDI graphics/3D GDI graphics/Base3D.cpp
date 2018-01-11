#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"

using namespace utils;



void Camera::calculateDistance(screen& sc) {
	int height = sc.getHeight()/2;
	float angle = fov.getRadsExact() / 2;

	distance = float(height / tan(angle));
}

CoOrdSysManager::CoOrdSysManager() : systems(*new vector<CoOrdinateSystem>(1)), globalCoOrdinateSystem(systems[0]) {
	clearSpots = vector<int>(0);
	systems[0].id = 0;
	systems[0].origin = Point(0, 0, 0);
}

Camera::Camera(UniversalPoint _origin, Angle _fov) {
	origin = _origin;
	fov = _fov;
	int test = (*origin.coOrdManager).newCoOrdSys(origin.globalPoint);
	id = test;
}

Camera::Camera() {
}

void UniversalPoint::transform(Vec3 transformVector) {
	globalPoint.transform(transformVector);
	for (int i = 0; i < children.size(), i++;) {
		children[i].refresh(globalPoint);
	}
}


void Point::transform(Vec3 transformVector) {
	coOrds[0] = coOrds[0] + transformVector.x;
	coOrds[1] = coOrds[1] + transformVector.y;
	coOrds[2] = coOrds[2] + transformVector.z;
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


//TODO:work out what this is
//TODO: remove this tag once no longer testing
// ReSharper disable once CppMemberFunctionMayBeStatic
void Point::refresh(Point globalPoint) const {
	Vec3 transformationVector;
	float test1 = globalPoint.coOrds[0];
	Point test2(0, 0, 0);
	float test4 = test2.coOrds[0];
	float test3 = test1 - test4;
	transformationVector.x = test3;
}



childCoOrdSys::childCoOrdSys(CoOrdinateSystem &_parent, Point _origin, int _Id) : parent(_parent) {
	origin = _origin;
	id = _Id;
	//TODO: register with coOrdSysManager?
}


Point findIntersect() {
	//TODO: fill in this
	return Point(0, 0, 0);
}

void World::setActiveCamera(Camera* newActiveCamera) {
	activeCamera = *newActiveCamera;
}

UniversalPoint World::createPoint(float x, float y, float z)
{
	println(systems.size());
	return UniversalPoint(Point(x,y,z), this);
}

UniversalPoint World::createPoint(Point point)
{
	return UniversalPoint(point, this);
}

void World::draw(screen sc) {
	activeCamera.calculateDistance(sc);
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
			int j = 0;
			while(j < face.verts.size()-2) { //number of tris in a poly = number of verts-2

				//variables for the second and third verts of the new tri
				UniversalPoint secondVert = faces[i].verts[j + 1];
				UniversalPoint thirdVert = faces[i].verts[j + 2];

				//create a vector of points, then push each of the chosen vertices to it
				vector<UniversalPoint> points(3);
				points[0] = firstVert;
				points[1] = secondVert;
				points[2] = thirdVert;

				//create a new face from the point vector, then add it to the vector of processed faces
				improvedFaces.push_back(Face(points));
				j++;
			}
		}
		else {
			improvedFaces.push_back(faces[i]); //add the face to the vector of tris if it is already a tri
		}
		i = i + 1;
	}

	//VISIBILITY CHECKING CODE (low processing cost check to see if the face is in the viewport
	int cameraId = activeCamera.id;
	vector<Face> visibleFaces; //a vector that will contain all faces that can be seen by the Camera
	i = 0;
	while(i<improvedFaces.size()) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible
		int j = 0;
		while(j < improvedFaces[i].verts.size()) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked
				Point p = improvedFaces[i].verts[j].getPoint(cameraId);
				
				//find polar co-ordinate style angles for each point
				Angle xyTheta = Angle(true, float(atan(improvedFaces[i].verts[j].getPoint(cameraId).coOrds[1] / improvedFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI));
				Angle xzTheta = Angle(true, float(atan(improvedFaces[i].verts[j].getPoint(cameraId).coOrds[2] / improvedFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI));
				
				//readability stuff \/
				float fovRads = activeCamera.fov.getRads();
				float &camDist = activeCamera.distance;
				int scHeight = sc.getHeight();

				//check if polar co-ordinates of the point are (not) within the fov of the Camera
				if (fovRads / 2*-1 > xzTheta.getRads() && xzTheta.getRads() > fovRads / 2 &&
					atan(camDist / scHeight)*-1 > xyTheta.getRads() && xyTheta.getRads() > atan(camDist / scHeight)) {
				} else { //if they are (ie not not) then a part of the face is definitely visible
					visible = true;
				}
			}
			j++;
		}
		if (visible) {
			
			//if the face is visible, add the face to a list of visible faces
			visibleFaces.push_back(improvedFaces[i]);
		}
		i++;
	}

	i = 0;
	while( i < visibleFaces.size()) { //for every visible face
		Face workingFace = visibleFaces[i]; //store the face currently being worked on
		vector<Point> facePoints(3); //vector of the Face's vertices
		float worldXs[3], worldYs[3], worldZs[3]; //
		for (int j = 0; j <= 2; j++) { //for each vertex of the tri
			println(workingFace.verts.size());
			println(j);
			facePoints[j] = workingFace.verts[j].getPoint(cameraId);
			worldXs[j] = facePoints[j].coOrds[0];
			worldYs[j] = facePoints[j].coOrds[1];
			worldZs[j] = facePoints[j].coOrds[2];
			
		}
		
		Point screenPoints[3];
		activeCamera.calculateDistance(sc);
		for (int j = 0; j <= 2; j++) {
			//X
			screenPoints[j].coOrds[0] = int(float(activeCamera.distance*(float(worldXs[j]) / worldZs[j])))-int(sc.getWidth()/2);
			//Y
			screenPoints[j].coOrds[1] = int(float(activeCamera.distance*(float(worldYs[j]) / worldZs[j])))+int(sc.getHeight()/2);
		}
		//DOESNT LIKE IT WHEN X VALS ARE THE SAME
		sc.drawDiagonal(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1],
			screenPoints[1].coOrds[0], screenPoints[1].coOrds[1], 0xFFFFFF);
		sc.drawDiagonal(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1],
			screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0xFFFFFF);
		sc.drawDiagonal(screenPoints[1].coOrds[0], screenPoints[1].coOrds[1],
			screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0xFFFFFF);
		sc.drawPx(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1], 0x00FFFF);
		sc.drawPx(screenPoints[1].coOrds[0], screenPoints[1].coOrds[1], 0xFFFFFF);
		sc.drawPx(screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0x0000FF);

		i++;
	}

	sc.refresh(); //copy the working bitmap to the actual screen
}


World::World() : activeCamera(*new Camera(UniversalPoint(Point(0,0,0), this), Angle(true,90))) {
	cameras.push_back(activeCamera);
}


Face::Face(vector<UniversalPoint> _verts) {
	verts = _verts;
}


UniversalPoint::UniversalPoint(Point _globalPoint, CoOrdSysManager *_coOrdManager)
{
	coOrdManager = _coOrdManager;
	globalPoint = _globalPoint; //TODO: Pointerize
}

float keepPositive(float input) {
	if (input < 0) {
		return -input;
	}
		return input;
}

// ReSharper disable CppInconsistentNaming
Point UniversalPoint::getPoint(int _Id) {
	// ReSharper restore CppInconsistentNaming
	CoOrdinateSystem target = (*coOrdManager).systems[_Id];
	Point origin = target.origin;
	Point workingPoint = Point(globalPoint.coOrds[0] - origin.coOrds[0], globalPoint.coOrds[1] - origin.coOrds[1], globalPoint.coOrds[2] - origin.coOrds[2]); 


	//around z-axis
	float radius;
	Angle zOriginAngle;
	if (workingPoint.coOrds[1] > 0) {
		if (workingPoint.coOrds[0] > 0) {
			zOriginAngle = Angle(true, float(atan(keepPositive(workingPoint.coOrds[0]) / keepPositive(workingPoint.coOrds[1])) / M_PI)); //tan(theta) = x/y;
		}
		else {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[1]) / keepPositive(workingPoint.coOrds[0]))) / M_PI, true); //tan(theta) = y/x
		}
	}
	else {
		if (workingPoint.coOrds[0] < 0) {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[0]) / keepPositive(workingPoint.coOrds[1]))) / M_PI, true); //tan(theta) = x/y
		}
		else {
			zOriginAngle = Angle((atan(keepPositive(workingPoint.coOrds[1]) / keepPositive(workingPoint.coOrds[0]))) / M_PI, true); //tan(theta) = y/x
		}

	}

	//ROTATION

	if (!origin.rotation[0].getRads() == 0) {
		//x-axis
		//transformations done on a plane perpendicular to the z-axis
		Angle startZtoPt = Angle(true, (atan(workingPoint.coOrds[3] / workingPoint.coOrds[2])) / M_2_PI);
		Angle endZtoPt = Angle(true, startZtoPt.getRads() + (origin.rotation[0].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[3], 2) + pow(workingPoint.coOrds[2], 2));
		workingPoint.coOrds[1] = (cos(endZtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[2] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}

	if (!origin.rotation[1].getRads() == 0) {
		//y-axis
		Angle startXtoPt = Angle(true, (atan(workingPoint.coOrds[0] / workingPoint.coOrds[3])) / M_2_PI);
		Angle endXtoPt = Angle(true, startXtoPt.getRads() + (origin.rotation[1].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[0], 2) + pow(workingPoint.coOrds[2], 2));
		workingPoint.coOrds[2] = (cos(endXtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}

	if (!origin.rotation[1].getRads() == 0) {
		//z-axis
		//transformations done on a plane perpendicular to the z-axis
		Angle startYtoPt = Angle(true, (atan(workingPoint.coOrds[0]/workingPoint.coOrds[1]))/M_2_PI);//the original Angle between the Y-axis and the line between the origin of the co-ord system and the point
		Angle endYtoPt = Angle(true, startYtoPt.getRads() + (origin.rotation[2].getRads()));
		radius = sqrt(pow(workingPoint.coOrds[0],2) + pow(workingPoint.coOrds[1],2));
		workingPoint.coOrds[1] = (cos(endYtoPt.getRadsExact()))*radius;
		workingPoint.coOrds[0] = sqrt(pow(radius, 2) - pow(workingPoint.coOrds[1], 2));
	}
	
	if (children.size() <= _Id) {
		children.resize(_Id+1);
	}
	children[_Id] = workingPoint;
	return workingPoint;
}

int CoOrdSysManager::newCoOrdSys(Point origin)
{
	int id;
	if (clearSpots.size() > 0) {
		id = clearSpots[0];
		systems[id] = static_cast<CoOrdinateSystem>(childCoOrdSys(globalCoOrdinateSystem, origin, id));
	} else {
		id = int(systems.size());
		systems.push_back(childCoOrdSys((globalCoOrdinateSystem), origin, id));
	}
	return id;
}

void CoOrdSysManager::removeCoOrdSys(int id)
{
	delete &systems[id];
	systems[id] = CoOrdinateSystem();
	clearSpots.push_back(id);
}

CoOrdinateSystem & CoOrdinateSystem::operator=(CoOrdinateSystem newCoOrdinateSystem)
{
	CoOrdinateSystem c = CoOrdinateSystem(newCoOrdinateSystem);
	swap(id, c.id);
	swap(origin, c.origin);
	return *this;
}
