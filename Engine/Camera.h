#pragma once

#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	~Camera();

	mat4f buildViewMatrix();
	
	float yaw = 0.0f;
	float pitch = 0.0f;
	vec3f pos = { 0,0,0 };

private:

};

