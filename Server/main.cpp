#include <iostream>

#include "Messages.h"
#include "ServerFramework.cpp"


static const float PLAYER_MOVE_SPEED = 0.15f;



void preInit() {

	manager.addListener(messages::MT_KEY_PRESS, [&](BaseMessage *bm, int i) {
		auto  m = (Message<messages::PayloadKeyPress> *)bm;
		auto p = shared::getPlayer(i);
		Camera c;
		c.yaw = m->payload.rot;
		c.pitch = 0;

		Vec3f v;
		if (m->payload.input & 0b1   ) { // D
			v = c.right() * PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
			pose.p += {v[0], v[1], v[2]};
			shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		}
		if (m->payload.input & 0b10  ) { // A
			v = -c.right() * PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
			pose.p += {v[0], v[1], v[2]};
			shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		}
		if (m->payload.input & 0b100 ) { // W
			v = c.foward() * PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
			pose.p += {v[0], v[1], v[2]};
			shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		}
		if (m->payload.input & 0b1000) { // S
			v = -c.foward() * PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
			pose.p += {v[0], v[1], v[2]};
			shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		}



	});

}

void postInit() {

	
	Level::loadCube({ 0.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ 0.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ -0.5,7,0.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ 4.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ -2.5,3,5.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ 6.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ -4.5,2,-2.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	
	Level::loadCube({ 0,-3,0 }, { 48,1,48 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		Level::loadPlayer(i, { float(5*i),15,0 });
	}
	

}



void networkLoop() {
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		messages::messageRef<messages::PayloadPlayerPosition>().payload.pos = shared::getPlayer(i)->_pos;
		messages::messageRef<messages::PayloadOtherPlayerPosition>().payload.pos = shared::getPlayer(i)->_pos;
		messages::messageRef<messages::PayloadOtherPlayerPosition>().payload.id = i;

		for (int j = 0; j < NUM_PLAYERS; ++j) {

			if (i == j) {
				manager.send(&messages::messageRef<messages::PayloadPlayerPosition>(), j, false);
			}
			else {
				manager.send(&messages::messageRef<messages::PayloadOtherPlayerPosition>(), j, false);
			}
		}

		auto &dynamicPayload = messages::messageRef<messages::PayloadDynamicPosition>().payload;
		for (int j = 0; j < Level::getNumDynamics(); ++j) {
			
			//std::cout << shared::getDynamic(j) << '\n';

			auto pos = shared::getDynamic(j)->getRigidActor()->getGlobalPose().p;
			auto rot = shared::getDynamic(j)->getRigidActor()->getGlobalPose().q;
			
			dynamicPayload.id = j;
			dynamicPayload.pos = { pos.x,pos.y,pos.z };
			dynamicPayload.rot.x = rot.x;
			dynamicPayload.rot.y = rot.y;
			dynamicPayload.rot.z = rot.z;
			dynamicPayload.rot.w = rot.w;

			manager.send(&messages::messageRef<messages::PayloadDynamicPosition>(), false);
		}
		//std::cout << "END\n\n";
	}
}


void engineLoop(float delta) {

}
