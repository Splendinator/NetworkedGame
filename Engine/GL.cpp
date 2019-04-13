#include "GL.h"

bool initialized = false;

int GL::init()
{
	if (initialized) return -1;
	int ret = glewInit();
	if (ret == GLEW_OK) initialized = true;
	return ret; 
		
}

bool GL::isInitialized()
{
	return initialized;
}
