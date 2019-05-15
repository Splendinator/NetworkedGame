#include "SnapshotManager.h"
#include "../Engine/Physics.h"
#include <thread>
#include <iostream>
#include "Messages.h"
#include "../Engine/Camera.h"
#include "../Engine/Math.h"

int SnapshotManager::getIndex(int serverTime) {
	int dif = _predictedTime - serverTime;
	
	if (dif > _numHistory) dif = _numHistory - 1;
	
	dif = _indexMod - dif;

	if (dif < 0) dif += _numHistory;

	return dif;
}

SnapshotManager::SnapshotManager()
{
	for (int i = 0; i < Networking::MAX_PLAYERS; ++i) _playerRot[i] = -1.f;
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

	//_predictedCounters = new int[numPlayers + numObjects] {};

}

void SnapshotManager::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	physx::PxRigidBody *a = (physx::PxRigidBody *)_physics->addOBB(pos, scale, rot, dynamic);
	if (dynamic) {
		_rbDynamics[id] = a;
	}
}


void SnapshotManager::addPlayer(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, int id)
{
	_rbPlayers[id] = (physx::PxRigidBody *)(_physics->addCapsule(pos, scale, rot, dynamic));
	_rbPlayers[id]->setMass(200000.f);
	_rbPlayers[id]->setLinearDamping(0.5f);
}

void SnapshotManager::threadFunc()
{
	while (!ready) {}
	for (;;) {

#pragma region PULL_DATA_FROM_SERVER_AFTER_WRONG_GUESS
		if (_wrong) {	//If we have guessed wrong
			_lastRealWorker = _lastRealMain;
			for (int i = 0; i < _numPlayers; ++i) {	//Update player information to most recent information
				auto pose = _rbPlayers[i]->getGlobalPose();
				auto &player = _predictedPlayers[_lastRealWorker * _numPlayers + i];

				pose.p.x = player.pos[0];
				pose.p.y = player.pos[1];
				pose.p.z = player.pos[2];
				
				_rbPlayers[i]->setGlobalPose(pose);
				
				_playerRot[i] = player.yaw;

			}
			for (int i = 0; i < _numObjects; ++i) {	//Update player information to most recent information
				auto pose = _rbDynamics[i]->getGlobalPose();
				auto &object = _predictedBodies[_lastRealWorker * _numObjects + i];

				pose.p.x = object.pos[0];
				pose.p.y = object.pos[1];
				pose.p.z = object.pos[2];
						   
				pose.q.x = object.rot.x;
				pose.q.y = object.rot.y;
				pose.q.z = object.rot.z;
				pose.q.w = object.rot.w;

				_rbDynamics[i]->setGlobalPose(pose);

				_rbDynamics[i]->setLinearVelocity({object.linVol[0],object.linVol[1] ,object.linVol[2] });
				_rbDynamics[i]->setAngularVelocity({ object.AngVol[0],object.AngVol[1] ,object.AngVol[2] });
			}
			_wrong = false;
		}
#pragma endregion

		if (_lastRealWorker != _indexMod) {

#pragma region UPRIGHT
			static Quatf upright = Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f);

			for (int i = 0; i < _numPlayers; ++i) {
				physx::PxTransform transform = _rbPlayers[i]->getGlobalPose();
				transform.q = { upright.x,upright.y,upright.z,upright.w };
				_rbPlayers[i]->setGlobalPose(transform);
				_rbPlayers[i]->setLinearVelocity({ 0,_rbPlayers[i]->getLinearVelocity().y,0 });
				_rbPlayers[i]->setAngularVelocity({ 0,0,0 });

			}
#pragma endregion

			for (int i = 0; i < _numPlayers; ++i) {
				Camera c;
				c.pitch = 0;
				if (_playerRot[i] >= 0.f) {
					c.yaw = _playerRot[i];
					Vec3f v = c.foward() * Networking::PLAYER_MOVE_SPEED;
					auto pose = _rbPlayers[i]->getGlobalPose();
					pose.p += { v[0], v[1], v[2] };
					_rbPlayers[i]->setGlobalPose(pose);

				}
			}

			_physics->update(Networking::PHYSICS_UPDATE_DELTA);






			//for (int i = 0; i < _numPlayers; ++i) {
			//	Camera c;
			//	c.pitch = 0;
			//	if (shared::getPlayer(i).yaw >= 0.f) {
			//		c.yaw = shared::getPlayer(i).yaw;
			//		Vec3f v = c.foward() * Networking::PLAYER_MOVE_SPEED;
			//		auto pose = shared::getPlayer(i).transform->getRigidActor()->getGlobalPose();
			//		pose.p += { v[0], v[1], v[2] };
			//		shared::getPlayer(i).transform->getRigidActor()->setGlobalPose(pose);
			//	}
			//}

			_lastRealWorker = increment(_lastRealWorker);

#pragma region PULL_DATA_FROM_SIIMULATION_AFTER_ESTIMATING
			for (int i = 0; i < _numPlayers; ++i) {	//Update player information to most recent information
				auto pose = _rbPlayers[i]->getGlobalPose();
				auto &player = _predictedPlayers[_lastRealWorker * _numPlayers + i];

				player.pos[0] = pose.p.x;
				player.pos[1] = pose.p.y;
				player.pos[2] = pose.p.z;

				player.yaw = _playerRot[i];

				//player.yaw = playerRot[d]

				//_playerRot[i] = player.yaw;
			}
			for (int i = 0; i < _numObjects; ++i) {	//Update player information to most recent information
				auto pose = _rbDynamics[i]->getGlobalPose();
				auto &object = _predictedBodies[_lastRealWorker * _numObjects + i];

				object.pos[0] = pose.p.x;
				object.pos[1] = pose.p.y;
				object.pos[2] = pose.p.z;

				object.rot.x = pose.q.x;
				object.rot.y = pose.q.y;
				object.rot.z = pose.q.z;
				object.rot.w = pose.q.w;

				object.linVol = { _rbDynamics[i]->getLinearVelocity().x, _rbDynamics[i]->getLinearVelocity().y, _rbDynamics[i]->getLinearVelocity().z };
				object.AngVol = { _rbDynamics[i]->getAngularVelocity().x, _rbDynamics[i]->getAngularVelocity().y, _rbDynamics[i]->getAngularVelocity().z};
			}
#pragma endregion
		}
		else {
		}
	}

	
}

