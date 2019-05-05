#include "Shared.h"
#include "../Engine/Quaternion.h"
#include "../Engine/Math.h"
#include "../Engine/Transform.h"

using namespace shared;

namespace shared {

	const int NUM_PLAYERS = 32;
	const int NUM_OBJECTS = 2048;

	Transform *players[NUM_PLAYERS] = {};

	Transform *dynamicObjects[NUM_OBJECTS] = {};

	Transform *player = nullptr;

}

Transform *shared::getDynamic(int id)
{
	return dynamicObjects[id];
}

Transform *shared::getPlayer(int id)
{
	return players[id];
}

void shared::setDynamic(Transform *t, int id)
{
	dynamicObjects[id] = t;
}

void shared::setPlayer(Transform *t, int id)
{
	players[id] = t;
}

Transform * shared::getCurrPlayer()
{
	return player;
}

void shared::setCurrPlayer(Transform * t)
{
	player = t;
}


void shared::setPlayersUpright()
{
	Quatf upright = Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f);
	
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		if (players[i]) {
			physx::PxTransform transform = players[i]->getRigidActor()->getGlobalPose();
			transform.q = { upright.x,upright.y,upright.z,upright.w };
			players[i]->getRigidBody()->setGlobalPose(transform);
		}
	}
	
	if (player) {
		physx::PxTransform transform = player->getRigidActor()->getGlobalPose();
		transform.q = { upright.x,upright.y,upright.z,upright.w };
		player->getRigidBody()->setGlobalPose(transform);
	}
}
