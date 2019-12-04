#include <array>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

// A simple vec3.
struct vec3 {
	vec3() : x(0), y(0), z(0) {}
	vec3(float a) : x(a), y(0), z(0) {}
	vec3(float a, float b, float c) : x(a), y(b), z(c) {}
	float x = 0;
	float y = 0;
	float z = 0;
};

template <size_t N>
struct vec3SoA {
	float x[N] = { 0 };
	float y[N] = { 0 };
	float z[N] = { 0 };
};

// Dot product.
float operator*(const vec3& v1, const vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <size_t N>
vec3SoA<N> operator*(const vec3SoA<N>& v1, const vec3SoA<N>& v2) {
	vec3SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = v1.x[i] * v2.x[i];
		ret.y[i] = v1.y[i] * v2.y[i];
		ret.z[i] = v1.z[i] * v2.z[i];
	}
	return ret;
}

template <size_t N>
vec3SoA<N> sqrtSoA(const vec3SoA<N>& v) {
	vec3SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = std::sqrt(v.x[i] + v.y[i] + v.z[i]);
		ret.y[i] = 0;
		ret.z[i] = 1;
	}
	return ret;
}

// A simple vec4.
struct alignas(16) vec4 {
	vec4() : x(0), y(0), z(0), w(1) {}
	vec4(float a) : x(a), y(0), z(0), w(1) {}
	vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

template <size_t N>
struct vec4SoA {
	float x[N] = { 0 };
	float y[N] = { 0 };
	float z[N] = { 0 };
	float w[N] = { 0 };
};

// Purposely simplified dot product (to be equivalent to the vec3).
float operator*(const vec4& v1, const vec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <size_t N>
vec4SoA<N> operator*(const vec4SoA<N>& v1, const vec4SoA<N>& v2) {
	vec4SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = v1.x[i] * v2.x[i];
		ret.y[i] = v1.y[i] * v2.y[i];
		ret.z[i] = v1.z[i] * v2.z[i];
	}
	return ret;
}

template <size_t N>
vec4SoA<N> sqrtSoA(const vec4SoA<N>& v) {
	vec4SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = std::sqrt(v.x[i] + v.y[i] + v.z[i]);
		ret.y[i] = 0;
		ret.z[i] = 1;
	}
	return ret;
}

constexpr size_t small_value = 1024;

template <typename T>
double CheckArray() {
	std::vector<double> result_array;
	result_array.resize(small_value);
	for (int i = 0; i < small_value; ++i)
	{
		std::array<T, small_value> T_array = {};
		auto before = std::chrono::high_resolution_clock::now();
		std::for_each(T_array.begin(), T_array.end(), [](T& val) {
			val = T{ std::sqrt(val * val) };
		});
		auto after = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta_time = after - before;
		result_array[i] = delta_time.count();
	}
	return *std::min_element(result_array.begin(), result_array.end());
}

template <typename T, size_t N>
double CheckArrayAoSoA() {
	std::vector<double> result_array;
	result_array.resize(small_value);
	for (int i = 0; i < small_value; ++i) {
		std::array<T, small_value / N> T_array = {};
		auto before = std::chrono::high_resolution_clock::now();
		std::for_each(T_array.begin(), T_array.end(), [](T& val) {
			val = T{ sqrtSoA(val * val) };
		});
		auto after = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta_time = after - before;
		result_array[i] = delta_time.count();
	}
	return *std::min_element(result_array.begin(), result_array.end());
}

template <typename T>
double CheckVector(const size_t big_value) {
	std::vector<double> result_array;
	result_array.resize(small_value);
	for (int i = 0; i < small_value; ++i)
	{
		std::vector<T> T_vector(big_value, T());
		auto before = std::chrono::high_resolution_clock::now();
		std::for_each(T_vector.begin(), T_vector.end(), [](T& val) {
			val = T{ std::sqrt(val * val) };
		});
		auto after = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta_time = after - before;
		result_array[i] = delta_time.count();
	}
	return *std::min_element(result_array.begin(), result_array.end());
}

template <typename T, size_t N>
double CheckVectorAoSoA(const size_t big_value) {
	std::vector<double> result_array;
	result_array.resize(small_value);
	for (int i = 0; i < small_value; ++i) {
		std::vector<T> T_vector;
		T_vector.resize(big_value / N);
		auto before = std::chrono::high_resolution_clock::now();
		std::for_each(T_vector.begin(), T_vector.end(), [](T& val) {
			val = sqrtSoA(val * val);
		});
		auto after = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta_time = after - before;
		result_array[i] = delta_time.count();
	}
	return *std::min_element(result_array.begin(), result_array.end());
}

// This is there to test if vec3 is faster than vec4.
int main(int ac, char** av) {
#ifdef WIN32
	std::cout
		<< "default alignment            : "
		<< __STDCPP_DEFAULT_NEW_ALIGNMENT__
		<< std::endl;
#endif
	// First I try with local arrays.
	{
		std::cout
			<< "time spend (array<vec3>)     : "
			<< CheckArray<vec3>()
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3) * small_value
			<< std::endl;
		std::cout
			<< "time spend (array<vec4>)     : "
			<< CheckArray<vec4>()
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4) * small_value
			<< std::endl;
	}
	{
		std::cout
			<< "time spend (array<vec3SoA>)  : "
			<< CheckArrayAoSoA<vec3SoA<8>, 8>()
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3SoA<8>) * small_value / 8
			<< std::endl;
		std::cout
			<< "time spend (array<vec4SoA>)  : "
			<< CheckArrayAoSoA<vec4SoA<8>, 8>()
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4SoA<8>) * small_value / 8
			<< std::endl;
	}
	// I finally try with vectors.
	{
		constexpr size_t big_value = 1024 * 1024;
		std::cout
			<< "time spend (vector<vec3>)    : "
			<< CheckVector<vec3>(big_value)
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3) * big_value
			<< std::endl;
		std::cout
			<< "time spend (vector<vec4>)    : "
			<< CheckVector<vec4>(big_value)
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4) * big_value
			<< std::endl;
	}
	{
		constexpr size_t big_value = 1024 * 1024;
		std::cout
			<< "time spend (vector<vec3SoA>) : "
			<< CheckVectorAoSoA<vec3SoA<8>, 8>(big_value)
			<< std::endl;
		std::cout
			<< "total spend used             : "
			<< sizeof(vec3SoA<8>) * big_value / 8
			<< std::endl;
		std::cout
			<< "time spend (vector<vec4SoA>) : "
			<< CheckVectorAoSoA<vec4SoA<8>, 8>(big_value)
			<< std::endl;
		std::cout
			<< "total spend used             : "
			<< sizeof(vec4SoA<8>) * big_value / 8
			<< std::endl;
	}
	return 0;
}
