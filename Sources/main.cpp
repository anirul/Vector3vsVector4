#include <array>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>

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
	std::array<float, N> x = { 0 };
	std::array<float, N> y = { 0 };
	std::array<float, N> z = { 0 };
};

// Dot product.
vec3 operator*(const vec3& v1, const vec3& v2) {
	vec3 ret;
	ret.x = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	ret.y = 0;
	ret.z = 0;
	return ret;
}

vec3 square_root(const vec3& v) {
	vec3 ret;
	ret.x = std::sqrt(v.x + v.y + v.z);
	ret.y = 0;
	ret.z = 1;
	return ret;
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
vec3SoA<N> square_root(const vec3SoA<N>& v) {
	vec3SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = std::sqrt(v.x[i] + v.y[i] + v.z[i]);
		ret.y[i] = 0;
		ret.z[i] = 1;
	}
	return ret;
}

// A simple vec4.
struct vec4 {
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
	std::array<float, N> x = { 0 };
	std::array<float, N> y = { 0 };
	std::array<float, N> z = { 0 };
	std::array<float, N> w = { 0 };
};

// Purposely simplified dot product (to be equivalent to the vec3).
vec4 operator*(const vec4& v1, const vec4& v2) {
	vec4 ret;
	ret.x = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	ret.y = 0;
	ret.z = 0;
	ret.w = 0;
	return ret;
}

vec4 square_root(const vec4& v) {
	vec4 ret;
	ret.x = std::sqrt(v.x + v.y + v.z);
	ret.y = 0;
	ret.z = 0;
	ret.w = 1;
	return ret;
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
vec4SoA<N> square_root(const vec4SoA<N>& v) {
	vec4SoA<N> ret;
	for (int i = 0; i < N; ++i) {
		ret.x[i] = std::sqrt(v.x[i] + v.y[i] + v.z[i]);
		ret.y[i] = 0;
		ret.z[i] = 0;
		ret.w[i] = 1;
	}
	return ret;
}

template <size_t N>
class RandomFill {
public:
	RandomFill() {
		generator_ = std::mt19937(device_());
		distribution_ =	std::uniform_real_distribution<>(-100.0, 100.0);
	}

	template <typename T>
	void Set(T& v) {
		SetValue(v.x);
		SetValue(v.y);
		SetValue(v.z);
	}

private:
	void SetValue(float& v) {
		v = (float)distribution_(generator_);
	}

	void SetValue(std::array<float, N>& v) {
		std::for_each(v.begin(), v.end(), [this](float& f) {
			f = (float)distribution_(generator_);
		});
	}

private:
	std::random_device device_;
	std::mt19937 generator_;
	std::uniform_real_distribution<> distribution_;
};

namespace {
	constexpr size_t small_value = 1024;
	RandomFill<8> random_fill;
}

template <typename T, size_t N = 1, typename I>
double Check(I& begin, I& end) {
	std::vector<double> result_vector;
	result_vector.resize(small_value);
	std::for_each(begin, end, [](T& val) {
		random_fill.Set<T>(val);
	});
	for (int i = 0; i < small_value; ++i) {
		auto before = std::chrono::high_resolution_clock::now();
		std::for_each(begin, end, [](T& val) {
			val = T{ square_root(val * val) };
		});
		auto after = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta_time = after - before;
		result_vector[i] = delta_time.count();
	}
	return *std::min_element(result_vector.begin(), result_vector.end());
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
		std::array<vec3, small_value> vec3_array;
		std::cout
			<< "time spend (array<vec3>)     : "
			<< Check<vec3>(vec3_array.begin(), vec3_array.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3) * small_value
			<< std::endl;
	}
	{
		std::array<vec4, small_value> vec4_array;
		std::cout
			<< "time spend (array<vec4>)     : "
			<< Check<vec4>(vec4_array.begin(), vec4_array.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4) * small_value
			<< std::endl;
	}
	{
		std::array<vec3SoA<8>, small_value / 8> vec3SoA_array;
		std::cout
			<< "time spend (array<vec3SoA>)  : "
			<< Check<vec3SoA<8>, 8>(vec3SoA_array.begin(), vec3SoA_array.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3SoA<8>) * small_value / 8
			<< std::endl;
	}
	{
		std::array<vec4SoA<8>, small_value / 8> vec4SoA_array;
		std::cout
			<< "time spend (array<vec4SoA>)  : "
			<< Check<vec4SoA<8>, 8>(vec4SoA_array.begin(), vec4SoA_array.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4SoA<8>) * small_value / 8
			<< std::endl;
	}
	// I finally try with vectors.
	constexpr size_t big_value = 1024 * 1024;
	{
		std::vector<vec3>  vec3_vector;
		vec3_vector.resize(big_value);
		std::cout
			<< "time spend (vector<vec3>)    : "
			<< Check<vec3>(vec3_vector.begin(), vec3_vector.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec3) * big_value
			<< std::endl;
	}
	{
		std::vector<vec4> vec4_vector;
		vec4_vector.resize(big_value);
		std::cout
			<< "time spend (vector<vec4>)    : "
			<< Check<vec4>(vec4_vector.begin(), vec4_vector.end())
			<< std::endl;
		std::cout
			<< "total space used             : "
			<< sizeof(vec4) * big_value
			<< std::endl;
	}
	{
		std::vector<vec3SoA<8>> vec3SoA_vector;
		vec3SoA_vector.resize(big_value / 8);
		std::cout
			<< "time spend (vector<vec3SoA>) : "
			<< Check<vec3SoA<8>, 8>(
				vec3SoA_vector.begin(),
				vec3SoA_vector.end())
			<< std::endl;
		std::cout
			<< "total spend used             : "
			<< sizeof(vec3SoA<8>) * big_value / 8
			<< std::endl;
	}
	{
		std::vector<vec4SoA<8>> vec4SoA_vector;
		vec4SoA_vector.resize(big_value / 8);
		std::cout
			<< "time spend (vector<vec4SoA>) : "
			<< Check<vec4SoA<8>, 8>(
				vec4SoA_vector.begin(), 
				vec4SoA_vector.end())
			<< std::endl;
		std::cout
			<< "total spend used             : "
			<< sizeof(vec4SoA<8>) * big_value / 8
			<< std::endl;
	}
	return 0;
}
