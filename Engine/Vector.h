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

	Vector<T, N> operator* (T other);
	const Vector<T, N> &operator+= (const Vector<T,N> &other);
	const Vector<T, N> &operator-= (const Vector<T, N> &other);
	
	Vector<T, N> operator- () const;

	Vector<T,N> &normalize();
private:
	T data[N];
};


typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;

