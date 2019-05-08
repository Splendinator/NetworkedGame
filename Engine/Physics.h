#include "PxRigidDynamic.h"
#include "Quaternion.h"

namespace physx {
	class PxRigidActor;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxPvd;
	class PxRigidDynamic;
}

class Physics {
public:
	void initialize();

	physx::PxRigidActor *addOBB(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);
	physx::PxRigidActor *addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);

	void update(float delta, bool fetch = true);

	void cleanUp();

private:
	physx::PxPhysics*				gPhysics = nullptr;
	physx::PxDefaultCpuDispatcher*	gDispatcher = nullptr;
	physx::PxScene*					gScene = nullptr;
	physx::PxMaterial*				gMaterial = nullptr;
	physx::PxPvd*					gPvd = nullptr;
	physx::PxRigidDynamic*			cube = nullptr;

};
