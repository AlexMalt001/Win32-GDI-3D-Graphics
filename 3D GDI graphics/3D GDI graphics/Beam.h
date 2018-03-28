#pragma once
#include "Ray.h"
#include "Object3D.h"

namespace base3D {
    class Beam {
        public :
            Beam* findIntersect(Object3D obj);
        private:
            Ray edges[4];   
    };
}