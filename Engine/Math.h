#pragma once

#include "Matrix.h"

class Math
{
public:
	Math();
	~Math();

	static mat4f buildScaleMatrix(vec3f v);
	static mat4f buildTranslateMatrix(vec3f v);
	static mat4f buildRotateMatrixX(float rad);
	static mat4f buildRotateMatrixY(float rad);
	static mat4f buildRotateMatrixZ(float rad);
};

