#include "Quaternion.h"
#include "Transform.h"
#include "Camera.h"

struct Engine {
	void init(float physicsDelta = 1/120.f);
	void update(float delta);
	Transform *addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true, bool draw = true);
	Transform *addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true, bool draw = true);

	bool isHeld(int key);
	bool isPressed(int key);

	int getMouseXDelta();
	int getMouseYDelta();

	Camera *getCamera();

	~Engine();
};