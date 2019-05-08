#include <iostream>
#include "ClientFramework.cpp"



void preInit() {

	engine.setDoPhysics(false);

	
}

void postInit() {
	manager.addListener(messages::MT_PREDICTION_PLAYER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadPredictionPlayerPosition> *)m;
		shared::getCurrPlayer().transform->setPos(p->payload.pos);
		shared::getCurrPlayer().yaw = p->payload.movementDir;

	});

	manager.addListener(messages::MT_PREDICTION_OTHER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadPredictionOtherPosition> *)m;
		shared::getPlayer(p->payload.id).transform->setPos(p->payload.pos);
	});

	manager.addListener(messages::MT_PREDICTION_DYNAMIC_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadPredictionDynamicPosition> *)m;
		auto transform = shared::getDynamic(p->payload.id)->getRigidBody()->getGlobalPose();
		transform.q = { p->payload.rot.x,p->payload.rot.y,p->payload.rot.z,p->payload.rot.w };
		transform.p = { p->payload.pos[0], p->payload.pos[1], p->payload.pos[2] };
		shared::getDynamic(p->payload.id)->getRigidBody()->setGlobalPose(transform);
	});
}

void engineLoop(float delta) {
  
		engine.getCamera()->pos = shared::getCurrPlayer().transform->_pos;
		
		messages::messageRef<messages::PayloadKeyPress>().payload.input = 0;

		if (engine.isHeld('D'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1;
			
		}
		if (engine.isHeld('A'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b10;
			
		}
		if (engine.isHeld('W'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b100;
			
		}
		if (engine.isHeld('S'))
		{
			messages::messageRef<messages::PayloadKeyPress>().payload.input += 0b1000;

		}
		
		messages::messageRef<messages::PayloadKeyPress>().payload.rot = engine.getCamera()->yaw;
		
}

void networkLoop() {
	manager.send(&messages::messageRef<messages::PayloadKeyPress>(), false);
}

void physicsLoop() {
	snapshotManager.incrementTime();
}
