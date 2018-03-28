#pragma once
#include "WindowManagement.h"
#include "Base3D.h"
#include "Camera.h"
#include "Object3D.h"


namespace base3D {
	class World : public CoOrdSysManager {
	public:
		void setActiveCamera(Camera *newActiveCamera);
		UniversalPoint createPoint(float x, float y, float  z);
		UniversalPoint createPoint(Point point);
		//TODO: 'createCamera();
		vector<Camera> cameras;
		vector<Object3D> objects;
		Camera &activeCamera;
		void draw(screen sc, int drawDist);
		World();
	private:
		vector<Face> seperateFaces();
		vector<Face> triangulate(vector<Face> faces);
		vector<Face> cullInvisible(vector<Face> faces, screen sc, int drawDist);
		void drawProcessedFaces(vector<Face> faces, screen sc);
		vector<Point> convertToScreenPoints(vector<Point> facePoints, screen sc);
	};
}

