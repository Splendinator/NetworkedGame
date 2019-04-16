#pragma once

#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	~Camera();

	Mat4f buildViewMatrix();
	
	float yaw = 0.0f;
	float pitch = 0.0f;
	Vec3f pos = { 0,0,0 };

private:

};

