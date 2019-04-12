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

	//m->verts[0] = (-1.f,  1.f,  1.f);
	//m->verts[1] = ( 1.f,  1.f,  1.f);
	//m->verts[2] = (-1.f, -1.f,  1.f);
	//m->verts[3] = ( 1.f, -1.f,  1.f);
	//m->verts[4] = ( 1.f,  1.f, -1.f);
	//m->verts[5] = (-1.f,  1.f, -1.f);
	//m->verts[6] = ( 1.f, -1.f, -1.f);
	//m->verts[7] = (-1.f, -1.f, -1.f);

	return m;
}
