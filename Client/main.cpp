#include <iostream>
#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h"
#include "ManagerClient.h"
#include "Messages.h"
#include "Shared.h"
#include "Timer.h"

using namespace domnet;

Engine e;
ManagerClient manager(0.0f,0.f);

static const char *IP_ADDRESS = "127.0.0.1";

static const float NETWORK_UPDATE_DELTA = 1/32.f;
static const float PHYSICS_UPDATE_DELTA = 1/128.f;	

Timer networkTimer;
Timer deltaTimer;

void networkSetup() {
	
	
	Address a;
	a.hostname = IP_ADDRESS;
	

	manager.connect(a);
}


void engineSetup() {

	
	e.init(PHYSICS_UPDATE_DELTA);
}

void initListeners() {

	manager.addListener(messages::MT_LOAD_LEVEL_CUBE, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelCube> *)m;
		shared::setDynamic(e.addCube(p->payload.pos, p->payload.scale, p->payload.rot, p->payload.dynamic, p->payload.visible), p->payload.cubeId);
	});

	manager.addListener(messages::MT_LOAD_LEVEL_PLAYER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelPlayer> *)m;
		shared::setCurrPlayer(e.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, false));
		shared::getCurrPlayer()->getRigidBody()->setMass(200000.f);
		shared::getCurrPlayer()->getRigidBody()->setLinearDamping(0.5f);
	});

	manager.addListener(messages::MT_LOAD_LEVEL_OTHER, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadLoadLevelOther> *)m;
		shared::setPlayer(e.addCapsule(p->payload.pos, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true), p->payload.playerId);
		shared::getPlayer(p->payload.playerId)->getRigidBody()->setMass(200000.f);
		shared::getPlayer(p->payload.playerId)->getRigidBody()->setLinearDamping(0.5f);
	});

	manager.addListener(messages::MT_PLAYER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadPlayerPosition> *)m;
		shared::getCurrPlayer()->setPos(p->payload.pos);
	});

	manager.addListener(messages::MT_OTHER_PLAYER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadOtherPlayerPosition> *)m;
		shared::getPlayer(p->payload.id)->setPos(p->payload.pos);
	});
}

void engineLoop(float delta) {
  
		e.getCamera()->pos = shared::getCurrPlayer()->_pos;
		
		messages::messageRef<messages::PayloadKeyPress>().payload.input = 0;

		if (e.isHeld('D'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1;
			
		}
		if (e.isHeld('A'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b10;
			
		}
		if (e.isHeld('W'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b100;
			
		}
		if (e.isHeld('S'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1000;

		}
		
		
		e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
		e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
		if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
		if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
		
		messages::messageRef<messages::PayloadKeyPress>().payload.rot = e.getCamera()->yaw;
		
		

		shared::setPlayersUpright();
		
		e.update(delta);
}

void networkLoop() {
	manager.send(&messages::messageRef<messages::PayloadKeyPress>(), false);
}


int main() {


	float delta;

	initListeners();

	engineSetup();
	networkSetup();

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
		engineLoop(delta);

	}
	


	//getchar();
	return 0;
}