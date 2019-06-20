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
	unsigned int getPredictedTime() { return _predictedTime; };

	volatile bool ready = false;

	void syncDynamic(physx::PxRigidActor *a, int id) {

		//if (_lastRealWorker != getIndex(_predictedTime)) { return; }

		auto pose = a->getGlobalPose();
			
		pose.p.x = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].pos[0];
		pose.p.y = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].pos[1];
		pose.p.z = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].pos[2];
									
		pose.q.x = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].rot.x;
		pose.q.y = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].rot.y;
		pose.q.z = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].rot.z;
		pose.q.w = _predictedBodies[decrement(_lastRealWorker) * _numObjects + id].rot.w;

		a->setGlobalPose(pose);
	}

	void syncPlayer(physx::PxRigidActor *a, int id) {
		
		//if (_lastRealWorker != getIndex(_predictedTime)) { return; }
		auto pose = a->getGlobalPose();

		physx::PxVec3 v = { _predictedPlayers[decrement(_lastRealWorker) * _numPlayers + id].pos[0],
			_predictedPlayers[decrement(_lastRealWorker) * _numPlayers + id].pos[1],
			_predictedPlayers[decrement(_lastRealWorker) * _numPlayers + id].pos[2]
		};


		//if ((v - pose.p).magnitude() > Networking::INTERP_MIN_DISE) {
		//
		//	v = (v - pose.p);
		//
		//	v *= Networking::INTERP_PCT;
		//
		//	pose.p += v;
		//}
		//else {
			pose.p = v;
		//}

		a->setGlobalPose(pose);

	}

	void setPlayerYaw(float yaw, int id) { _playerRot[id] = yaw;}
	void setCurrPlayerYaw(float yaw) { _myPlayerInputBuffer[_indexMod] = yaw; }

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
		float yVel;
	};


	DynamicEntry *_predictedBodies;
	PlayerEntry *_predictedPlayers;
	
	
	float *_myPlayerInputBuffer;

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

