#include <iostream>
#include "Quaternion.h"
#include "Engine.h"
#include "Math.h"



int main() {

	Engine e;
	e.init();
	e.addCube({ 0.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ -0.5,7,0.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ 4.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ -2.5,3,5.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ 6.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ -4.5,2,-2.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	Transform *player = e.addCapsule({ 0,10,0 }, { 1,1,1 }, Quatf::quatFromEuler({0,0,1},(Math::PI + 0.02f)/2.f),true,false);
	Transform *floor = e.addCube({ 0,-3,0 }, { 48,1,48 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	

	player->getRigidBody()->setAngularDamping(PX_MAX_F32);
	player->getRigidBody()->setMass(200.f);

	for (;;) {

		e.getCamera()->pos = player->_pos;

		//if (e.isHeld('D')) 
		//{
		//	e.getCamera()->pos += e.getCamera()->right() * 0.02f;
		//}
		//if (e.isHeld('A'))
		//{
		//	e.getCamera()->pos -= e.getCamera()->right() * 0.02f;
		//}
		//if (e.isHeld('W'))
		//{
		//	e.getCamera()->pos += e.getCamera()->foward() * 0.02f;
		//}										 
		//if (e.isHeld('S'))						 
		//{										 
		//	e.getCamera()->pos -= e.getCamera()->foward() * 0.02f;
		//}
		//if (e.isHeld(' '))
		//{
		//	e.getCamera()->pos += e.getCamera()->up() * 0.02f;
		//}
		//if (e.isHeld(0x11))
		//{
		//	e.getCamera()->pos -= e.getCamera()->up() * 0.02f;
		//}

		if (e.isHeld('D')) 
		{
			Vec3f right = e.getCamera()->right() * 0.04f;
			physx::PxTransform t = player->getRigidActor()->getGlobalPose();
			t.p += {right[0], right[1], right[2]};
			player->getRigidBody()->setGlobalPose(t);
			
		}
		if (e.isHeld('A'))
		{
			Vec3f right = e.getCamera()->right() * -0.04f;
			physx::PxTransform t = player->getRigidActor()->getGlobalPose();
			t.p += {right[0], right[1], right[2]};
			player->getRigidBody()->setGlobalPose(t);
		}
		if (e.isHeld('W'))
		{
			Vec3f right = e.getCamera()->up();
			right[1] = 0;
			right.normalize();
			right = right * 0.04f;
			physx::PxTransform t = player->getRigidActor()->getGlobalPose();
			t.p += {right[0], right[1], right[2]};
			player->getRigidBody()->setGlobalPose(t);
		}										 
		if (e.isHeld('S'))						 
		{										 
			Vec3f right = e.getCamera()->up();
			right[1] = 0;
			right.normalize();
			right = right * -0.04f;
			physx::PxTransform t = player->getRigidActor()->getGlobalPose();
			t.p += {right[0], right[1], right[2]};
			player->getRigidBody()->setGlobalPose(t);
		}
		
	
		e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
		e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
		if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
		if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
		e.update(0.004f);
	}


	getchar();
	return 0;
}