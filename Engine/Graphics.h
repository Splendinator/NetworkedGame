#pragma once

#include "Quaternion.h"
#include "Camera.h"

class Transform;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void initialize();
	Transform *addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);
	void update();

	bool isHeld(int key);
	bool isPressed(int key);

	Camera *getCamera();

	
};

