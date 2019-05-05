#include "HostFramework.cpp"
#include "ManagerClient.h"
#include "Messages.h"


static float PACKET_DROP_PCT = 0.f;	//Chance of UDP packets to not be sent (1.0f = 100%)
static float LATENCY = 0.f;	//Latency in MS	

static const char *IP_ADDRESS = "127.0.0.1";

ManagerClient manager(PACKET_DROP_PCT, LATENCY);

void networkSetup() {
	Address a;
	a.hostname = IP_ADDRESS;
	manager.connect(a);
}

void initLevelLoading() {
	manager.addListener(messages::MT_LOAD_LEVEL_CUBE, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelCube> *)m;
		if (p->payload.dynamic) {
			shared::setDynamic(engine.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.visible), p->payload.cubeId);
		}
		else
		{
			engine.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.visible);
		}
	});

	manager.addListener(messages::MT_LOAD_LEVEL_PLAYER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelPlayer> *)m;
		shared::setCurrPlayer(engine.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, false));
		shared::getCurrPlayer()->getRigidBody()->setMass(200000.f);
		shared::getCurrPlayer()->getRigidBody()->setLinearDamping(0.5f);
	});

	manager.addListener(messages::MT_LOAD_LEVEL_OTHER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelOther> *)m;
		shared::setPlayer(engine.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true), p->payload.playerId);
		shared::getPlayer(p->payload.playerId)->getRigidBody()->setMass(200000.f);
		shared::getPlayer(p->payload.playerId)->getRigidBody()->setLinearDamping(0.5f);
	});
}

void baseLoop(float delta) {

	engine.getCamera()->yaw += engine.getMouseXDelta() / 200.f;
	engine.getCamera()->pitch += engine.getMouseYDelta() / 200.f;
	if (engine.getCamera()->pitch > 1.5f) engine.getCamera()->pitch = 1.5f;
	if (engine.getCamera()->pitch < -1.5f) engine.getCamera()->pitch = -1.5f;

	shared::setPlayersUpright();

	engine.update(delta);
}


void main() {

	float delta;

	initLevelLoading();
	preInit();
	engine.init(PHYSICS_UPDATE_DELTA);
	networkSetup();
	postInit();

	//Wait for player to be loaded into the world
	while (!shared::getCurrPlayer()) {
		manager.update();
	}

	networkTimer.resetTimer();
	deltaTimer.resetTimer();
	for (;;) {
		if (networkTimer.getDelta() > NETWORK_UPDATE_DELTA) {
			networkTimer -= NETWORK_UPDATE_DELTA;
			networkLoop();
			manager.update();
		}
		delta = deltaTimer.getDelta();
		deltaTimer.resetTimer();
		baseLoop(delta);
		engineLoop(delta);
	}

}