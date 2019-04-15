#pragma once
#include "Quaternion.h"
#include "PxRigidDynamic.h"

class physx::PxRigidDynamic;
class Mesh;


class Transform
{
public:
	Transform();
	~Transform();

	void setRigidBody(physx::PxRigidDynamic *p) { _rigidbody = p; }

	physx::PxRigidDynamic *_rigidbody;
	
	quatf _rot;
	vec3f _pos;
	vec3f _scale;
};

