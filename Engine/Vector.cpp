#include "Vector.h"
#include <cstdarg>
#include <iostream>

template<class T, int N>
inline Vector<T, N>::Vector(std::initializer_list<T> il)
{
	int i = 0;
	for (auto it = il.begin(); it != il.end(); ++it) {
		data[i++] = *it;
	}
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator*(T other)
{
	Vector<T, N> ret;
	for (int i = 0; i < N; ++i) {
		ret[i] = data[i] * other;
	}
	return ret;
}

template<class T, int N>
const Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& other)
{
	for (int i = 0; i < N; ++i) {
		data[i] += other[i];
	}
	return *this;
}

template<class T, int N>
bool Vector<T,N>::operator==(const Vector<T, N> &other) const {
	bool different = false;
	for (int i = 0; i < N; ++i) {
		different |= (data[0] != other.data[0]);
	}
	return !different;
}


template<class T, int N>
const Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& other)
{
	for (int i = 0; i < N; ++i) {
		data[i] -= other[i];
	}
	return *this;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator-() const
{
	Vector<T, N> ret;
	for (int i = 0; i < N; ++i) {
		ret[i] = -data[i];
	}
	return ret;
}

template<class T, int N>
Vector<T,N> &Vector<T, N>::normalize()
{
	T total = 0;
	for (int i = 0; i < N; ++i) {
		total += abs(data[i]);
	}
	for (int i = 0; i < N; ++i) {
		data[i] /= total;
	}
	return *this;
}

template<class T, int N>
bool Vector<T,N>::closeTo(const Vector<T, N> &other, float leeway) const
{
	bool different = false;
	for (int i = 0; i < N; ++i) {
		different |= !(((data[i] - leeway) < other[i]) && ((data[i] + leeway) > other[i]));
	}
	return !different;
}


template class Vector<float, 2>;
template class Vector<float, 3>;
template class Vector<float, 4>;