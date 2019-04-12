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

PxRigidDynamic *cube = nullptr;

int main() {


	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxShape* shape = gPhysics->createShape(PxBoxGeometry(1, 1, 1), *gMaterial);

	PxTransform localTm(PxVec3(0,0,0));
	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0, 0, 0)));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	gScene->addActor(*body);

	for (int i = 0; i < 1000; ++i) {
		gScene->simulate(1.0f / 120.0f);
		gScene->fetchResults(true);
		cout << body->getGlobalPose().p.y << "\n";
	}


	getchar();

	return 0;
}