#include "Camera.h"
#include "Math.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

mat4f Camera::buildViewMatrix()
{
	return 
		Math::buildRotateMatrixX(pitch) * Math::buildRotateMatrixY(yaw) * Math::buildTranslateMatrix(pos);
}
