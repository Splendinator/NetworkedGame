#pragma once
#include "../Engine/Quaternion.h"
#include "PxRigidActor.h"
#include <vector>
#include "Definitions.h"

class Physics;
class Transform;
namespace std {
	class thread;
}

class SnapshotManager
{
public:
	SnapshotManager();
	~SnapshotManager();

	void initBuffers(int numObjects, int numPlayers, int numHistory);

	void addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id);
	void addPlayer(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id);

	void incrementTime() { ++_predictedTime; }

	void setPredictedTime(int time) { _predictedTime = time; };

	volatile bool ready = false;
private:

	Physics *_physics;
	std::thread *_worker;
	
	unsigned int _predictedTime = 0;

	physx::PxRigidActor *_predictedBodies;
	Networking::Player *_predictedPlayers;

	unsigned int _actualTime;

	bool _upToDate;

	//float _physicsDelta = 0.1f;

	int _numObjects, _numPlayers, _numHistory;

	void threadFunc();
};