bool closeTo(float a, float b, float leeway) {
	return ((a - leeway) < b) && ((a + leeway) > b);
}

void SnapshotManager::receiveSnapshotFromServer(char *data, int numPlayers, int numDynamics, int time) {
	
	bool changes = false;

	static int highestTime = time;

	if (time < highestTime || _wrong) return;
	
	int index = getIndex(time);

	
	int dataIndex = 0;
	int playerIndex = _numPlayers * index;
	int dynamicIndex = _numObjects * index;


	for (int i = 0; i < numPlayers; ++i) {
		auto p = (messages::PayloadPredictionOtherPositionNT *)&data[dataIndex];
		
		

		changes |= !(_predictedPlayers[playerIndex + p->id].pos.closeTo(p->pos, Networking::ESTIMATE_FLOAT_LEEWAY));
		changes |= !(_predictedPlayers[playerIndex + p->id].yaw == p->movementDir);
		//changes |= _playerRot[i] == p->movementDir;
		//std::cout << _predictedPlayers[playerIndex + p->id].yaw << '\n' << p->movementDir << "\n\n";

		//std::cout << (_predictedPlayers[playerIndex + p->id].pos.closeTo(p->pos, Networking::ESTIMATE_FLOAT_LEEWAY)) << " " << closeTo(_predictedPlayers[playerIndex + p->id].yaw,p->movementDir,Networking::ESTIMATE_FLOAT_LEEWAY) << " ";

		//std::cout << _predictedPlayers[playerIndex + p->id].pos[0] << "\t\t" << p->pos[0] << '\n';
		//std::cout << _predictedPlayers[playerIndex + p->id].pos[1] << "\t\t" << p->pos[1] << '\n';
		//std::cout << _predictedPlayers[playerIndex + p->id].pos[2] << "\t\t" << p->pos[2] << "\n\n";

		//std::cout << time << ": ";

		//std::cout << _predictedPlayers[playerIndex + p->id].yaw << '\t' << p->movementDir << '\t' << _playerRot[i] << '\n';

		_predictedPlayers[playerIndex + p->id].pos = p->pos;
		_predictedPlayers[playerIndex + p->id].yaw = p->movementDir;

		dataIndex += sizeof(messages::PayloadPredictionOtherPositionNT);
	}


	for (int i = 0; i < numDynamics; ++i) {
		auto p = (messages::PayloadPredictionDynamicPositionNT *)&data[dataIndex];

		changes |= !(_predictedBodies[dynamicIndex + p->id].AngVol.closeTo(p->angVel, Networking::ESTIMATE_FLOAT_LEEWAY));
		changes |= !(_predictedBodies[dynamicIndex + p->id].linVol.closeTo(p->linVel, Networking::ESTIMATE_FLOAT_LEEWAY));
		changes |= !(_predictedBodies[dynamicIndex + p->id].pos.closeTo(p->pos, Networking::ESTIMATE_FLOAT_LEEWAY));
		changes |= !(_predictedBodies[dynamicIndex + p->id].rot.closeTo(p->rot, Networking::ESTIMATE_FLOAT_LEEWAY));

		//std::cout << (_predictedBodies[dynamicIndex + p->id].AngVol.closeTo(p->angVel,Networking::ESTIMATE_FLOAT_LEEWAY)) << " " << (_predictedBodies[dynamicIndex + p->id].linVol.closeTo(p->linVel, Networking::ESTIMATE_FLOAT_LEEWAY)) << " " << (_predictedBodies[dynamicIndex + p->id].pos.closeTo(p->pos, Networking::ESTIMATE_FLOAT_LEEWAY))  << " " << (_predictedBodies[dynamicIndex + p->id].rot.closeTo(p->rot, Networking::ESTIMATE_FLOAT_LEEWAY)) << '\n';

		//std::cout << _predictedBodies[dynamicIndex + p->id].pos[0] << " " << p->pos[0] << '\n';

		(_predictedBodies[dynamicIndex + p->id].AngVol = p->angVel);
		(_predictedBodies[dynamicIndex + p->id].linVol = p->linVel);
		(_predictedBodies[dynamicIndex + p->id].pos = p->pos);
		(_predictedBodies[dynamicIndex + p->id].rot = p->rot);
	}
	

	


	if (changes) {
		_lastRealMain = index;
		std::cout << "Changes\n";
		_wrong = true;
	}
	else {
		std::cout << '\n';
	}
}
