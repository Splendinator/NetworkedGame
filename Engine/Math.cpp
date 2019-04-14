#include "Math.h"
#include <math.h>


mat4f Math::buildScaleMatrix(vec3f v)
{
	mat4f m;
	m.toIdentity();

	m[0][0] = v[0];
	m[1][1] = v[1];
	m[2][2] = v[2];

	return m;
}

mat4f Math::buildTranslateMatrix(vec3f v)
{
	mat4f m;
	m.toIdentity();
	
	m[0][3] = v[0];
	m[1][3] = v[1];
	m[2][3] = v[2];

	return m;
}

mat4f Math::buildRotateMatrixX(float rad)
{
	mat4f m;
	m.toIdentity();

	m[1][1] = m[2][2] = static_cast<float>(cos(rad));
	m[1][2] = static_cast<float>(-sin(rad));
	m[2][1] = static_cast<float>(sin(rad));

	return m;
}

mat4f Math::buildRotateMatrixY(float rad)
{
	mat4f m;
	m.toIdentity();

	m[0][0] = m[2][2] = static_cast<float>(cos(rad));
	m[2][0] = static_cast<float>(-sin(rad));
	m[0][2] = static_cast<float>(sin(rad));

	return m;
}

mat4f Math::buildRotateMatrixZ(float rad)
{
	mat4f m;
	m.toIdentity();

	m[0][0] = m[1][1] = static_cast<float>(cos(rad));
	m[0][1] = static_cast<float>(-sin(rad));
	m[1][0] = static_cast<float>(sin(rad));

	return m;
}

mat4f Math::buildPerspectiveMatrix(float fov, float aspect, float near, float far)
{
	mat4f m;
	m.toIdentity();

	float top = near * float(tan(Math::PI / 180.f * (fov / 2.f)));
	float bottom = -top;
	float right = top * aspect;
	float left = -right;

	m[0][0] = (2 * near) / (right - left);
	m[0][2] = (right + left) / (right - left);
	m[1][1] = (2 * near) / (top - bottom);
	m[1][2] = (top + bottom) / (top - bottom);
	m[2][2] = -((far + near) / (far - near));
	m[2][3] = -((2 * far * near) / (far - near));
	m[3][2] = -1;
	m[3][3] = 0;

	return m;
}

