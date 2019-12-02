#include <array>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>
	
// A simple vec3.
struct vec3 {
	vec3(): x(0), y(0), z(0) {}
	vec3(float a, float b, float c): x(a), y(b), z(c) {}
	float x = 0;
	float y = 0;
	float z = 0;
};

// Dot product.
float operator*(const vec3& v1, const vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// A simple vec4.
struct vec4 {
	vec4(): x(0), y(0), z(0), w(0) {}
	vec4(float a, float b, float c, float d): x(a), y(b), z(c), w(d) {}
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

// Purposely simplified dot product (to be equivalent to the vec3).
float operator*(const vec4& v1, const vec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// This is there to test if vec3 is faster than vec4.
int main(int ac, char** av) {
	// First I try with local arrays.
	{
		constexpr int big_value = 1024;
		// Local array with vec3.
		{
			std::array<vec3, big_value> vec3_array = {};
			auto before = std::chrono::high_resolution_clock::now();
			std::for_each(vec3_array.begin(), vec3_array.end(), [](vec3& val) {
				val = vec3{ std::sqrt(val * val), 0, 1 };
			});
			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> delta_time = after - before;
			std::cout 
				<< "time spend (array<vec3>)  : " 
				<< delta_time.count() 
				<< std::endl;
			std::cout 
				<< "total space used          : " 
				<< sizeof(vec3) * big_value 
				<< std::endl;
		}
		// Local array with vec4.
		{
			std::array<vec4, big_value> vec4_array = {};
			auto before = std::chrono::high_resolution_clock::now();
			std::for_each(vec4_array.begin(), vec4_array.end(), [](vec4& val) {
				val = vec4{ std::sqrt(val * val), 0, 0, 1 };
			});
			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> delta_time = after - before;
			std::cout 
				<< "time spend (array<vec4>)  : " 
				<< delta_time.count() 
				<< std::endl;
			std::cout
				<< "total space used          : "
				<< sizeof(vec4) * big_value
				<< std::endl;
		}
	}
	{
		constexpr int big_value = 1024 * 1024;
		// Heap allocated array with vec3.
		{
			std::vector<vec3> vec3_array{ big_value, {0, 0, 0} };
			auto before = std::chrono::high_resolution_clock::now();
			std::for_each(vec3_array.begin(), vec3_array.end(), [](vec3& val) {
				val = vec3{ std::sqrt(val * val), 0, 1 };
			});
			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> delta_time = after - before;
			std::cout 
				<< "time spend (vector<vec3>) : " 
				<< delta_time.count() 
				<< std::endl;
			std::cout
				<< "total space used          : "
				<< sizeof(vec3) * big_value
				<< std::endl;

		}
		// Heap allocated array with vec4.
		{
			std::vector<vec4> vec4_array{ big_value, {0, 0, 0, 0} };
			auto before = std::chrono::high_resolution_clock::now();
			std::for_each(vec4_array.begin(), vec4_array.end(), [](vec4& val) {
				val = vec4{ std::sqrt(val * val), 0, 0, 1 };
			});
			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> delta_time = after - before;
			std::cout 
				<< "time spend (vector<vec4>) : " 
				<< delta_time.count() 
				<< std::endl;
			std::cout
				<< "total space used          : "
				<< sizeof(vec4) * big_value
				<< std::endl;
		}
	}
	return 0;
}
