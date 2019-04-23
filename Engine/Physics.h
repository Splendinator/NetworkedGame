#include "PxRigidDynamic.h"
#include "Quaternion.h"

class physx::PxRigidActor;

namespace Physics {

	void initialize();

	physx::PxRigidActor *addOBB(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);
	physx::PxRigidActor *addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic);

	void update(float delta);

	void cleanUp();
}
