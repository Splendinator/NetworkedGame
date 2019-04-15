#include "Quaternion.h"
#include <math.h>
#include "Math.h"


template<class T>
Quaternion<T>::Quaternion()
{
	static_assert(std::is_floating_point<T>::value);
}


template<class T>
Quaternion<T>::~Quaternion()
{
}

template<class T>
Quaternion<T> Quaternion<T>::quatFromEuler(Vector<T, 3> around, float rad)
{
	Quaternion<T> q;

	q.x = around[0] * static_cast<T>(sin(rad / 2));
	q.y = around[1] * static_cast<T>(sin(rad / 2));
	q.z = around[2] * static_cast<T>(sin(rad / 2));
	q.w = static_cast<T>(cos(rad / 2));

	return q;
}

template<class T>
Vector<T, 3> Quaternion<T>::eulerFromQuat(Quaternion<T> q)
{

	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	T roll = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	T pitch;
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		pitch = copysign(Math::PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	T yaw = atan2(siny_cosp, cosy_cosp);

	return { roll,pitch,yaw };
}


template class Quaternion<float>;

