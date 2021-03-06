#pragma once

#include "Vector.h"

template <class T, int N>
class Matrix
{
public:
	Matrix();
	~Matrix() {};

	//Gets the row.
	Vector<T, N> &operator[] (int n) { return data[n]; }
	const Vector<T, N> &operator[] (int n) const { return data[n]; }

	
	Matrix<T, N> operator* (const Matrix<T, N> &other) const;
	Vector<T, N> operator* (const Vector<T, N> &other) const;
	Matrix<T, N> &operator*= (const Matrix<T, N> &other);
	
	Matrix<T, N> &toIdentity();

private:
	Vector<T,N> data[N];
};

typedef Matrix<float, 4> Mat4f;
typedef Matrix<float, 3> Mat3f;




template<class T, int N>
inline Matrix<T, N>::Matrix()
{
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			data[i][j] = 0;
		}
	}
}

template<class T, int N>
inline Matrix<T, N>& Matrix<T, N>::toIdentity()
{
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			data[i][j] = float(j == i);
		}
	}
	return *this;
}


template<class T, int N>
inline Matrix<T, N> Matrix<T, N>::operator*(const Matrix<T, N>& other) const
{
	Matrix<T, N> m;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			for (int k = 0; k < N; ++k) {
				m[i][j] += data[i][k] * other[k][j];
			}
		}
	}

	return m;
}

template<class T, int N>
inline Vector<T, N> Matrix<T, N>::operator*(const Vector<T, N>& other) const
{
	Vector<T, N> v;

	for (int i = 0; i < N; ++i) { v[i] = 0; }

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			v[i] += data[i][j] * other[j];
		}
	}
	

	return v;
}

template<class T, int N>
inline Matrix<T, N>& Matrix<T, N>::operator*=(const Matrix<T, N>& other)
{
	return *this = *this * other;
}
