#include "Engine.h"
#include "Graphics.h"
#include "Physics.h"
#include <vector>

Graphics g;
std::vector<Transform *> noDraw;

void cleanUp() {
	//exit(0);
}

void updateTransforms() {
	for (Transform *t : noDraw) {
		t->update();
	}
}

Engine::~Engine() {
	Physics::cleanUp();
}

void Engine::init() {
	g.initialize();
	Physics::initialize();
	g.setOnWindowClose(cleanUp);
}

Transform *Engine::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, bool draw) {
	Transform *cube = g.addCube(pos,scale,rot, dynamic);
	cube->setRigidBody(Physics::addOBB(pos,scale,rot, dynamic));
	return cube;
}



Transform * Engine::addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic, bool draw)
{
	Transform *capsule;
	if (draw) {
		capsule = g.addCapsule(pos, scale, rot, dynamic);
	}
	else {
		capsule = new Transform(dynamic);
		noDraw.push_back(capsule);
	}
	capsule->setRigidBody(Physics::addCapsule(pos, scale, rot, dynamic));
	return capsule;
}

bool Engine::isHeld(int key)
{
	return g.isHeld(key);
}

bool Engine::isPressed(int key)
{
	return g.isPressed(key);
}

int Engine::getMouseXDelta()
{
	return g.getMouseXDelta();
}

int Engine::getMouseYDelta()
{
	return g.getMouseYDelta();
}

Camera * Engine::getCamera()
{
	return g.getCamera();
}

void Engine::update(float delta) {
	updateTransforms();
	g.update();
	Physics::update(delta);
	
}
