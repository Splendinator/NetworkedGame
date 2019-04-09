#include <iostream>
#include "PxPhysicsAPI.h"

using namespace physx;
using namespace std;

class UserErrorCallback : public PxErrorCallback
{
public:
	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
		int line)
	{
		cout << "PhysX Error\nCode: " << code << "\nMessage: " << message << "\n";
	}
};


PxDefaultAllocator		gAllocator;
UserErrorCallback		gErrorCallback;

PxFoundation*			gFoundation = nullptr;
PxPhysics*				gPhysics = nullptr;

PxDefaultCpuDispatcher*	gDispatcher = nullptr;
PxScene*				gScene = nullptr;

PxMaterial*				gMaterial = nullptr;

PxPvd*                  gPvd = nullptr;

int main() {


	PxTolerancesScale scale;
	scale.length = 100;        // typical length of an object
	scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);



	getchar();

	return 0;
}