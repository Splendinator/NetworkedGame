#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	delete[] inds;
	delete[] verts;
}

Mesh * Mesh::genCube()
{
	Mesh *m = new Mesh;

	m->numVerts = 8;
	m->numInds = 36;

	m->verts = new vec3f[m->numVerts];
	m->inds = new int[m->numInds];

	m->verts[0] = {-1.f,  1.f,  1.f};
	m->verts[1] = { 1.f,  1.f,  1.f};
	m->verts[2] = {-1.f, -1.f,  1.f};
	m->verts[3] = { 1.f, -1.f,  1.f};
	m->verts[4] = { 1.f,  1.f, -1.f};
	m->verts[5] = {-1.f,  1.f, -1.f};
	m->verts[6] = { 1.f, -1.f, -1.f};
	m->verts[7] = {-1.f, -1.f, -1.f};

	m->inds[0 ] = 1;
	m->inds[1 ] = 0;
	m->inds[2 ] = 2;
	m->inds[3 ] = 3;
	m->inds[4 ] = 1;
	m->inds[5 ] = 2;

	m->inds[6 ] = 0;
	m->inds[7 ] = 0;
	m->inds[8 ] = 0;
	m->inds[9 ] = 0;
	m->inds[10] = 0;
	m->inds[11] = 0;
	m->inds[12] = 0;
	m->inds[13] = 0;
	m->inds[14] = 0;
	m->inds[15] = 0;
	m->inds[16] = 0;
	m->inds[17] = 0;
	m->inds[18] = 0;
	m->inds[19] = 0;
	m->inds[20] = 0;
	m->inds[21] = 0;
	m->inds[22] = 0;
	m->inds[23] = 0;
	m->inds[24] = 0;
	m->inds[25] = 0;
	m->inds[26] = 0;
	m->inds[27] = 0;
	m->inds[28] = 0;
	m->inds[29] = 0;
	m->inds[30] = 0;
	m->inds[31] = 0;
	m->inds[32] = 0;
	m->inds[33] = 0;
	m->inds[34] = 0;
	m->inds[35] = 0;


	

	return m;
}
