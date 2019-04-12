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

template class Vector<float, 2>;
template class Vector<float, 3>;
template class Vector<float, 4>;