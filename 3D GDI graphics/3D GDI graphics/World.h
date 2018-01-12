#pragma once
#include "WindowManagement.h"
#include "Base3D.h"



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
		void draw(screen sc);
		World();
	};
}

