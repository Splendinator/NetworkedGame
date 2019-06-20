#include <iostream>
#include "ClientFramework.cpp"

unsigned int networkTime = 0;

messages::PayloadKeyPress lastSentInput;

void preInit() {

	engine.setDoPhysics(false);

	
}

void postInit() {
	manager.addListener(messages::MT_RUDP_SNAPSHOT, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadRUDPSnapshot> *)m;
		
		snapshotManager.receiveSnapshotFromServer(p->payload.data, p->payload.players, p->payload.dynamics, p->payload.time);
	});
}

void engineLoop(float delta) {
  
		engine.getCamera()->pos = shared::getCurrPlayer().transform->_pos;
		
		messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0] = 0;
		 
		if (engine.isHeld('D'))
		{
			messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0] += 0b1;
			
		}
		if (engine.isHeld('A'))
		{
			messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0] += 0b10;
			
		}
		if (engine.isHeld('W'))
		{
			messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0] += 0b100;
			
		}
		if (engine.isHeld('S'))
		{
			messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0] += 0b1000;

		}
		
		messages::messageRef<messages::PayloadKeyPressBuffered>().payload.rot[0] = engine.getCamera()->yaw;
		
}

void networkLoop() {
	messages::messageRef<messages::PayloadKeyPressBuffered>().payload.time = networkTime++;
	messages::messageRef<messages::PayloadKeyPressBuffered>().payload.physTime[0] = snapshotManager.getPredictedTime();
	lastSentInput.rot = messages::messageRef<messages::PayloadKeyPressBuffered>().payload.rot[0];
	lastSentInput.input = messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[0];
	
	manager.send(&messages::messageRef<messages::PayloadKeyPressBuffered>(), false);
	
	for (int i = Networking::INPUT_BUFFER_AMMOUNT - 1; i > 0; --i) {
		messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[i] = messages::messageRef<messages::PayloadKeyPressBuffered>().payload.input[i - 1];
		messages::messageRef<messages::PayloadKeyPressBuffered>().payload.rot[i] = messages::messageRef<messages::PayloadKeyPressBuffered>().payload.rot[i - 1];
		messages::messageRef<messages::PayloadKeyPressBuffered>().payload.physTime[i] = messages::messageRef<messages::PayloadKeyPressBuffered>().payload.physTime[i - 1];
	}
	
}

void physicsLoop() {
	for (int i = 0; i < readycheck::numPlayers; ++i) {
		snapshotManager.syncPlayer(shared::getPlayer(i).transform->getRigidActor(), i);
	}
	for (int i = 0; i < readycheck::numDynamics; ++i) {
		snapshotManager.syncDynamic(shared::getDynamic(i)->getRigidActor(), i);
	}
#pragma region GET_YAW
	//auto &m = messages::messageRef<messages::PayloadKeyPress>();
	Camera c;
	Vec3f v = { 0,0,0 };
	c.pitch = 0;
	c.yaw = lastSentInput.rot;
	float yaw = lastSentInput.rot;

	while (yaw < 0) {
		yaw += 2 * Math::PI;
	}

	if ((lastSentInput.input & 0b0011) == 0b0011)	//A and D cancel
		lastSentInput.input -= 0b0011;
	if ((lastSentInput.input & 0b1100) == 0b1100)	//W and S cancel
		lastSentInput.input -= 0b1100;


	switch (lastSentInput.input) {
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
#pragma endregion
	snapshotManager.setCurrPlayerYaw(yaw);
	snapshotManager.incrementTime();
	//snapshotManager.setPlayerYaw(yaw,shared::getCurrPlayerId());
	
}
