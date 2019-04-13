#include "Graphics.h"
#include "Window.h"
#include <Windows.h>
#include "GL.h"
#include "Mesh.h"
#include "IO.h"
#include <iostream>

Window w;
GLuint shaderVertex, shaderFragment;
GLuint programID;

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

}

Graphics::Graphics()
{

}

void tempFunc() {
	Mesh *m = Mesh::genCube();

	while (true) {

		glClearColor(rand() & 1000 / 1000, rand() & 1000 / 1000, rand() & 1000 / 1000, 1);

		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(w.getContextDevice());
	}
}

void Graphics::initialize() {
	w.initialize();

	while (w.isInitialized() == false) {}

	HGLRC contextOpengl = wglCreateContext(w.getContextDevice());
	wglMakeCurrent(w.getContextDevice(), contextOpengl);

	initGL();
	initShaders();

	tempFunc();
}

Graphics::~Graphics()
{

}

