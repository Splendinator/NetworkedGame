#pragma once

#include "Matrix.h"
#include <type_traits>

template <class T>
class Quaternion
{
public:
	
	Quaternion();
	~Quaternion();

	static Quaternion<T> quatFromEuler(Vector<T, 3> around, float rad);
	static Vector<T,3> eulerFromQuat(Quaternion<T>);

	T x, y, z, w;
	
};

typedef Quaternion<float> quatf;