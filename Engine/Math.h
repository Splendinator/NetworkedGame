#pragma once

#include "Matrix.h"

namespace Math
{
	static const float PI = 3.14159265359f;

	Mat4f buildScaleMatrix(Vec3f v);
	Mat4f buildTranslateMatrix(Vec3f v);
	Mat4f buildRotateMatrixX(float rad);
	Mat4f buildRotateMatrixY(float rad);
	Mat4f buildRotateMatrixZ(float rad);
	Mat4f buildPerspectiveMatrix(float fov, float aspect, float near, float far);
};

//Math::PI = 3.14159265359f;
