#include "Engine.h"
#include "Graphics.h"
#include "Physics.h"

Graphics g;

namespace Engine {

	void init() {
		g.initialize();
		Physics::initialize();
	}

	Transform *addCube() {
		Transform *cube = g.addCube();
		cube->setRigidBody(Physics::addOBB());
		return cube;
	}

	void update(float delta) {
		g.update();
		Physics::update(delta);
	}
}