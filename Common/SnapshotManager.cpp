#include "SnapshotManager.h"
#include "../Engine/Physics.h"
#include <thread>
#include <iostream>

int SnapshotManager::getIndex(int serverTime) {
	int dif = _predictedTime - serverTime;
	//if (dif < 0) dif = 0;
	//if (dif >= _numHistory) dif = _numHistory - 1;
	std::cout << dif << std::endl;
	return dif;
}

SnapshotManager::SnapshotManager()
{
	_physics = new Physics();
	_physics->initialize();
	_worker = new std::thread(&SnapshotManager::threadFunc,this);
	_worker->detach();
}


SnapshotManager::~SnapshotManager()
{
	_physics->cleanUp();
	delete _physics;
	delete _worker;
	delete _predictedBodies;
	delete _predictedPlayers;
}

void SnapshotManager::initBuffers(int numObjects, int numPlayers, int numHistory)
{
	_numObjects = numObjects;
	_numPlayers = numPlayers;
	_numHistory = numHistory;

	_predictedPlayers = new PlayerEntry[numPlayers * numHistory];
	_predictedBodies = new DynamicEntry[numObjects * numHistory];
	_predictedCounters = new int[numPlayers + numObjects] {};

}

void SnapshotManager::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	physx::PxRigidActor *a = _physics->addOBB(pos, scale, rot, dynamic);
	if (dynamic) {
		_rbDynamics.push_back(a);
	}
}

void SnapshotManager::estimatePlayer(Vec3f pos, float yaw, int playerId, int serverTime) {
	int dif = getIndex(serverTime);
	

	_predictedPlayers[dif * _numPlayers] = { pos,yaw };
}

void SnapshotManager::addPlayer(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	_rbPlayers.push_back(_physics->addCapsule(pos, scale, rot, dynamic));
}

void SnapshotManager::threadFunc()
{
	while (!ready) {}
	for (;;) {
		
	}
}
