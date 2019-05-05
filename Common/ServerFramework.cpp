#include "HostFramework.cpp"
#include "ManagerServer.h"

#include "Level.h"

static float PACKET_DROP_PCT = 0.f;	//Chance of UDP packets to not be sent (1.0f = 100%)
static float LATENCY = 0.f;	//Latency in MS	

static const int NUM_PLAYERS = 1;

static const float CAMERA_MOVE_SPEED = 0.02f;
static const float CAMERA_SENSITIVITY = 200.f;

ManagerServer manager(PACKET_DROP_PCT, LATENCY);


void baseLoop(float delta) {


	if (engine.isHeld('D'))
	{
		engine.getCamera()->pos += engine.getCamera()->right() * CAMERA_MOVE_SPEED;
	}
	if (engine.isHeld('A'))
	{
		engine.getCamera()->pos -= engine.getCamera()->right() * CAMERA_MOVE_SPEED;
	}
	if (engine.isHeld('W'))
	{
		engine.getCamera()->pos += engine.getCamera()->foward() * CAMERA_MOVE_SPEED;
	}
	if (engine.isHeld('S'))
	{
		engine.getCamera()->pos -= engine.getCamera()->foward() * CAMERA_MOVE_SPEED;
	}
	if (engine.isHeld(' '))
	{
		engine.getCamera()->pos += engine.getCamera()->up() * CAMERA_MOVE_SPEED;
	}
	if (engine.isHeld(0x11))
	{
		engine.getCamera()->pos -= engine.getCamera()->up() * CAMERA_MOVE_SPEED;
	}


	engine.getCamera()->yaw += engine.getMouseXDelta() / CAMERA_SENSITIVITY;
	engine.getCamera()->pitch += engine.getMouseYDelta() / CAMERA_SENSITIVITY;
	if (engine.getCamera()->pitch > 1.5f) engine.getCamera()->pitch = 1.5f;
	if (engine.getCamera()->pitch < -1.5f) engine.getCamera()->pitch = -1.5f;

	shared::setPlayersUpright();

	engine.update(delta);



}



void main() {
	float delta;
	
	preInit();
	engine.init(PHYSICS_UPDATE_DELTA);
	manager.host(NUM_PLAYERS);
	Level::init(&engine, &manager);
	postInit();


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
		baseLoop(delta);
		engineLoop(delta);

	}
}