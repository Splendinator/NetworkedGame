#include "Shared.h"


using namespace shared;

namespace shared {

	Transform *players[32];

	Transform *dynamicObjects[2048];

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
