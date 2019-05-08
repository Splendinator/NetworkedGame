#include "Shared.h"
#include "../Engine/Quaternion.h"
#include "../Engine/Math.h"
#include "../Engine/Transform.h"

using namespace shared;
using namespace Networking;

namespace shared {

	Player players[MAX_PLAYERS] = {};

	Transform *dynamicObjects[MAX_OBJECTS] = {};

	Player *player;

}

Transform *shared::getDynamic(int id)
{
	return dynamicObjects[id];
}

Player &shared::getPlayer(int id)
{
	return players[id];
}

void shared::setDynamic(Transform *t, int id)
{
	dynamicObjects[id] = t;
}

void shared::setPlayer(Transform *t, int id, float yaw)
{
	players[id] = Networking::Player({ t,yaw });
}

Player &shared::getCurrPlayer()
{
	return *player;
}

void shared::setCurrPlayer(int id)
{
	player = &players[id];
}


void shared::setPlayersUpright()
{
	Quatf upright = Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f);
	
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		if (players[i].transform) {
			physx::PxTransform transform = players[i].transform->getRigidActor()->getGlobalPose();
			transform.q = { upright.x,upright.y,upright.z,upright.w };
			players[i].transform->getRigidBody()->setGlobalPose(transform);
		}
	}
	
	//if (player.transform) {
	//	physx::PxTransform transform = player.transform->getRigidActor()->getGlobalPose();
	//	transform.q = { upright.x,upright.y,upright.z,upright.w };
	//	player.transform->getRigidBody()->setGlobalPose(transform);
	//}
}
