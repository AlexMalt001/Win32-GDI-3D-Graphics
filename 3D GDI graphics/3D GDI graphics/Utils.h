#pragma once
#include <sstream>
#include<Windows.h>
#include <typeinfo>
#include <atlstr.h>  
#include <sstream>
#include <codecvt>
using namespace std;

namespace utils {
	
	template<class T>
	static void println(T output, std::true_type) { //numeric
		wstring x = (to_wstring(output)) + L"\n";
		OutputDebugStringW(x.c_str());
	}

	template<class T>
	static void println(T output, std::false_type) { //numeric
		println("no \'println\' function for this data type");
		throw 0;
	}

	static void println (string output, false_type) { //non-numeric
		/*basic_string<wchar_t> test = output;
		OutputDebugStringW(test);*/
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