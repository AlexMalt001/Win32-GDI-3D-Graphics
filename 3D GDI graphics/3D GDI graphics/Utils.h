#pragma once
#include <sstream>
using namespace std;

namespace utils {
	class angle {
		public:
			float getRadsExact();
			float getRads();
			float getDegs();
			void setRads(float angle);
			void setRadsExact(float angle);
			void setDegs(float angle);
			angle();
			angle(bool inRads, float value);
			angle operator=(angle &other);
		private:
			bool inRads;
			float angleDegs;
			float angleRads; //done between 0-2, not between 0-2PI, cause its nicer
	};

	class vec3 {
	public:
		float x;
		float y;
		float z;
		float xRot;
		float yRot;
		float zRot;
		vec3(float x, float y, float z);
		//TODO: add constructor for rotation
		vec3 operator+(vec3 &other);
		vec3();
	};

	
	static string intToStr(int Number) {
		int test = Number;
		stringstream ss;
		ss.clear();
		ss << test;
		return ss.str();
	}

}