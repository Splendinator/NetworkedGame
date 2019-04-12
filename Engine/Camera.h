#pragma once

#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	~Camera();

	mat4f buildViewMatrix();

private:
	float yaw;
	float pitch;
	vec3f pos;
};

