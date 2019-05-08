#include "../Engine/Quaternion.h"
#include "../Engine/Engine.h"
#include "../Engine/Math.h"
#include "DomNet.h"
#include "Timer.h"
#include "Shared.h"
#include "Definitions.h"



using namespace domnet;

Engine engine;

Timer networkTimer;
Timer deltaTimer;


//Loops only every NETWORK_UPDATE_DELTA
void networkLoop();

//Loops as often as possible, passing delta time.
void engineLoop(float);

//Things to do before initializing engine and network
void preInit();

//Things to do after initializing engine and network
void postInit();

//Called every time the physics engine is ran.
void physicsLoop();