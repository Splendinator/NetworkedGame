#include "Level.h"
#include "../Engine/Engine.h"
#include "ManagerServer.h"
#include "Messages.h"

#include <iostream>

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

		auto m = Messages::messageRef<Messages::PayloadLoadLevelCube>();

		//std::cout << m.payload << '\n';

		//auto m = &Messages::m_LoadLevelCube().payload;
		//m->pos = pos;
		//m->scale = scale;
		//m->rot = rot;
		//m->dynamic = dynamic;
		//m->visible = visible;
		//
		//s->broadcast(&Messages::m_LoadLevelCube());
	}

	void loadPlayer(int playerId, Vec3f pos)
	{
		
		
		//auto p = &Messages::m_LoadLevelPlayer().payload;
		//auto o = &Messages::m_LoadLevelOther().payload;
		//
		//p->pos = pos;
		//
		//o->
		//
		//for (int i = 0; i < s->getNumPlayers(); ++i) {
		//
		//}
	}


}