#include "Camera.h"
#include "Math.h"
#include <math.h>


Camera::Camera()
{
}


Camera::~Camera()
{
}

Mat4f Camera::buildViewMatrix()
{
	return
		Math::buildRotateMatrixX(-pitch)
		*
		Math::buildRotateMatrixY(yaw)
		*
		Math::buildTranslateMatrix(-pos)
		;
}

Vec3f Camera::foward()
{
	float f = cos(pitch);
	return Vec3f({ float(sin(yaw)) * f, float(sin(pitch)), float(-cos(yaw)) * f }).normalize();
}

Vec3f Camera::right()
{
	return Vec3f({float(cos(yaw)), 0,float(sin(yaw)) }).normalize();
}

Vec3f Camera::up()
{
	float f = sin(pitch);
	return Vec3f({ float(-sin(yaw)) * f, float(cos(pitch)), float(cos(yaw)) * f }).normalize();
}
