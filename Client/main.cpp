#include <iostream>
#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h"
#include "ManagerClient.h"
#include "Messages.h"
#include "Shared.h"

using namespace domnet;

Engine e;
ManagerClient manager;

static const char *IP_ADDRESS = "127.0.0.1";


void networkSetup() {
	
	
	Address a;
	a.hostname = IP_ADDRESS;
	

	manager.connect(a);
}


void engineSetup() {

	
	e.init();
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

void engineLoop() {
  
		e.getCamera()->pos = shared::getCurrPlayer()->_pos;
		
		messages::messageRef<messages::PayloadKeyPress>().payload.input = 0;

		if (e.isHeld('D'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1;
			//Vec3f right = e.getCamera()->right() * 0.04f;
			//physx::PxTransform t = shared::getCurrPlayer()->getRigidActor()->getGlobalPose();
			//t.p += {right[0], right[1], right[2]};
			//shared::getCurrPlayer()->getRigidBody()->setGlobalPose(t);
		
		}
		if (e.isHeld('A'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b10;
			//Vec3f right = e.getCamera()->right() * -0.04f;
			//physx::PxTransform t = shared::getCurrPlayer()->getRigidActor()->getGlobalPose();
			//t.p += {right[0], right[1], right[2]};
			//shared::getCurrPlayer()->getRigidBody()->setGlobalPose(t);
		}
		if (e.isHeld('W'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b100;
			//Vec3f right = e.getCamera()->up();
			//right[1] = 0;
			//right.normalize();
			//right = right * 0.04f;
			//physx::PxTransform t = shared::getCurrPlayer()->getRigidActor()->getGlobalPose();
			//t.p += {right[0], right[1], right[2]};
			//shared::getCurrPlayer()->getRigidBody()->setGlobalPose(t);
		}
		if (e.isHeld('S'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1000;
			//Vec3f right = e.getCamera()->up();
			//right[1] = 0;
			//right.normalize();
			//right = right * -0.04f;
			//physx::PxTransform t = shared::getCurrPlayer()->getRigidActor()->getGlobalPose();
			//t.p += {right[0], right[1], right[2]};
			//shared::getCurrPlayer()->getRigidBody()->setGlobalPose(t);
		}
		
		
		e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
		e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
		if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
		if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
		
		messages::messageRef<messages::PayloadKeyPress>().payload.rot = e.getCamera()->yaw;
		
		manager.send(&messages::messageRef<messages::PayloadKeyPress>());

		shared::setPlayersUpright();
		
		e.update(0.004f);
}

int main() {

	initListeners();

	engineSetup();
	networkSetup();

	while (!shared::getCurrPlayer()) {
		manager.update();
	}


	for (;;) {
		manager.update();
		engineLoop();
	}


	//getchar();
	return 0;
}