#include <iostream>
#include "Quaternion.h"
#include "Engine.h"
#include "Math.h"


int main() {

	Engine e;
	e.init();
	e.addCube({ 0.5,4,0 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCube({ -0.5,7,0.5 }, { 1,1,1 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f));
	e.addCapsule({ 0,1,0 }, { 1,1,1 }, Quatf::quatFromEuler({0,0,1},(Math::PI + 0.02f)/2.f));
	e.addCube({ 0,-3,0 }, { 12,1,12 }, Quatf::quatFromEuler({ 0,1,0 }, 0.f), false);
	


	for (;;) {
		if (e.isHeld('D')) 
		{
			e.getCamera()->pos += e.getCamera()->right() * 0.02f;
		}
		if (e.isHeld('A'))
		{
			e.getCamera()->pos -= e.getCamera()->right() * 0.02f;
		}
		if (e.isHeld('W'))
		{
			e.getCamera()->pos += e.getCamera()->foward() * 0.02f;
		}										 
		if (e.isHeld('S'))						 
		{										 
			e.getCamera()->pos -= e.getCamera()->foward() * 0.02f;
		}
		if (e.isHeld(' '))
		{
			e.getCamera()->pos += e.getCamera()->up() * 0.02f;
		}
		if (e.isHeld(0x11))
		{
			e.getCamera()->pos -= e.getCamera()->up() * 0.02f;
		}
		//std::cout << e.getMouseXDelta() << '\n';
		e.getCamera()->yaw += e.getMouseXDelta() / 200.f;
		e.getCamera()->pitch += e.getMouseYDelta() / 200.f;
		if (e.getCamera()->pitch > 1.5f) e.getCamera()->pitch = 1.5f;
		if (e.getCamera()->pitch < -1.5f) e.getCamera()->pitch = -1.5f;
		e.update(0.004f);
	}


	getchar();
	return 0;
}