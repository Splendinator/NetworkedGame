#include "PxRigidDynamic.h"

class physx::PxRigidDynamic;

namespace Physics {

	void initialize();

	physx::PxRigidDynamic *addOBB();

	void update(float delta);
}
