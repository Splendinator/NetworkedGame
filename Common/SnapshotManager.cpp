#include "SnapshotManager.h"
#include "../Engine/Physics.h"
#include <thread>
#include <iostream>

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
}

void SnapshotManager::initBuffers(int numObjects, int numPlayers, int numHistory)
{
	_numObjects = numObjects;
	_numPlayers = numPlayers;
	_numHistory = numHistory;

}

void SnapshotManager::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	physx::PxRigidActor *a = _physics->addOBB(pos, scale, rot, dynamic);
	if (dynamic) {
		//_predictedBodies[id] = a;
	}
}

void SnapshotManager::addPlayer(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	//_predictedPlayers[id] = _physics->addCapsule(pos, scale, rot, dynamic);
}

void SnapshotManager::threadFunc()
{
	while (!ready) {}
	for (;;) {
		//Networking::PHYSICS_UPDATE_DELTA;
		//_physics->update(Networking::Physi);
	}
}
