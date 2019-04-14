#pragma once
#include "Vector.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	static Mesh *genCube();

	void bufferMesh();

	const unsigned int getVaoID() { return vaoID; }
	const unsigned int getVertexID() { return vertexID; }
	const unsigned int getIndiciesID() { return indiciesID; }
	const unsigned int getNormalID() { return normalsID; }
	const unsigned int getNumInds() { return numInds; }
	const unsigned int getNumVerts() { return numVerts; }

private:
	unsigned int numInds, numVerts;
	
	vec3f *verts;
	vec3f *normals;
	unsigned int *inds;

	vec3f colour;
	unsigned int vaoID;
	unsigned int vertexID;
	unsigned int normalsID;
	unsigned int indiciesID;
	
};

