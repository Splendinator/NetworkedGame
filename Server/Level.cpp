#include "Level.h"
#include "../Engine/Engine.h"
#include "ManagerServer.h"
#include "Messages.h"

Engine *e;
ManagerServer *s;

int dynamicIndex = 0;
Transform *dynamicTransform[1024];

int playerIndex = 0;
Transform *playerTransform[16];


namespace Level {

	void init(Engine *engine, ManagerServer *server) {
		e = engine;
		s = server;
	}

	void loadCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, bool visible) {
		dynamicTransform[dynamicIndex++] = e->addCube(pos, scale, rot, dynamic, visible);

	}


}