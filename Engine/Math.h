#pragma once

#include "Matrix.h"

namespace Math
{
	static const float PI = 3.14159265359f;

	mat4f buildScaleMatrix(vec3f v);
	mat4f buildTranslateMatrix(vec3f v);
	mat4f buildRotateMatrixX(float rad);
	mat4f buildRotateMatrixY(float rad);
	mat4f buildRotateMatrixZ(float rad);
	mat4f buildPerspectiveMatrix(float fov, float aspect, float near, float far);
};

//Math::PI = 3.14159265359f;
