#include "Quaternion.h"
#include "Transform.h"

struct Engine {
	void init();
	void update(float delta);
	Transform *addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true);

	~Engine();
};