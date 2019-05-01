#include <iostream>
#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h" 
#include "ManagerServer.h"
#include "Messages.h"
#include "Level.h"





using namespace domnet;

Engine e;
Transform *player;
ManagerServer manager;

static const int NUM_PLAYERS = 1;


void networkSetup() {
	
	manager.host(NUM_PLAYERS);

}

void engineSetup() {

	e.init();
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

	//player = e.addCapsule({ 0,10,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f), true, true);
	//player->getRigidBody()->setMass(200000.f);
	//player->getRigidBody()->setLinearDamping(0.5f);
	

}

void engineLoop() {

	if (e.isHeld('D'))
	{
		e.getCamera()->pos += e.getCamera()->right() * 0.02f;
	}
	if (e.isHeld('A'))
	{
		e.getCamera()->pos -= e.getCamera()->right() * 0.02f;
	}
	if (e.isHeld('W'))
	{
		e.getCamera()->pos += e.getCamera()->foward() * 0.02f;
	}
	if (e.isHeld('S'))
	{
		e.getCamera()->pos -= e.getCamera()->foward() * 0.02f;
	}
	if (e.isHeld(' '))
	{
		e.getCamera()->pos += e.getCamera()->up() * 0.02f;
	}
	if (e.isHeld(0x11))
	{
		e.getCamera()->pos -= e.getCamera()->up() * 0.02f;
	}


	e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
	e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
	if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
	if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
	e.update(0.004f);


	////Player Rotation
	//Quatf upright = Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f);
	//physx::PxTransform transform = player->getRigidActor()->getGlobalPose();
	//transform.q = { upright.x,upright.y,upright.z,upright.w };
	//player->getRigidBody()->setGlobalPose(transform);
}

int main() {



	networkSetup();
	engineSetup();
	

	for (;;) {
		manager.update();
		engineLoop();
	}


	//getchar();
	return 0;
}