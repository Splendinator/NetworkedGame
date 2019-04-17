#include <iostream>
#include "Quaternion.h"
#include "Engine.h"


int main() {

	Engine e;
	e.init();
	e.addCube({ 0,2,0 }, { 1,1,1 }, Quatf::quatFromEuler({0,1,0},0.f));
	e.addCube({ 0,-3,0 }, { 4,1,4 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	for (int i = 0; i < 10000; ++i) {
		if (e.isHeld('D')) 
		{
			e.getCamera()->pos += e.getCamera()->right() * 0.01f;
		}
		if (e.isHeld('A'))
		{
			e.getCamera()->pos -= e.getCamera()->right() * 0.01f;
		}
		if (e.isHeld('W'))
		{
			e.getCamera()->pos -= e.getCamera()->foward() * 0.01f;
		}										 
		if (e.isHeld('S'))						 
		{										 
			e.getCamera()->pos += e.getCamera()->foward() * 0.01f;
		}
		if (e.isHeld(' '))
		{
			e.getCamera()->pos += e.getCamera()->up() * 0.01f;
		}
		if (e.isHeld(0x11))
		{
			e.getCamera()->pos -= e.getCamera()->up() * 0.01f;
		}
		e.update(0.001f);
	}


	getchar();
	return 0;
}