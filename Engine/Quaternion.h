#pragma once

#include "Matrix.h"
#include <type_traits>

template <class T>
class Quaternion
{
public:
	
	Quaternion();
	~Quaternion();

	Quaternion(T x, T y, T z, T w); 

	static Quaternion<T> quatFromEuler(Vector<T, 3> around, float rad);
	static Vector<T,3> eulerFromQuat(Quaternion<T>);

	bool closeTo(const Quaternion<T> &other, float leeway) const;

	bool operator== (const Quaternion<T> &other) const {
		return
			x == other.x &&
			y == other.y &&
			z == other.z &&
			w == other.w;
	};
	Mat4f toMat4();

	T x, y, z, w;
	
};

typedef Quaternion<float> Quatf;