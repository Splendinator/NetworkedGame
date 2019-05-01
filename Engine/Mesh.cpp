#include "Mesh.h"
#include "GL.h"
#include "Math.h"
#include "Quaternion.h";



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

	m->verts = new Vec3f[m->numVerts];
	m->inds = nullptr;
	m->normals = new Vec3f[m->numVerts];

	m->verts[0] = { -1.0, -1.0,  1.0 };
	m->verts[1] = { 1.f, -1.f,  1.f };
	m->verts[2] = { 1.0,  1.0,  1.0 };
	m->verts[3] = { 1.0,  1.0,  1.0 };
	m->verts[4] = { -1.0,  1.0,  1.0 };
	m->verts[5] = { -1.0, -1.0,  1.0 };
	m->verts[6] = { 1.f, -1.f,  1.f };
	m->verts[7] = { 1.0, -1.0, -1.0 };
	m->verts[8] = { 1.0,  1.0, -1.0 };
	m->verts[9] = { 1.0,  1.0, -1.0 };
	m->verts[10] = { 1.0,  1.0,  1.0 };
	m->verts[11] = { 1.f, -1.f,  1.f };
	m->verts[12] = { -1.0,  1.0, -1.0 };
	m->verts[13] = { 1.0,  1.0, -1.0 };
	m->verts[14] = { 1.0, -1.0, -1.0 };
	m->verts[15] = { 1.0, -1.0, -1.0 };
	m->verts[16] = { -1.0, -1.0, -1.0 };
	m->verts[17] = { -1.0,  1.0, -1.0 };
	m->verts[18] = { -1.0, -1.0, -1.0 };
	m->verts[19] = { -1.0, -1.0,  1.0 };
	m->verts[20] = { -1.0,  1.0,  1.0 };
	m->verts[21] = { -1.0,  1.0,  1.0 };
	m->verts[22] = { -1.0,  1.0, -1.0 };
	m->verts[23] = { -1.0, -1.0, -1.0 };
	m->verts[24] = { -1.0, -1.0, -1.0 };
	m->verts[25] = { 1.0, -1.0, -1.0 };
	m->verts[26] = { 1.f, -1.f,  1.f };
	m->verts[27] = { 1.f, -1.f,  1.f };
	m->verts[28] = { -1.0, -1.0,  1.0 };
	m->verts[29] = { -1.0, -1.0, -1.0 };
	m->verts[30] = { -1.0,  1.0,  1.0 };
	m->verts[31] = { 1.0,  1.0,  1.0 };
	m->verts[32] = { 1.0,  1.0, -1.0 };
	m->verts[33] = { 1.0,  1.0, -1.0 };
	m->verts[34] = { -1.0,  1.0, -1.0 };
	m->verts[35] = { -1.0,  1.0,  1.0 };

	

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


