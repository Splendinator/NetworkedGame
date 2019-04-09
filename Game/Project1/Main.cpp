#include <iostream>
#include "PxPhysicsAPI.h"

using namespace physx;
using namespace std;


int main() {


	PxTolerancesScale scale;
	scale.length = 100;        // typical length of an object
	scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

	cout << scale.isValid();



	getchar();

	return 0;
}