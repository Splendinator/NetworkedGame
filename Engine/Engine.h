#include "Quaternion.h"
#include "Transform.h"
#include "Camera.h"

struct Engine {
	void init();
	void update(float delta);
	Transform *addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true);

	bool isHeld(int key);
	bool isPressed(int key);

	Camera *getCamera();

	~Engine();
};