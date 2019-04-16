#include "Engine.h"
#include "Graphics.h"
#include "Physics.h"

Graphics g;

Engine::~Engine() {
	Physics::cleanUp();
}

void Engine::init() {
	g.initialize();
	Physics::initialize();
}

Transform *Engine::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic) {
	Transform *cube = g.addCube(pos,scale,rot, dynamic);
	cube->setRigidBody(Physics::addOBB(pos,scale,rot, dynamic));
	return cube;
}

void Engine::update(float delta) {
	g.update();
	Physics::update(delta);
}
