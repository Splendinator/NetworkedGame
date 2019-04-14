#include "Mesh.h"
#include "GL.h"



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

	m->numVerts = 36;

	m->verts = new vec3f[m->numVerts];
	m->inds = nullptr;
	m->normals = new vec3f[m->numVerts];

	m->verts[ 0] = { 1.f, 1.f, 1.f};
	m->verts[ 1] = {-1.f, 1.f, 1.f};
	m->verts[ 2] = {-1.f, -1.f, 1.f};
	m->verts[ 3] = { 1.f, -1.f, 1.f};
	m->verts[ 4] = { 1.f, 1.f, 1.f};
	m->verts[ 5] = {-1.f, -1.f, 1.f}; 
	m->verts[ 6] = { 1.f, -1.f, 1.f};
	m->verts[ 7] = { 1.f, 1.f, 1.f};
	m->verts[ 8] = { 1.f, 1.f, -1.f};
	m->verts[ 9] = { 1.f, -1.f, -1.f};
	m->verts[10] = { 1.f, -1.f, 1.f};
	m->verts[11] = { 1.f, 1.f, -1.f};
	m->verts[12] = {-1.f, 1.f, -1.f};
	m->verts[13] = { 1.f, 1.f, -1.f};
	m->verts[14] = { 1.f, -1.f, -1.f};
	m->verts[15] = {-1.f, -1.f, -1.f};
	m->verts[16] = {-1.f, 1.f, -1.f};
	m->verts[17] = { 1.f, -1.f, -1.f};
	m->verts[18] = {-1.f, 1.f, 1.f};
	m->verts[19] = {-1.f, 1.f, -1.f};
	m->verts[20] = {-1.f, -1.f, -1.f};
	m->verts[21] = {-1.f, -1.f, 1.f};
	m->verts[22] = {-1.f, 1.f, 1.f};
	m->verts[23] = {-1.f, -1.f, -1.f};
	m->verts[24] = {-1.f, 1.f, -1.f};
	m->verts[25] = { 1.f, 1.f, -1.f};
	m->verts[26] = {-1.f, 1.f, 1.f};
	m->verts[27] = { 1.f, 1.f, 1.f};
	m->verts[28] = {-1.f, 1.f, -1.f};
	m->verts[29] = {-1.f, 1.f, 1.f};
	m->verts[30] = { 1.f, -1.f, 1.f};
	m->verts[31] = {-1.f, -1.f, 1.f};
	m->verts[32] = { 1.f, -1.f, -1.f};
	m->verts[33] = {-1.f, -1.f, -1.f};
	m->verts[34] = { 1.f, -1.f, 1.f};
	m->verts[35] = { 1.f, -1.f, -1.f};

	m->normals[0]	= {0.0f,0.0f,1.0f};
	m->normals[1]	= {0.0f,0.0f,1.0f};
	m->normals[2]	= {0.0f,0.0f,1.0f};
	m->normals[3]	= {0.0f,0.0f,1.0f};
	m->normals[4]	= {0.0f,0.0f,1.0f};
	m->normals[5]	= {0.0f,0.0f,1.0f};
	m->normals[6]	= {1.0f,0.0f,0.0f};
	m->normals[7]	= {1.0f,0.0f,0.0f};
	m->normals[8]	= {1.0f,0.0f,0.0f};
	m->normals[9]	= {1.0f,0.0f,0.0f};
	m->normals[10]	= {1.0f,0.0f,0.0f};
	m->normals[11]	= {1.0f,0.0f,0.0f};
	m->normals[12]	= {0.0f,0.0f,-1.0f};
	m->normals[13]	= {0.0f,0.0f,-1.0f};
	m->normals[14]	= {0.0f,0.0f,-1.0f};
	m->normals[15]	= {0.0f,0.0f,-1.0f};
	m->normals[16]	= {0.0f,0.0f,-1.0f};
	m->normals[17]	= {0.0f,0.0f,-1.0f};
	m->normals[18]	= {-1.0f,0.0f,0.0f};
	m->normals[19]	= {-1.0f,0.0f,0.0f};
	m->normals[20]	= {-1.0f,0.0f,0.0f};
	m->normals[21]	= {-1.0f,0.0f,0.0f};
	m->normals[22]	= {-1.0f,0.0f,0.0f};
	m->normals[23]	= {-1.0f,0.0f,0.0f};
	m->normals[24]	= {0.0f,1.0f,0.0f};
	m->normals[25]	= {0.0f,1.0f,0.0f};
	m->normals[26]	= {0.0f,1.0f,0.0f};
	m->normals[27]	= {0.0f,1.0f,0.0f};
	m->normals[28]	= {0.0f,1.0f,0.0f};
	m->normals[29]	= {0.0f,1.0f,0.0f};
	m->normals[30]	= {0.0f,-1.0f,0.0f};
	m->normals[31]	= {0.0f,-1.0f,0.0f};
	m->normals[32]	= {0.0f,-1.0f,0.0f};
	m->normals[33]	= {0.0f,-1.0f,0.0f};
	m->normals[34]	= {0.0f,-1.0f,0.0f};
	m->normals[35]	= {0.0f,-1.0f,0.0f};

	return m;
}

void Mesh::bufferMesh()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(vec3f), verts, GL_STATIC_DRAW);

	if (inds) {
		glGenBuffers(1, &indiciesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned int), inds, GL_STATIC_DRAW);
	}
	
	if (normals) {
		glGenBuffers(1, &normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, normalsID);
		glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(vec3f), normals, GL_STATIC_DRAW);
	}
}
