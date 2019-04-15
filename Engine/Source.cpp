#include <iostream>
#include "Vector.h"
#include "Engine.h"


int main() {

	Engine::init();
	Engine::addCube();
	Engine::update(0.01f);


	

	getchar();
}