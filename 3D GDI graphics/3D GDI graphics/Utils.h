#pragma once
#include <sstream>
#include<Windows.h>
#include <sstream>
#include <codecvt>
using namespace std;

namespace utils {
	
	template<class T>
	static void println(T output, true_type) { //numeric
		wstring x = (to_wstring(output)) + L"\n";
		OutputDebugStringW(x.c_str());
	}

	template<class T>
	static void println(T output, false_type) { //numeric
		println("no \'println\' function for this data type", false_type{});
		throw 0;
	}

	static void println (const string output, false_type) { //non-numeric

		std::wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		wstring finalForm = converter.from_bytes(output);
		OutputDebugStringW(finalForm.c_str());
	}

	static void println(wstring output, false_type) { //non-numeric
		OutputDebugStringW(output.c_str());
	}

	template<class T>
	static void println(T output) {
		println(output, std::is_arithmetic<T>{});
	}


	class Angle {
		public:
			float getRadsExact() const; //get real radians, ie angleRads*PI
			float getRads() const; //returns angleRads, ie radians/PI
			float getDegs() const; //returns angle in degrees
			void setRads(float angle); //sets angleRads and updates angleDegs. Input is in radians/PI
			void setRadsExact(float angle); //sets angleRads and updates angleDegs. Input is in normal radians
			void setDegs(float angle); //sets angleDegs and updates angleRads
			Angle();
			Angle(bool inRads, float value);
			Angle operator=(const Angle &other); 
		private:
			bool inRads = true; //was the angle last set to radians or degrees?
			float angleDegs = 0; //stores angle in degrees
			float angleRads = 0; //angle stored as radians/PI, as it is easier to tell angle size
	};

	class Vec3 {
	public:
		Vec3(float x, float y, float z) : x(x),
		                                  y(y),
		                                  z(z) {}

		float x = 0;
		float y = 0;
		float z = 0;
		float xRot = 0;
		float yRot = 0;
		float zRot = 0;
		//TODO: add constructor for rotation
		Vec3() = default;
	};

	
	static string intToStr(int number) {
		int test = number;
		stringstream ss;
		ss.clear();
		ss << test;
		return ss.str();
	}

	static float overflowConstrain(float currentVal, float increment, float constrain) { //constrain a value to 
		float val = currentVal+increment;
		if (val > constrain) {
			val = val - constrain;
			val = overflowConstrain(val, 0, constrain);

		}
		return val;
	}

}