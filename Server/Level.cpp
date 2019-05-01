#include "Level.h"
#include "../Engine/Engine.h"
#include "ManagerServer.h"
#include "Messages.h"
#include "../Engine/Math.h"
#include "Shared.h"

#include <iostream>

Engine *e;
ManagerServer *s;

int dynamicIndex = 0;

namespace Level {

	void init(Engine *engine, ManagerServer *server) {
		e = engine;
		s = server;
	}

	void loadCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, bool visible) {
		shared::setDynamic(e->addCube(pos, scale, rot, dynamic, visible), dynamicIndex);

		auto &m = messages::messageRef<messages::PayloadLoadLevelCube>().payload;

		m.cubeId = dynamicIndex++;
		m.pos = pos;
		m.scale = scale;
		m.rot = rot;
		m.dynamic = dynamic;
		m.visible = visible;

		s->broadcast(&messages::messageRef<messages::PayloadLoadLevelCube>());
		
	}

	void loadPlayer(int playerId, Vec3f pos)
	{
		shared::setPlayer(e->addCapsule(pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true), playerId);
		shared::getPlayer(playerId)->getRigidBody()->setMass(200000.f);
		shared::getPlayer(playerId)->getRigidBody()->setLinearDamping(0.5f);

		//Player
		messages::messageRef<messages::PayloadLoadLevelPlayer>().payload.pos = pos;

		//OtherPlayer
		messages::messageRef<messages::PayloadLoadLevelOther>().payload.playerId = playerId;
		messages::messageRef<messages::PayloadLoadLevelOther>().payload.pos = pos;

		for (int i = 0; i < s->getNumPlayers(); ++i) {
			if (i == playerId)
				s->send(&messages::messageRef<messages::PayloadLoadLevelPlayer>(), i, true);
			else {
				s->send(&messages::messageRef<messages::PayloadLoadLevelOther>(), i, true);
			}
		}
	}

	//Transform * getPlayer(int id)
	//{
	//	return playerTransform[id];
	//}
	//
	//Transform * getDynamic(int id)
	//{
	//	return dynamicTransform[id];
	//}


}