#pragma once
#include "../Engine/Quaternion.h"


class Engine;
class ManagerServer;

namespace Level {
	void init(Engine *engine, ManagerServer *manager);
	void loadCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true, bool visible = true);

}