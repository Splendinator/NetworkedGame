#include <iostream>
#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h" 
#include "ManagerServer.h"
#include "Messages.h"
#include "Level.h"
#include "Shared.h"
#include "Timer.h"





using namespace domnet;

Engine e;
Transform *player;
ManagerServer manager(0.0f,200.f);

static const int NUM_PLAYERS = 1;
static const float PLAYER_MOVE_SPEED = 0.15f;

static const float CAMERA_MOVE_SPEED = 0.02f;
static const float CAMERA_SENSITIVITY = 200.f;

static const float NETWORK_UPDATE_DELTA = 1 / 32.f;
static const float PHYSICS_UPDATE_DELTA = 1 / 128.f;

Timer networkTimer;
Timer deltaTimer;



void networkSetup() {
	
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

	manager.host(NUM_PLAYERS);

}

void engineSetup() {

	e.init(PHYSICS_UPDATE_DELTA);
	Level::init(&e, &manager);
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

void engineLoop(float delta) {


	if (e.isHeld('D'))
	{
		e.getCamera()->pos += e.getCamera()->right() * CAMERA_MOVE_SPEED;
	}
	if (e.isHeld('A'))
	{
		e.getCamera()->pos -= e.getCamera()->right() * CAMERA_MOVE_SPEED;
	}
	if (e.isHeld('W'))
	{
		e.getCamera()->pos += e.getCamera()->foward() * CAMERA_MOVE_SPEED;
	}
	if (e.isHeld('S'))
	{
		e.getCamera()->pos -= e.getCamera()->foward() * CAMERA_MOVE_SPEED;
	}
	if (e.isHeld(' '))
	{
		e.getCamera()->pos += e.getCamera()->up() * CAMERA_MOVE_SPEED;
	}
	if (e.isHeld(0x11))
	{
		e.getCamera()->pos -= e.getCamera()->up() * CAMERA_MOVE_SPEED;
	}


	e.getCamera()->yaw += e.getMouseXDelta() / CAMERA_SENSITIVITY;
	e.getCamera()->pitch += e.getMouseYDelta() / CAMERA_SENSITIVITY;
	if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
	if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
	
	shared::setPlayersUpright();

	e.update(delta);

	

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
	}
}

int main() {

	float delta;

	networkSetup();
	engineSetup();
	

	networkTimer.resetTimer();
	deltaTimer.resetTimer();
	for (;;) {
		if (networkTimer.getDelta() > NETWORK_UPDATE_DELTA) {
			networkTimer -= NETWORK_UPDATE_DELTA;
			manager.update();
			networkLoop();
		}
		delta = deltaTimer.getDelta();
		deltaTimer.resetTimer();
		engineLoop(delta);
		
	}


	//getchar();
	return 0;
}