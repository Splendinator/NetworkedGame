#pragma once
#include "../Engine/Quaternion.h"


class Engine;
class ManagerServer;
class Transform;

namespace Level {
	void init(Engine *engine, ManagerServer *manager);
	void loadCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic = true, bool visible = true);
	void loadPlayer(int playerId, Vec3f pos);
	void finish();

	int getNumDynamics();
	int getNumPlayers();
	int getNumObjects();

}