#include <iostream>
const int CAPSULE_NUM_STRIPS = 16;
const int CAPSULE_SPHERE_ROWS = 8;
Mesh * Mesh::genCapsule(float height, float rad)
{
	Mesh *m = new Mesh;

	m->numVerts = CAPSULE_SPHERE_ROWS * 2 * 6 * CAPSULE_NUM_STRIPS;

	m->verts = new Vec3f[m->numVerts];
	m->inds = nullptr;
	m->normals = new Vec3f[m->numVerts];

	//x^2 + y^2 + z^2 = 1
	
	float topHeight = height + rad;
	float angle = 2 * Math::PI / CAPSULE_NUM_STRIPS;
	Vec4f v1 = {1,0,0,1};
	Vec4f v2;

	float fullX = Math::PI / float(2 * CAPSULE_SPHERE_ROWS);
	float fullY = 2 * Math::PI / float(CAPSULE_NUM_STRIPS);

	Mat4f rotateX = Math::buildRotateMatrixY(fullX);
	Mat4f rotateY = Math::buildRotateMatrixX(fullY);
	
	v2 = rotateX * v1;

	//std::cout << v2[0] << " " << v2[1] << " " << v2[2] << '\n';

	//Top of sphere
	for (int i = 0; i < CAPSULE_NUM_STRIPS; ++i) {
		m->normals[3 * i] = { v1[0],v1[1],v1[2] };
		m->verts[3 * i] = { v1[0] + height,v1[1],v1[2] };
		m->normals[3 * i + 1] = { v2[0],v2[1],v2[2] };
		m->verts[3 * i + 1] = { v2[0] + +height,v2[1],v2[2] };
		v2 = rotateY * v2;
		m->normals[3 * i + 2] = { v2[0],v2[1],v2[2] };
		m->verts[3 * i + 2] = { v2[0] + +height,v2[1],v2[2] };
	}

	int offset = 3 * CAPSULE_NUM_STRIPS;

	//Rest of sphere
	for (int i = 0; i < CAPSULE_SPHERE_ROWS-1; ++i) {
		v1 = v2;
		v2 = rotateX * v2;
		for (int j = 0; j < CAPSULE_NUM_STRIPS; ++j) {
			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0] + height,v1[1],v1[2] };
			
			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] + height,v2[1],v2[2] };

			v2 = rotateY * v2;				   
			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] + height,v2[1],v2[2] };
											
			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0] + height,v1[1],v1[2] };

			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] + height,v2[1],v2[2] };
			v1 = rotateY * v1;				   
			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0]+ height,v1[1],v1[2] };

		}
	}
	
	//Capsule Body
	v1 = v2;
	v1[0] += height;
	v2[0] -= height;
	for (int j = 0; j < CAPSULE_NUM_STRIPS; ++j) {
		m->normals[offset] = { 0,v1[1],v1[2] };
		m->verts[offset++] = { v1[0],v1[1],v1[2] };

		m->normals[offset] = { 0,v2[1],v2[2] };
		m->verts[offset++] = { v2[0],v2[1],v2[2] };
		v2 = rotateY * v2;

		m->normals[offset] = { 0,v2[1],v2[2] };
		m->verts[offset++] = { v2[0],v2[1],v2[2] };

		m->normals[offset] = { 0,v1[1],v1[2] };
		m->verts[offset++] = { v1[0],v1[1],v1[2] };

		m->normals[offset] = { 0,v2[1],v2[2] };
		m->verts[offset++] = { v2[0],v2[1],v2[2] };
		v1 = rotateY * v1;
		m->normals[offset] = { 0,v1[1],v1[2] };
		m->verts[offset++] = { v1[0],v1[1],v1[2] };
	}

	v1 = { -1,0,0,1 };
	v2 = rotateX * v1;

	//Top of sphere
	for (int i = 0; i < CAPSULE_NUM_STRIPS; ++i) {
		m->normals[offset] = { v1[0],v1[1],v1[2] };
		m->verts[offset++] = { v1[0] - height,v1[1],v1[2] };
		m->normals[offset] = { v2[0],v2[1],v2[2] };
		m->verts[offset++] = { v2[0] - height,v2[1],v2[2] };
		v2 = rotateY * v2;
		m->normals[offset] = { v2[0],v2[1],v2[2] };
		m->verts[offset++] = { v2[0] - height,v2[1],v2[2] };
	}

	//Rest of sphere
	for (int i = 0; i < CAPSULE_SPHERE_ROWS - 1; ++i) {
		v1 = v2;
		v2 = rotateX * v2;
		for (int j = 0; j < CAPSULE_NUM_STRIPS; ++j) {
			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0] - height,v1[1],v1[2] };

			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] - height,v2[1],v2[2] };

			v2 = rotateY * v2;
			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] - height,v2[1],v2[2] };

			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0] - height,v1[1],v1[2] };

			m->normals[offset] = { v2[0],v2[1],v2[2] };
			m->verts[offset++] = { v2[0] - height,v2[1],v2[2] };
			v1 = rotateY * v1;
			m->normals[offset] = { v1[0],v1[1],v1[2] };
			m->verts[offset++] = { v1[0] - height,v1[1],v1[2] };

		}
	}






	return m;
}

void Mesh::bufferMesh()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vec3f), verts, GL_STATIC_DRAW);

	if (inds) {
		glGenBuffers(1, &indiciesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInds * sizeof(unsigned int), inds, GL_STATIC_DRAW);
	}
	
	if (normals) {
		glGenBuffers(1, &normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, normalsID);
		glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vec3f), normals, GL_STATIC_DRAW);
	}
}
