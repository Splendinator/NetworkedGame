#include <iostream>

#include "Messages.h"
#include "ServerFramework.cpp"
#include "../Engine/Math.h"


static const float PLAYER_MOVE_SPEED = 0.15f;

unsigned int frameNum = 0;



void preInit() {

	manager.addListener(messages::MT_KEY_PRESS, [&](BaseMessage *bm, int i) {
		auto  m = (Message<messages::PayloadKeyPress> *)bm;
		auto &p = shared::getPlayer(i);
		Camera c;
		Vec3f v = { 0,0,0 };
		c.pitch = 0;
		c.yaw = m->payload.rot;
		float yaw = m->payload.rot;

		while (yaw < 0) {
			yaw += 2*Math::PI;
		}

		if ((m->payload.input & 0b0011) == 0b0011)	//A and D cancel
			m->payload.input -= 0b0011;
		if ((m->payload.input & 0b1100) == 0b1100)	//W and S cancel
			m->payload.input -= 0b1100;

			
	switch (m->payload.input) {
		case 0b0100:	//W
			break;
		case 0b1000:	//S
			yaw += Math::PI;
			break;
		case 0b0001:	//D
			yaw += Math::PI/2;
			break;
		case 0b0010:	//A
			yaw += 1.5f*Math::PI;
			break;
		
		case 0b0101:	//WD
			yaw += Math::PI / 4;
			break;
		case 0b0110:	//WA
			yaw += 7*Math::PI / 4;
			break;

		case 0b1001:	//SD
			yaw += 3 * Math::PI / 4;
			break;
		case 0b1010:	//SA
			yaw += 5 * Math::PI / 4;
			break;
		
		default:
			yaw = -1.f;
	}

	//while (yaw > 2 * Math::PI) {
	//	yaw -= 2 * Math::PI;
	//}

	p.yaw = yaw;

		//if(m->payload.input == 0)
		//	yaw = -1.f;
		//else {
		//	yaw = m->payload.rot;
		//	if (m->payload.input & 0b1) { // D
		//		//yaw += Math::PI / 2;
		//		//v += c.right();
		//		//v = c.right() * PLAYER_MOVE_SPEED;
		//		//auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
		//		//pose.p += {v[0], v[1], v[2]};
		//		//shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		//	}
		//	if (m->payload.input & 0b10) { // A
		//		//yaw += 3 * Math::PI / 2;
		//		//v = -c.right() * PLAYER_MOVE_SPEED;
		//		//auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
		//		//pose.p += {v[0], v[1], v[2]};
		//		//shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		//	}
		//	if (m->payload.input & 0b100) { // W
		//		//v = c.foward() * PLAYER_MOVE_SPEED;
		//		//auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
		//		//pose.p += {v[0], v[1], v[2]};
		//		//shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		//	}
		//	if (m->payload.input & 0b1000) { // S
		//		//v = -c.foward() * PLAYER_MOVE_SPEED;
		//		//auto pose = shared::getPlayer(i)->getRigidBody()->getGlobalPose();
		//		//pose.p += {v[0], v[1], v[2]};
		//		//shared::getPlayer(i)->getRigidActor()->setGlobalPose(pose);
		//	}
		//}
	
	});

}

