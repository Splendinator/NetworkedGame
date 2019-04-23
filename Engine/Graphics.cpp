#include "Graphics.h"
#include "Window.h"
#include <Windows.h>
#include "GL.h"
#include "Mesh.h"
#include "IO.h"
#include <iostream>
#include "Renderable.h"
#include "Math.h"
#include "Transform.h"

Camera camera;

Window w;
GLuint shaderVertex, shaderFragment;
GLuint programID;

GLuint VertexArrayID;
GLuint vertexbuffer;

Mesh *m = Mesh::genCube();
Mesh *meshCapsule = Mesh::genCapsule();

std::vector<Transform *> cubes;
std::vector<Transform *> staticCubes;
std::vector<Transform *> capsules;

const static Vec3f DYNAMIC_COLOUR = { 0,0.8f,0 };
const static Vec3f STATIC_COLOUR = { 0.8f,0.8f,0.8f };
const static Vec3f PLAYER_COLOUR = { 0.8f,0,0 };
const static Vec3f CLEAR_COLOUR = { 0.3f, 0.3f, 0.6f };


void initGL() {
	if (!GL::isInitialized()) {
		GL::init();
	}
}

void initShaders() {

	char buf[IO::MAX_READ_SIZE];
	char err[IO::MAX_READ_SIZE];
	const char *bufP = buf;

	GLint status;

	programID = glCreateProgram();
	shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	
	//Vertex Shader
	IO::fReadInto(buf, "BasicVert.glsl");
	glShaderSource(shaderVertex, 1, &bufP, NULL);
	glCompileShader(shaderVertex);
	glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(shaderVertex, IO::MAX_READ_SIZE, NULL, err);
		std::cout << "Error compiling vertex shader " <<  err << buf << "\n";
		programID = 0;
	}
	glAttachShader(programID, shaderVertex);

	//Fragment Shader
	IO::fReadInto(buf, "BasicFrag.glsl");
	glShaderSource(shaderFragment, 1, &bufP, NULL);
	glCompileShader(shaderFragment);
	glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(shaderFragment, IO::MAX_READ_SIZE, NULL, err);
		std::cout << "Error compiling fragment shader " << err << buf << "\n";
		programID = 0;
	}
	glAttachShader(programID, shaderFragment);

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetProgramInfoLog(programID, 8192, NULL, err);
		std::cout << "Error linking compiled shader: " << err;
		programID = 0;
	}

	glUseProgram(programID);

}

void initVAO() {
	m->bufferMesh();
	meshCapsule->bufferMesh();
}

Graphics::Graphics()
{

}

void passView() {
	Mat4f view = camera.buildViewMatrix();
	
	
	glUniformMatrix4fv(1, 1, false, (GLfloat *)&view);
}

void passProj(){

	static Mat4f proj = Math::buildPerspectiveMatrix(110, 1.77777777778f, 0.1f, 100);
	glUniformMatrix4fv(0, 1, false, (GLfloat *)&proj);

}

void setUpCubes() {
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m->getVertexID());
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized? 
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m->getNormalID());
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized? 
		0,                  // stride
		(void*)0            // array buffer offset
	);

}

void setUpCapsules() {

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, meshCapsule->getVertexID());
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized? 
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, meshCapsule->getNormalID());
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized? 
		0,                  // stride
		(void*)0            // array buffer offset
	);

}

void drawCubes() {

	glUniform3f(10, DYNAMIC_COLOUR[0], DYNAMIC_COLOUR[1], DYNAMIC_COLOUR[2]);

	for (auto cube : cubes) {
		cube->update();
		Mat4f model = cube->getModelMatrix();
		glUniformMatrix4fv(2, 1, false, (GLfloat *)&model);
		glDrawArrays(GL_TRIANGLES, 0, m->getNumVerts());
	}

	glUniform3f(10, STATIC_COLOUR[0], STATIC_COLOUR[1], STATIC_COLOUR[2]);

	for (auto cube : staticCubes) {
		cube->update();
		Mat4f model = cube->getModelMatrix();
		glUniformMatrix4fv(2, 1, false, (GLfloat *)&model);
		glDrawArrays(GL_TRIANGLES, 0, m->getNumVerts());
	}
	
}

void drawCapsules() {

	glUniform3f(10, PLAYER_COLOUR[0], PLAYER_COLOUR[1], PLAYER_COLOUR[2]);

	for (auto capsule : capsules) {
		capsule->update();
		Mat4f model = capsule->getModelMatrix();
		glUniformMatrix4fv(2, 1, false, (GLfloat *)&model);
		glDrawArrays(GL_TRIANGLES, 0, meshCapsule->getNumVerts());
	}

}

void Graphics::update() {
	passView();
	setUpCubes();
	drawCubes();
	setUpCapsules();
	drawCapsules();
	SwapBuffers(w.getContextDevice());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	w.updateInput();
}

bool Graphics::isHeld(int key)
{
	return w.isHeld(key);
}

bool Graphics::isPressed(int key)
{
	return w.isPressed(key);
}

int Graphics::getMouseXDelta()
{
	return w.getMouseXDelta();
}

int Graphics::getMouseYDelta()
{
	return w.getMouseYDelta();
}

void Graphics::setOnWindowClose(void(*func)())
{
	w.setOnWindowClose(func);
}

Camera * Graphics::getCamera()
{
	return &camera;
}

void Graphics::initialize() {
	w.initialize();

	while (w.isInitialized() == false) {}

	HGLRC contextOpengl = wglCreateContext(w.getContextDevice());
	wglMakeCurrent(w.getContextDevice(), contextOpengl);

	initGL();
	initShaders();
	initVAO();

	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(CLEAR_COLOUR[0], CLEAR_COLOUR[1], CLEAR_COLOUR[2],1);
	

	passProj();
	
	//*** TEMP ***
	camera.pos[2] = 3.f;
	camera.pos[0] += 2.0f;
	camera.yaw = -0.2f;
	camera.pitch = -0.4f;
	camera.pos[1] = 2.0f;

}

Transform *Graphics::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic)
{
	Transform *p = new Transform();
	p->_pos = pos;
	p->_scale = scale;
	p->_rot = rot;
	(dynamic ? cubes : staticCubes).push_back(p);
	return p;
}

Transform * Graphics::addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic)
{
	Transform *p = new Transform();
	p->_pos = pos;
	p->_scale = scale;
	p->_rot = rot;
	capsules.push_back(p);
	return p;
}



Graphics::~Graphics()
{
	for (auto i : cubes) {
		delete i;
	}
	for (auto i : capsules) {
		delete i;
	}

}

