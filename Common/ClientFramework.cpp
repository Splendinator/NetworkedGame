#include "HostFramework.cpp"
#include "ManagerClient.h"
#include "Messages.h"
#include "SnapshotManager.h"


static float PACKET_DROP_PCT = 0.f;	//Chance of UDP packets to not be sent (1.0f = 100%)
static float LATENCY = 0.f;	//Latency in MS	

static const int SNAPSHOT_HISTORY = 30;

static const char *IP_ADDRESS = "127.0.0.1";

namespace readycheck {
	int numPlayers	 = -1;
	int numObjects	 = -1;
	int currPlayers	 = 0;
	int	currObjects = 0;
	
	int numDynamics = 0;

}

ManagerClient manager(PACKET_DROP_PCT, LATENCY);

SnapshotManager snapshotManager;

void networkSetup() {
	Address a;
	a.hostname = IP_ADDRESS;
	manager.connect(a);
}

void initLevelLoading() {

	manager.addListener(messages::MT_READY, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadReady> *)m;
		readycheck::numPlayers = p->payload.numPlayers;
		readycheck::numObjects = p->payload.numObjects;
		int time = std::chrono::system_clock::now().time_since_epoch().count() - p->payload.epoch;
		float ticks = (time / 10000000.f) / Networking::PHYSICS_UPDATE_DELTA;
		std::cout << "Server is " << time << " nanoseconds behind client, meaning " << int(ticks) << " ticks behind client\n";
		snapshotManager.setPredictedTime(int(ticks));
	});				 

	manager.addListener(messages::MT_LOAD_LEVEL_CUBE, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelCube> *)m;
		if (p->payload.dynamic) {
			shared::setDynamic(engine.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.visible), p->payload.cubeId);
			++readycheck::numDynamics;
		}
		else
		{
			engine.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.visible);
		}
		snapshotManager.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.cubeId);
		++readycheck::currObjects;
	});

	manager.addListener(messages::MT_LOAD_LEVEL_PLAYER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelPlayer> *)m;
		shared::setPlayer(engine.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, false), p->payload.playerId);
		shared::setCurrPlayer(p->payload.playerId);
		shared::getCurrPlayer().transform->getRigidBody()->setMass(200000.f);
		shared::getCurrPlayer().transform->getRigidBody()->setLinearDamping(0.5f);
		++readycheck::currPlayers;
		snapshotManager.addPlayer(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, p->payload.playerId);
	});

	manager.addListener(messages::MT_LOAD_LEVEL_OTHER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelOther> *)m;
		shared::setPlayer(engine.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true), p->payload.playerId);
		shared::getPlayer(p->payload.playerId).transform->getRigidBody()->setMass(200000.f);
		shared::getPlayer(p->payload.playerId).transform->getRigidBody()->setLinearDamping(0.5f);
		++readycheck::currPlayers;
		snapshotManager.addPlayer(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, p->payload.playerId);
	});
}

int baseLoop(float delta) {

	engine.getCamera()->yaw += engine.getMouseXDelta() / 200.f;
	engine.getCamera()->pitch += engine.getMouseYDelta() / 200.f;
	if (engine.getCamera()->pitch > 1.5f) engine.getCamera()->pitch = 1.5f;
	if (engine.getCamera()->pitch < -1.5f) engine.getCamera()->pitch = -1.5f;

	shared::setPlayersUpright();

	return engine.update(delta);
}


void main() {

	float delta;
	int runPhysics;

	initLevelLoading();
	preInit();
	engine.init(Networking::PHYSICS_UPDATE_DELTA);
	networkSetup();

	//Wait for client to load everything.
	while (readycheck::currPlayers != readycheck::numPlayers && readycheck::currObjects != readycheck::numObjects) {
		manager.update();	
	}
	snapshotManager.initBuffers(readycheck::numDynamics, readycheck::numPlayers, SNAPSHOT_HISTORY);

	postInit();

	snapshotManager.ready = true;

	networkTimer.resetTimer();
	deltaTimer.resetTimer();
	for (;;) {
		if (networkTimer.getDelta() > Networking::NETWORK_UPDATE_DELTA) {
			networkTimer -= Networking::NETWORK_UPDATE_DELTA;
			networkLoop();
			manager.update();
		}
		delta = deltaTimer.getDelta();
		deltaTimer.resetTimer();
		for (int i = baseLoop(delta); i > 0; --i) {
			physicsLoop();
		}
		engineLoop(delta);
	}

}