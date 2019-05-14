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

	void estimatePlayer(Vec3f pos, float yaw, int playerId, int serverTime);

	//void setBodyPrediction(int numObject, int )

	void incrementTime() { ++_predictedTime; (++_indexMod) %= _numHistory; }

	void setPredictedTime(int time) { _predictedTime = time; };

	volatile bool ready = false;
private:

	int getIndex(int serverTime);

	Physics *_physics;
	std::thread *_worker;
	
	unsigned int _predictedTime = 0;

	struct DynamicEntry {
		Vec3f pos;
		Vec3f scale;
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
	int *_predictedCounters;

	std::vector<physx::PxRigidActor *>_rbDynamics;
	std::vector<physx::PxRigidActor *>_rbPlayers;

	int _indexMod = 0;

	

	//float _physicsDelta = 0.1f;

	int _numObjects, _numPlayers, _numHistory;

	void threadFunc();
};

