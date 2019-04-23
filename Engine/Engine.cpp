#include "Engine.h"
#include "Graphics.h"
#include "Physics.h"

Graphics g;

void cleanUp() {
	//exit(0);
}

Engine::~Engine() {
	Physics::cleanUp();
}

void Engine::init() {
	g.initialize();
	Physics::initialize();
	g.setOnWindowClose(cleanUp);
}

Transform *Engine::addCube(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic) {
	Transform *cube = g.addCube(pos,scale,rot, dynamic);
	cube->setRigidBody(Physics::addOBB(pos,scale,rot, dynamic));
	return cube;
}

Transform * Engine::addCapsule(Vec3f pos, Vec3f scale, Quatf rot, bool dynamic)
{
	Transform *capsule = g.addCapsule(pos, scale, rot, dynamic);
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
	g.update();
	Physics::update(delta);
}
