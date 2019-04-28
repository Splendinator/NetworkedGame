#include <iostream>
#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h"
#include "ManagerClient.h"
#include "Messages.h"

using namespace domnet;

Engine e;
//Transform *player;
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

void engineLoop() {
  
		//e.getCamera()->pos = player->_pos;
		//
		//if (e.isHeld('D'))
		//{
		//	Vec3f right = e.getCamera()->right() * 0.04f;
		//	physx::PxTransform t = player->getRigidActor()->getGlobalPose();
		//	t.p += {right[0], right[1], right[2]};
		//	player->getRigidBody()->setGlobalPose(t);
		//
		//}
		//if (e.isHeld('A'))
		//{
		//	Vec3f right = e.getCamera()->right() * -0.04f;
		//	physx::PxTransform t = player->getRigidActor()->getGlobalPose();
		//	t.p += {right[0], right[1], right[2]};
		//	player->getRigidBody()->setGlobalPose(t);
		//}
		//if (e.isHeld('W'))
		//{
		//	Vec3f right = e.getCamera()->up();
		//	right[1] = 0;
		//	right.normalize();
		//	right = right * 0.04f;
		//	physx::PxTransform t = player->getRigidActor()->getGlobalPose();
		//	t.p += {right[0], right[1], right[2]};
		//	player->getRigidBody()->setGlobalPose(t);
		//}
		//if (e.isHeld('S'))
		//{
		//	Vec3f right = e.getCamera()->up();
		//	right[1] = 0;
		//	right.normalize();
		//	right = right * -0.04f;
		//	physx::PxTransform t = player->getRigidActor()->getGlobalPose();
		//	t.p += {right[0], right[1], right[2]};
		//	player->getRigidBody()->setGlobalPose(t);
		//}
		//
		//
		//e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
		//e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
		//if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
		//if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
		
		//
		//
		////Player Rotation
		//Quatf upright = Quatf::quatFromEuler({ 0,0,1 }, (Math::PI) / 2.f);
		//physx::PxTransform transform = player->getRigidActor()->getGlobalPose();
		//transform.q = { upright.x,upright.y,upright.z,upright.w };
		//player->getRigidBody()->setGlobalPose(transform);
		
		e.update(0.004f);
}

int main() {

	manager.addListener(Messages::MT_PLAYER_POSITION, [&](BaseMessage *m) {
		std::cout << "UDP Packet Recieved\n";
		manager.send(&Messages::m_PlayerPosition(), false);
	});

	networkSetup();
	engineSetup();

	for (;;) {
		manager.update();
		engineLoop();
	}


	//getchar();
	return 0;
}