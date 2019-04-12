#pragma once
#include "Vector.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	static Mesh *genCube();

private:
	int numInds, numVerts;
	vec3f *verts;
	int *inds;
	vec3f colour;
};

