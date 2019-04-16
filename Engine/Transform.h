#pragma once
#include "Quaternion.h"
#include "PxRigidDynamic.h"

class physx::PxRigidActor;
class Mesh;


class Transform
{
public:
	Transform();
	~Transform();

	void setRigidBody(physx::PxRigidActor *p) { _rigidbody = p; }
	Mat4f getModelMatrix();

	void update();

	Mat4f _modelMatrix;
	bool _dirty = true;

	physx::PxRigidActor *_rigidbody;
	
	Quatf _rot;
	Vec3f _pos;
	Vec3f _scale;
};

