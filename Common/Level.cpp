#include "Level.h"
#include "../Engine/Engine.h"
#include "ManagerServer.h"
#include "Messages.h"
#include "../Engine/Math.h"
#include "Shared.h"
#include <chrono>

#include <iostream>

Engine *e;
ManagerServer *s;

int dynamicIndex = 0;
int numPlayers = 0;
int numObjects = 0;

namespace Level {

	void init(Engine *engine, ManagerServer *server) {
		e = engine;
		s = server;
	}

	void loadCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, bool visible) {
		auto &m = messages::messageRef<messages::PayloadLoadLevelCube>().payload;
		
		if (dynamic) {
			shared::setDynamic(e->addCube(pos, scale, rot, dynamic, visible), dynamicIndex);
			m.cubeId = dynamicIndex++;
		}
		else {
			m.cubeId = 0;
			e->addCube(pos, scale, rot, dynamic, visible);
		}

		m.pos = pos;
		m.scale = scale;
		m.rot = rot;
		m.dynamic = dynamic;
		m.visible = visible;

		s->broadcast(&messages::messageRef<messages::PayloadLoadLevelCube>());
		++numObjects;
		
	}

	void loadPlayer(int playerId, Vec3f pos)
	{
		shared::setPlayer(e->addCapsule(pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true), playerId);
		shared::getPlayer(playerId).transform->getRigidBody()->setMass(200000.f);
		shared::getPlayer(playerId).transform->getRigidBody()->setLinearDamping(0.5f);

		//Player
		messages::messageRef<messages::PayloadLoadLevelPlayer>().payload.playerId = playerId;
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
		++numPlayers;
	}

	void finish() {
		messages::messageRef<messages::PayloadReady>().payload.numObjects = numObjects;
		messages::messageRef<messages::PayloadReady>().payload.numPlayers = numPlayers;
		messages::messageRef<messages::PayloadReady>().payload.epoch = std::chrono::system_clock::now().time_since_epoch().count();
		s->broadcast(&messages::messageRef<messages::PayloadReady>());
	}

	int getNumDynamics()
	{
		return dynamicIndex;
	}

	int getNumPlayers() {
		return numPlayers;
	}

	int getNumObjects() {
		return numObjects;
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