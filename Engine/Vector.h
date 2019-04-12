#pragma once

#include <initializer_list>

template<class T, int N>
class Vector
{
public:

	Vector() {};
	Vector(std::initializer_list<T> il);
	
	~Vector() {};

	T &operator[](int n) { return data[n]; }
	const T &operator[](int n) const { return data[n]; }


private:
	T data[N];
};


typedef Vector<float, 2> vec2f;
typedef Vector<float, 3> vec3f;
typedef Vector<float, 4> vec4f;