void postInit() {

	//Level::loadCube({ 0.5,4 ,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	//Level::loadCube({ 0.5,4 ,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	//Level::loadCube({ -0.5,7,0.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	//Level::loadCube({ 4.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	//Level::loadCube({ -2.5,3 ,5.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	//Level::loadCube({ 6.5,4 ,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Level::loadCube({ -4.5,2 ,-2.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	
	Level::loadCube({ 0,-3,0 }, { 48,1,48 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		Level::loadPlayer(i, { float(5*i),15,0 });
	}
	

}



void networkLoop() {
	//for (int i = 0; i < NUM_PLAYERS; ++i) {
	//	messages::messageRef<messages::PayloadPlayerPosition>().payload.pos = shared::getPlayer(i)->_pos;
	//	messages::messageRef<messages::PayloadOtherPlayerPosition>().payload.pos = shared::getPlayer(i)->_pos;
	//	messages::messageRef<messages::PayloadOtherPlayerPosition>().payload.id = i;
	//
	//	for (int j = 0; j < NUM_PLAYERS; ++j) {
	//
	//		if (i == j) {
	//			manager.send(&messages::messageRef<messages::PayloadPlayerPosition>(), j, false);
	//		}
	//		else {
	//			manager.send(&messages::messageRef<messages::PayloadOtherPlayerPosition>(), j, false);
	//		}
	//	}
	//
	//	auto &dynamicPayload = messages::messageRef<messages::PayloadDynamicPosition>().payload;
	//	for (int j = 0; j < Level::getNumDynamics(); ++j) {
	//		
	//
	//		auto pos = shared::getDynamic(j)->getRigidActor()->getGlobalPose().p;
	//		auto rot = shared::getDynamic(j)->getRigidActor()->getGlobalPose().q;
	//		
	//		dynamicPayload.id = j;
	//		dynamicPayload.pos = { pos.x,pos.y,pos.z };
	//		dynamicPayload.rot.x = rot.x;
	//		dynamicPayload.rot.y = rot.y;
	//		dynamicPayload.rot.z = rot.z;
	//		dynamicPayload.rot.w = rot.w;
	//
	//		manager.send(&messages::messageRef<messages::PayloadDynamicPosition>(), false);
	//	}
	//}
	
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		messages::messageRef<messages::PayloadPredictionPlayerPosition>().payload.pos = shared::getPlayer(i).transform->_pos;
		messages::messageRef<messages::PayloadPredictionPlayerPosition>().payload.movementDir = shared::getPlayer(i).yaw;
		messages::messageRef<messages::PayloadPredictionPlayerPosition>().payload.time = frameNum;

		messages::messageRef<messages::PayloadPredictionOtherPosition>().payload.pos = shared::getPlayer(i).transform->_pos;
		messages::messageRef<messages::PayloadPredictionOtherPosition>().payload.id = i;
		messages::messageRef<messages::PayloadPredictionOtherPosition>().payload.movementDir = shared::getPlayer(i).yaw;
		messages::messageRef<messages::PayloadPredictionOtherPosition>().payload.time = frameNum;



		for (int j = 0; j < NUM_PLAYERS; ++j) {

			if (i == j) {
				manager.send(&messages::messageRef<messages::PayloadPredictionPlayerPosition>(), j, false);
			}
			else {
				manager.send(&messages::messageRef<messages::PayloadPredictionOtherPosition>(), j, false);
			}
		}

		auto &dynamicPayload = messages::messageRef<messages::PayloadPredictionDynamicPosition>().payload;
		for (int j = 0; j < Level::getNumDynamics(); ++j) {


			auto pos = shared::getDynamic(j)->getRigidActor()->getGlobalPose().p;
			auto rot = shared::getDynamic(j)->getRigidActor()->getGlobalPose().q;

			dynamicPayload.id = j;
			dynamicPayload.pos = { pos.x,pos.y,pos.z };
			dynamicPayload.rot.x = rot.x;
			dynamicPayload.rot.y = rot.y;
			dynamicPayload.rot.z = rot.z;
			dynamicPayload.rot.w = rot.w;

			manager.send(&messages::messageRef<messages::PayloadPredictionDynamicPosition>(), false);
		}
	}

}


void engineLoop(float delta) {

}


void physicsLoop() {
	++frameNum;
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		Camera c;
		c.pitch = 0;
		if (shared::getPlayer(i).yaw >= 0.f) {
			c.yaw = shared::getPlayer(i).yaw;
			Vec3f v = c.foward() * PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i).transform->getRigidActor()->getGlobalPose();
			pose.p += { v[0], v[1], v[2] };
			shared::getPlayer(i).transform->getRigidActor()->setGlobalPose(pose);
		}
	}
}