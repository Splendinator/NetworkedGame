#include <iostream>
#include "Quaternion.h"
#include "Engine.h"


int main() {

	Engine e;
	e.init();
	e.addCube({ 0,2,0 }, { 1,1,1 }, Quatf::quatFromEuler({0,1,0},0.f));
	e.addCube({ 0,-3,0 }, { 4,1,4 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	for (int i = 0; i < 10000; ++i) {
		e.update(0.001f);
	}


	getchar();
	return 0;
}