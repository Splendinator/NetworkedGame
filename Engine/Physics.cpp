#include "Physics.h"
#include "PxPhysicsAPI.h"
#include <iostream>
#include <vector>

using namespace physx;


physx::PxFoundation* gFoundation = nullptr;

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


void Physics::initialize() {

	if(!gFoundation)
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	
	gPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), false, nullptr);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void Physics::update(float delta, bool fetch){
	gScene->simulate(delta);
	gScene->fetchResults(true);
}

void Physics::cleanUp()
{ 
	gPhysics->release();

	if(gFoundation)
	gFoundation->release();
}

PxRigidActor *Physics::addOBB(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic) {


	PxShape* shape = gPhysics->createShape(PxBoxGeometry(scale[0], scale[1], scale[2]), *gMaterial);

	PxTransform localTm(PxVec3(pos[0], pos[1], pos[2]));
	localTm.q.x = rot.x;
	localTm.q.y = rot.y;
	localTm.q.z = rot.z;
	localTm.q.w = rot.w;
	PxRigidActor *body;
	if (dynamic) {
		body = gPhysics->createRigidDynamic(localTm);
		PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic *)body, 10.0f);
	}
	else {
		body = gPhysics->createRigidStatic(localTm);
	}
	body->attachShape(*shape);
	gScene->addActor(*body);
	return body;
}

PxRigidActor *Physics::addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic) {

	static PxMaterial *playerMaterial = gPhysics->createMaterial(1.0f, 1.0f, 0.f);

	PxShape* shape = gPhysics->createShape(PxCapsuleGeometry(1,1), *playerMaterial);

	PxTransform localTm(PxVec3(pos[0], pos[1], pos[2])); 
	localTm.q.x = rot.x;
	localTm.q.y = rot.y;
	localTm.q.z = rot.z;
	localTm.q.w = rot.w;

	PxRigidActor *body;
	if (dynamic) {
		body = gPhysics->createRigidDynamic(localTm);
		PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic *)body, 10.0f);
	}
	else {
		body = gPhysics->createRigidStatic(localTm);
	}
	body->attachShape(*shape);
	gScene->addActor(*body);
	return body;
}

