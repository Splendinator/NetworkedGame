#include "Physics.h"
#include "PxPhysicsAPI.h"
#include <iostream>
#include <vector>

using namespace physx;

namespace Physics {


	class UserErrorCallback : public PxErrorCallback
	{
	public:
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
			int line)
		{
			std::cout << "PhysX Error\nCode: " << code << "\nMessage: " << message << "\n";
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


	void initialize() {
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);

		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	}

	void update(float delta){
		gScene->simulate(delta);
		gScene->fetchResults(true);
	}
	
	PxRigidDynamic *addOBB() {

		PxShape* shape = gPhysics->createShape(PxBoxGeometry(1, 1, 1), *gMaterial);

		PxTransform localTm(PxVec3(0, 0, 0));
		PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0, 0, 0)));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		return body;
	}

}