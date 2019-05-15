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

	void receiveSnapshotFromServer(char *data, int numPlayers, int numDynamics, int time);

	//void setBodyPrediction(int numObject, int )

	void incrementTime() { ++_predictedTime; (++_indexMod) %= _numHistory; }

	void setPredictedTime(int time) { _predictedTime = time; };

	volatile bool ready = false;

	void syncDynamic(physx::PxRigidActor *a, int id) {
		auto pose = a->getGlobalPose();
			
		pose.p.x = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].pos[0];
		pose.p.y = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].pos[1];
		pose.p.z = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].pos[2];

		pose.q.x = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].rot.x;
		pose.q.y = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].rot.y;
		pose.q.z = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].rot.z;
		pose.q.w = _predictedBodies[(_lastRealWorker - 1) * _numObjects + id].rot.w;

		a->setGlobalPose(pose);
	}

	void syncPlayer(physx::PxRigidActor *a, int id) {
		auto pose = a->getGlobalPose();

		pose.p.x = _predictedPlayers[(_lastRealWorker - 1) * _numPlayers + id].pos[0];
		pose.p.y = _predictedPlayers[(_lastRealWorker - 1) * _numPlayers + id].pos[1];
		pose.p.z = _predictedPlayers[(_lastRealWorker - 1) * _numPlayers + id].pos[2];

		a->setGlobalPose(pose);

	}

private:

	int getIndex(int serverTime);
	int increment(int x) { return (++x) % _numHistory; }
	int decrement(int x) {
		--x;
		if (x < 0) x += _numHistory;
		return x;
	}

	Physics *_physics;
	std::thread *_worker;
	
	unsigned int _predictedTime = 0;

	struct DynamicEntry {
		Vec3f pos;
		Quatf rot;
		Vec3f linVol;
		Vec3f AngVol;
	};
	struct PlayerEntry {
		Vec3f pos;
		float yaw;
	};


	DynamicEntry *_predictedBodies;
	PlayerEntry *_predictedPlayers;
	
	

	float _playerRot[Networking::MAX_PLAYERS];
	physx::PxRigidBody * _rbPlayers[Networking::MAX_PLAYERS];
	physx::PxRigidBody * _rbDynamics[Networking::MAX_OBJECTS];

	int _indexMod = 0;

	int _lastRealMain = 0;
	int _lastRealWorker = 0;

	volatile bool _wrong = false;

	//float _physicsDelta = 0.1f;

	int _numObjects, _numPlayers, _numHistory;

	void threadFunc();
};

