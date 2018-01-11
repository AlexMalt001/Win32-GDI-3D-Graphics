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
			float getRadsExact() const;
			float getRads() const;
			float getDegs() const;
			void setRads(float angle);
			void setRadsExact(float angle);
			void setDegs(float angle);
			Angle();
			Angle(bool inRads, float value);
			Angle operator=(const Angle &other);
		private:
			bool inRads = true;
			float angleDegs = 0;
			float angleRads = 0; //done between 0-2, not between 0-2PI, cause its nicer
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