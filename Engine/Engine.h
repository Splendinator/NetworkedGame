#include "Quaternion.h"
#include "Transform.h"
#include "Camera.h"

struct Engine {
	void init();
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