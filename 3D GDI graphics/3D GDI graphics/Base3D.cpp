#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"
#include "Point.h"

using namespace base3D;

using namespace utils;



void Camera::calculateDistance(screen& sc) {
	int height = sc.getHeight()/2;
	float angle = fov.getRadsExact() / 2;

	distance = float(height / tan(angle));
}

CoOrdSysManager::CoOrdSysManager() : systems(*new vector<CoOrdinateSystem>(1)){
	globalCoOrdinateSystem = &systems[0];
	clearSpots = vector<int>(0);
	systems[0].id = 0;
	systems[0].origin = Point(0, 0, 0);
}

void Camera::setOrigin(Point p) {
	(*world).setSystemOrigin(id, p);
}

Camera::Camera(UniversalPoint _origin, Angle _fov, World* newWorld) : WorldObject(newWorld) {
	fov = _fov;
	id = (*_origin.coOrdManager).newCoOrdSys(_origin.globalPoint);
}

Camera::Camera() : WorldObject(){
}

void UniversalPoint::transform(Vec3 transformVector) {
	globalPoint.transform(transformVector);
}

childCoOrdSys::childCoOrdSys(CoOrdinateSystem* _parent, Point _origin, int _Id) : parent(_parent) {
	origin = _origin;
	id = _Id;
	//TODO: register with coOrdSysManager?
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
	vector<Face> triangulatedFaces; //vector that will contain the newly broken down face
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
				triangulatedFaces.push_back(Face(points));
				j++;
			}
		}
		else {
			triangulatedFaces.push_back(faces[i]); //add the face to the vector of tris if it is already a tri
		}
		i = i + 1;
	}

	//VISIBILITY CHECKING CODE (low processing cost check to see if the face is in the viewport
	int cameraId = activeCamera.id;
	vector<Face> visibleFaces; //a vector that will contain all faces that can be seen by the Camera
	i = 0;
	while(i<triangulatedFaces.size()) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible
		int j = 0;
		while(j < triangulatedFaces[i].verts.size()) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked
				Point p = triangulatedFaces[i].verts[j].getPoint(cameraId);
				
				//find polar co-ordinate style angles for each point
				Angle xyTheta = Angle(true, float(atan(triangulatedFaces[i].verts[j].getPoint(cameraId).coOrds[1] / triangulatedFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI));
				Angle xzTheta = Angle(true, float(atan(triangulatedFaces[i].verts[j].getPoint(cameraId).coOrds[2] / triangulatedFaces[i].verts[j].getPoint(cameraId).coOrds[0])/M_2_PI));
				
				//vars made local for convenience \/
				float fovRads = activeCamera.fov.getRads();
				float &camDist = activeCamera.distance;
				int scHeight = sc.getHeight();

				//check if polar co-ordinates of the point are (not) within the fov of the Camera
				if (!(fovRads / 2*-1 > xzTheta.getRads() && xzTheta.getRads() > fovRads / 2 &&
					atan(camDist / scHeight)*-1 > xyTheta.getRads() && xyTheta.getRads() > atan(camDist / scHeight))) {
	
					visible = true; //if they are within the camera's FOV range, set them to visible
				}
			}
			j++;
		}
		if (visible) {
			
			//if the face is visible, add the face to a list of visible faces
			visibleFaces.push_back(triangulatedFaces[i]);
		}
		i++;
	}

	i = 0;
	while( i < visibleFaces.size()) { //for every visible face
		Face workingFace = visibleFaces[i]; //store the face currently being worked on
		vector<Point> facePoints(3); //vector of the Face's vertices
		float worldXs[3], worldYs[3], worldZs[3]; //create an an array for the point in the world of each vert of the tri
		for (int j = 0; j <= 2; j++) { //for each vertex of the tri
			facePoints[j] = workingFace.verts[j].getPoint(cameraId); //get the vertex we are working on from the camera's perspective
			
			//add each x,y,z to array of xs, ys and zs of the tri
			worldXs[j] = facePoints[j].coOrds[0];
			worldYs[j] = facePoints[j].coOrds[1];
			worldZs[j] = facePoints[j].coOrds[2];
			
		}
		

		//DRAWING CODE
		Point screenPoints[3]; //array to hold the onscreen location of each vert of the current tri
		activeCamera.calculateDistance(sc);//refresh distance between camera and viewing plane

		//find the intersection of the ray from each vert to the camera point with the viewing plane
		for (int j = 0; j <= 2; j++) {
			//X
			screenPoints[j].coOrds[0] = int(float(activeCamera.distance*(float(worldXs[j]) / worldZs[j])))+int(sc.getWidth()/2);
			//Y

			screenPoints[j].coOrds[1] = int(float(activeCamera.distance*(float(worldYs[j]) / worldZs[j])))+int(sc.getHeight()/2);
		}

		//draw the object
		sc.drawDiagonal(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1],
			screenPoints[1].coOrds[0], screenPoints[1].coOrds[1], 0xFFFFFF);
		sc.drawDiagonal(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1],
			screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0xFFFFFF);
		sc.drawDiagonal(screenPoints[1].coOrds[0], screenPoints[1].coOrds[1],
			screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0xFFFFFF);
		//highlight each corner, for testing
		sc.drawPx(screenPoints[0].coOrds[0], screenPoints[0].coOrds[1], 0x00FFFF);
		sc.drawPx(screenPoints[1].coOrds[0], screenPoints[1].coOrds[1], 0xFFFFFF);
		sc.drawPx(screenPoints[2].coOrds[0], screenPoints[2].coOrds[1], 0x0000FF);

		i++;
	}

	sc.refresh(); //copy the working bitmap to the actual screen
}


World::World() : activeCamera(*new Camera(  UniversalPoint(Point(0,0,0), this)  , Angle(true,90)  , this)) {
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
	CoOrdinateSystem target = (*coOrdManager).getSystem(_Id);
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

WorldObject::WorldObject(World* newWorld) {
	world = newWorld;
}

void WorldObject::setWorld(World* newWorld) {
	world = newWorld;
}

UniversalPoint Camera::getOrigin() {
	return UniversalPoint((*world).getSystem(id).origin,world);
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

void CoOrdSysManager::setSystemOrigin(int id, Point p) {
	systems[id].origin = p;
}

CoOrdinateSystem CoOrdSysManager::getSystem(int id) {
	return systems[id];
}

CoOrdinateSystem & CoOrdinateSystem::operator=(CoOrdinateSystem newCoOrdinateSystem)
{
	CoOrdinateSystem c = CoOrdinateSystem(newCoOrdinateSystem);
	swap(id, c.id);
	swap(origin, c.origin);
	return *this;
}
