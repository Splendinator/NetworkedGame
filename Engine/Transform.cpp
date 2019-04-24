#include "Transform.h"
#include "Math.h"




Transform::~Transform()
{
}

Mat4f Transform::getModelMatrix()
{
	Mat4f m;
	//m.toIdentity();
	m = Math::buildTranslateMatrix(_pos) * Math::buildScaleMatrix(_scale) * _rot.toMat4();
	//m = _rot.toMat4();
	return m;

}

void Transform::update()
{
	if (!_rigidbody) return;

	_pos[0] = _rigidbody->getGlobalPose().p.x;
	_pos[1] = _rigidbody->getGlobalPose().p.y;
	_pos[2] = _rigidbody->getGlobalPose().p.z;

	_rot.x = _rigidbody->getGlobalPose().q.x;
	_rot.y = _rigidbody->getGlobalPose().q.y;
	_rot.z = _rigidbody->getGlobalPose().q.z;
	_rot.w = _rigidbody->getGlobalPose().q.w;

}
