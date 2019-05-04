#include <iostream>
#include "ClientFramework.cpp"



void preInit() {


	manager.addListener(messages::MT_PLAYER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadPlayerPosition> *)m;
		shared::getCurrPlayer()->setPos(p->payload.pos);
	});

	manager.addListener(messages::MT_OTHER_PLAYER_POSITION, [&](BaseMessage *m) {
		auto p = (domnet::Message<messages::PayloadOtherPlayerPosition> *)m;
		shared::getPlayer(p->payload.id)->setPos(p->payload.pos);
	});
}

void postInit() {

}

void engineLoop(float delta) {
  
		engine.getCamera()->pos = shared::getCurrPlayer()->_pos;
		
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

