#pragma once
#include "Vector.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	static Mesh *genCube();
	static Mesh *genCapsule(float height = 1.f, float rad = 1.f);

	void bufferMesh();

	const unsigned int getVaoID() { return vaoID; }
	const unsigned int getVertexID() { return vertexID; }
	const unsigned int getIndiciesID() { return indiciesID; }
	const unsigned int getNormalID() { return normalsID; }
	const unsigned int getNumInds() { return numInds; }
	const unsigned int getNumVerts() { return numVerts; }

private:
	unsigned int numInds, numVerts;
	
	Vec3f *verts;
	Vec3f *normals;
	unsigned int *inds;

	Vec3f colour;
	unsigned int vaoID;
	unsigned int vertexID;
	unsigned int normalsID;
	unsigned int indiciesID;
	
};

