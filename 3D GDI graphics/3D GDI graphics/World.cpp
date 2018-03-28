#include "World.h"
#include "Face.h"
#include "Object3D.h"

using namespace base3D;

World::World() : activeCamera(*new Camera(UniversalPoint(Point(0, 0, 0), this), Angle(true, 90), this)) {
	cameras.push_back(activeCamera);
}

void World::setActiveCamera(Camera* newActiveCamera) {
	activeCamera = *newActiveCamera;
}

UniversalPoint World::createPoint(float x, float y, float z)
{
	println(systems.size());
	return UniversalPoint(Point(x, y, z), this);
}

UniversalPoint World::createPoint(Point point)
{
	return UniversalPoint(point, this);
}

vector<Face> World::seperateFaces() { //seperates all faces from their parent objects
	vector<Face> faces;
	int i = 0;
	while (i<objects.size()) { //for every object in the world

		Object3D object = objects[i]; //current object variable for ease of access
		int j = 0;
		while (j < object.faces.size()) { //for every face of each object
			faces.push_back(object.faces[j]); //add each face tp the final vector
			j = j + 1;
		}
		i = i + 1;
	}
	return faces;
}

vector<Face> World::triangulate(vector<Face> faces) {
	//CODE TO BREAK DOWN EACH FACE INTO TRIANGLES
	vector<Face> triangulatedFaces; //vector that will contain the newly broken down faces
	for(int i = 0; i < faces.size(); i++) { //for every face
		if (faces[i].verts.size() > 3) { //face only needs to be processed if it is not already a tri
			Face face = faces[i]; //reference to current face. readability.
			UniversalPoint firstVert = faces[i].verts[0]; //first vertex of the current face. for readability
			for(int j = 0; j < face.verts.size() - 2; j++) { //number of tris in a poly = number of verts-2

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
			}
		}
		else {
			triangulatedFaces.push_back(faces[i]); //add the face to the vector of tris if it is already a tri
		}
	}
	return triangulatedFaces;
}

vector<Face> World::cullInvisible(vector<Face> faces, screen sc, int drawDist) {
	//VISIBILITY CHECKING CODE (low processing cost check to see if the face is in the viewport
	int cameraId = activeCamera.id;
	vector<Face> visibleFaces; //a vector that will contain all faces that can be seen by the Camera
	for(int i = 0; i<faces.size(); i++) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible
		for(int j = 0; j < faces[i].verts.size(); j++) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked
				Point p = faces[i].verts[j].getPoint(cameraId);

				if(p.coOrds[2] < drawDist) {

					//find polar co-ordinate style angles for each point
					Angle xyTheta = Angle(true, float(atan(p.coOrds[1] / p.coOrds[0]) / M_2_PI));
					Angle xzTheta = Angle(true, float(atan(p.coOrds[2] / p.coOrds[0]) / M_2_PI));

					//vars made local for convenience \/
					float fovRads = activeCamera.fov.getRads();
					float &camDist = activeCamera.distance;
					int scHeight = sc.getHeight();

					//check if polar co-ordinates of the point are (not) within the fov of the Camera
					if (!(fovRads / 2 * -1 > xzTheta.getRads() && xzTheta.getRads() > fovRads / 2 &&
						atan(camDist / scHeight)*-1 > xyTheta.getRads() && xyTheta.getRads() > atan(camDist / scHeight))) {

						visible = true; //if they are within the camera's FOV range, set them to visible
					}
				}
			}
		}

		if (visible) {

			//if the face is visible, add the face to a list of visible faces
			visibleFaces.push_back(faces[i]);
		}
	}
	return visibleFaces;
}

vector<Point> World::convertToScreenPoints(vector<Point> facePoints, screen sc) {
	vector<Point> screenPoints(3);
	//find the intersection of the ray from each vert to the camera point with the viewing plane
	for (int j = 0; j <= 2; j++) {
		//X
		screenPoints[j].coOrds[0] = float(int(activeCamera.distance*(facePoints[j].coOrds[0] / facePoints[j].coOrds[2])) + int(sc.getWidth() / 2));
		
		//Y
		screenPoints[j].coOrds[1] = float(int(activeCamera.distance*(facePoints[j].coOrds[1] / facePoints[j].coOrds[2])) + int(sc.getHeight() / 2));
	}
	return screenPoints;
}


void World::drawProcessedFaces(vector<Face> faces, screen sc) {
	for(int i = 0; i < faces.size(); i++) { //for every visible face
		vector<Point> facePoints = faces[i].getPointArray(activeCamera.id); //vector of the Face's vertices
		
		//DRAWING CODE
		vector<Point> screenPoints = convertToScreenPoints(facePoints, sc); //array to hold the onscreen location of each vert of the current tri

		//draw the object
		sc.drawDiagonal(int(screenPoints[0].coOrds[0]), int(screenPoints[0].coOrds[1]),
			int(screenPoints[1].coOrds[0]), int(screenPoints[1].coOrds[1]), 0xFFFFFF);
		sc.drawDiagonal(int(screenPoints[0].coOrds[0]), int(screenPoints[0].coOrds[1]),
			int(screenPoints[2].coOrds[0]), int(screenPoints[2].coOrds[1]), 0xFFFFFF);
		sc.drawDiagonal(int(screenPoints[1].coOrds[0]), int(screenPoints[1].coOrds[1]),
			int(screenPoints[2].coOrds[0]), int(screenPoints[2].coOrds[1]), 0xFFFFFF);
		//highlight each corner, for testing
		sc.drawPx(int(screenPoints[0].coOrds[0]), int(screenPoints[0].coOrds[1]), 0x00FFFF);
		sc.drawPx(int(screenPoints[1].coOrds[0]), int(screenPoints[1].coOrds[1]), 0xFFFFFF);
		sc.drawPx(int(screenPoints[2].coOrds[0]), int(screenPoints[2].coOrds[1]), 0x0000FF);
	}
}

void World::draw(screen sc, int drawDist) {
	activeCamera.calculateDistance(sc);
	//REMOVE ALL FACES IN SCENE FROM THEIR ASSOCIATED OBJECTS
	vector<Face> faces = seperateFaces(); //holds all faces in the world
	
	faces = triangulate(faces); //converts all n-gons into tris

	faces = cullInvisible(faces, sc, drawDist); //removes faces outside viewing area

	drawProcessedFaces(faces, sc);

	sc.refresh(); //copy the working bitmap to the actual screen
}