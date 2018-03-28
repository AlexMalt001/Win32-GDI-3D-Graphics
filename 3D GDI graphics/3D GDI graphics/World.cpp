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

void World::draw(screen sc, int drawDist) {
	activeCamera.calculateDistance(sc);
	//REMOVE ALL FACES IN SCENE FROM THEIR ASSOCIATED OBJECTS
	vector<Face> faces; //vector that will contain all faces in the world
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

	//CODE TO BREAK DOWN EACH FACE INTO TRIANGLES
	vector<Face> triangulatedFaces; //vector that will contain the newly broken down face
	i = 0;
	while (i < faces.size()) { //for every face
		if (faces[i].verts.size() > 3) { //face only needs to be processed if it is not already a tri
			Face face = faces[i]; //reference to current face. readability.
			UniversalPoint firstVert = faces[i].verts[0]; //first vertex of the current face. for readability
			int j = 0;
			while (j < face.verts.size() - 2) { //number of tris in a poly = number of verts-2

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
	while (i<triangulatedFaces.size()) { //for every face
		bool visible = false; //a variable that will be changed to determine whether or not part of the face is visible
		int j = 0;
		while (j < triangulatedFaces[i].verts.size()) { //for every vertex of the face

			if (!visible) { //if one vertex of the face has been found to be visible, at least some part of the face is visible, therefore the other points dont need to be checked
				Point p = triangulatedFaces[i].verts[j].getPoint(cameraId);

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
			j++;
		}

		if (visible) {

			//if the face is visible, add the face to a list of visible faces
			visibleFaces.push_back(triangulatedFaces[i]);
		}
		i++;
	}

	i = 0;
	while (i < visibleFaces.size()) { //for every visible face
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
			screenPoints[j].coOrds[0] = int(float(activeCamera.distance*(float(worldXs[j]) / worldZs[j]))) + int(sc.getWidth() / 2);
			//Y

			screenPoints[j].coOrds[1] = int(float(activeCamera.distance*(float(worldYs[j]) / worldZs[j]))) + int(sc.getHeight() / 2);
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