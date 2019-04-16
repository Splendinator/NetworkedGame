#pragma once
#include "Mesh.h"
#include "Matrix.h"

class Renderable
{
public:
	Renderable(Mesh *m) {mesh = m;}
	~Renderable();

	Mat4f model;
private:
	Mesh *mesh;
	
	
};

