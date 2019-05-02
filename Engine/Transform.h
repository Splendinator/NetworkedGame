#pragma once
#include "Quaternion.h"
#include "PxRigidDynamic.h"

//class physx::PxRigidActor;
class Mesh;


class Transform
{
public:
	Transform(bool dynamic) : _dynamic(dynamic) {};
	~Transform();

	void setRigidBody(physx::PxRigidActor *p) { _rigidbody = p; }
	physx::PxRigidBody * getRigidBody() { return (_dynamic ? (physx::PxRigidBody *) _rigidbody : nullptr); };
	physx::PxRigidActor * getRigidActor() { return _rigidbody; };
	bool isDynamic() { return _dynamic; }
	Mat4f getModelMatrix();

	void update();

	Mat4f _modelMatrix;
	bool _dirty = true;

	void setPos(Vec3f pos);
	
	Quatf _rot;
	Vec3f _pos;
	Vec3f _scale;

private:
	const bool _dynamic;
	physx::PxRigidActor *_rigidbody;

};

