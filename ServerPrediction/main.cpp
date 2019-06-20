#include <iostream>

#include "Messages.h"
#include "ServerFramework.cpp"
#include "../Engine/Math.h"

static const int PACKET_BUFFER_SIZE = 10;	//Lockstep Buffer Size;

//Buffer players' input for a few frames to avoid packet loss and keep all players in lock-step
struct Input{
	float yaw;
	unsigned int time;
};
Input bufferInput[NUM_PLAYERS * PACKET_BUFFER_SIZE]{};

unsigned int frameNum = 0;
unsigned int networkTime = 0;
unsigned short bufferIndex = 0;




void preInit() {

	for (int i = 0; i < NUM_PLAYERS * PACKET_BUFFER_SIZE; ++i) {
		bufferInput[i].yaw = -1.f;
	}

	manager.addListener(messages::MT_KEY_PRESS_BUFFERED, [&](BaseMessage *bm, int player) {
		auto  m = (Message<messages::PayloadKeyPressBuffered> *)bm;
		//auto &p = shared::getPlayer(i);
		
		Camera c;
		Vec3f v = { 0,0,0 };
		c.pitch = 0;
		for (int i = 0; i < Networking::INPUT_BUFFER_AMMOUNT; ++i) {
			c.yaw = m->payload.rot[i];
			float yaw = m->payload.rot[i];

			while (yaw < 0) {
				yaw += 2 * Math::PI;
			}

			if ((m->payload.input[i] & 0b0011) == 0b0011)	//A and D cancel
				m->payload.input[i] -= 0b0011;
			if ((m->payload.input[i] & 0b1100) == 0b1100)	//W and S cancel
				m->payload.input[i] -= 0b1100;


			switch (m->payload.input[i]) {
			case 0b0100:	//W
				break;
			case 0b1000:	//S
				yaw += Math::PI;
				break;
			case 0b0001:	//D
				yaw += Math::PI / 2;
				break;
			case 0b0010:	//A
				yaw += 1.5f*Math::PI;
				break;

			case 0b0101:	//WD
				yaw += Math::PI / 4;
				break;
			case 0b0110:	//WA
				yaw += 7 * Math::PI / 4;
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

			int dif = networkTime - m->payload.time;
			if (dif >= PACKET_BUFFER_SIZE) dif = PACKET_BUFFER_SIZE - 1;
			if (dif < 0) dif = 0;
			//bufferInput[((bufferIndex + PACKET_BUFFER_SIZE - dif) % PACKET_BUFFER_SIZE) + PACKET_BUFFER_SIZE * i].input = m->payload.input;
			int temp = ((bufferIndex + PACKET_BUFFER_SIZE - dif) % PACKET_BUFFER_SIZE) - i;
			
			if (temp < 0) temp += PACKET_BUFFER_SIZE;
		
			int index = (temp + PACKET_BUFFER_SIZE * player);

			bufferInput[index].yaw = yaw;
			bufferInput[index].time = m->payload.physTime[i];

			
		}
		
	//std::cout << "Networked Time: " << networkTime << ", client time: " << m->payload.time << " bufferIndex = " << bufferIndex << "input index = " << ((bufferIndex + PACKET_BUFFER_SIZE - dif) % PACKET_BUFFER_SIZE) + PACKET_BUFFER_SIZE * i << '\n';
	
	});

}

void postInit() {

	for (int i = 0; i < 5; ++i) {
		Level::loadCube({ -4.5,float(1 + i),-2.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	}
	
	Level::loadCube({ 0,-3,0 }, { 48,1,48 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		Level::loadPlayer(i, { float(5*i),15,0 });
	}

	messages::messageRef<messages::PayloadRUDPSnapshot>().payload.players = Level::getNumPlayers();
	messages::messageRef<messages::PayloadRUDPSnapshot>().payload.dynamics = Level::getNumDynamics();
	

}



void networkLoop() {
	
#pragma region INPUT_BUFFER
	++bufferIndex %= PACKET_BUFFER_SIZE;
	++networkTime;
#pragma endregion;

	if (frameNum < ((Networking::NETWORK_UPDATE_DELTA / Networking::PHYSICS_UPDATE_DELTA) * PACKET_BUFFER_SIZE)) return;

	auto &pl = messages::messageRef<messages::PayloadRUDPSnapshot>().payload;

	//pl.time = (frameNum - (Networking::NETWORK_UPDATE_DELTA / Networking::PHYSICS_UPDATE_DELTA) * PACKET_BUFFER_SIZE) + (Networking::NETWORK_UPDATE_DELTA / Networking::PHYSICS_UPDATE_DELTA);
	int index = 0;

	messages::PayloadPredictionOtherPositionNT playerPL;
	messages::PayloadPredictionDynamicPositionNT dynamicPL;

	for (int i = 0; i < NUM_PLAYERS; ++i) {
		shared::getPlayer(i).yaw = bufferInput[i * PACKET_BUFFER_SIZE + bufferIndex].yaw;

		playerPL.id = i;
		playerPL.movementDir = bufferInput[i * PACKET_BUFFER_SIZE + bufferIndex].yaw;
		playerPL.pos = shared::getPlayer(i).transform->_pos;
		playerPL.yVel = shared::getPlayer(i).transform->getRigidBody()->getLinearVelocity().y;


		memcpy((void *)&pl.data[index], &playerPL, sizeof(playerPL));
		index += sizeof(playerPL);
	
	}
	for (int j = 0; j < Level::getNumDynamics(); ++j) {
	
		auto pos = shared::getDynamic(j)->getRigidActor()->getGlobalPose().p;
		auto rot = shared::getDynamic(j)->getRigidActor()->getGlobalPose().q;
		auto linVel = shared::getDynamic(j)->getRigidBody()->getLinearVelocity();
		auto angVel = shared::getDynamic(j)->getRigidBody()->getAngularVelocity();
		
		dynamicPL.id = j;
		dynamicPL.angVel = { angVel.x,angVel.y,angVel.z };
		dynamicPL.linVel = { linVel.x,linVel.y,linVel.z };
		dynamicPL.pos = { pos.x,pos.y,pos.z };
		dynamicPL.rot = Quatf(rot.x,rot.y,rot.z,rot.w);

		memcpy((void *)&pl.data[index], &dynamicPL, sizeof(dynamicPL));
		index += sizeof(dynamicPL);
	}

	for (int i = 0; i < NUM_PLAYERS; ++i) {
		pl.time = bufferInput[i * PACKET_BUFFER_SIZE + bufferIndex].time;

		messages::messageRef<messages::PayloadRUDPSnapshot>().setPayloadSize(index + sizeof(pl.time) + sizeof(pl.dynamics) + sizeof(pl.players));
		manager.send(&messages::messageRef<messages::PayloadRUDPSnapshot>(),i,false);
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
			Vec3f v = c.foward() * Networking::PLAYER_MOVE_SPEED;
			auto pose = shared::getPlayer(i).transform->getRigidActor()->getGlobalPose();
			pose.p += { v[0], v[1], v[2] };
			shared::getPlayer(i).transform->getRigidActor()->setGlobalPose(pose);
			
		}
		
	}
	
}