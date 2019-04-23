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
	Transform *addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);
	void update();

	bool isHeld(int key);
	bool isPressed(int key);

	int getMouseXDelta();
	int getMouseYDelta();

	void setOnWindowClose(void (*func)());

	Camera *getCamera();

	
};

