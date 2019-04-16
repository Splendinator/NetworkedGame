#pragma once

#include "Quaternion.h"

class Transform;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void initialize();
	Transform *addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);
	void update();

	
};

