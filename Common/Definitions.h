#pragma once

class Transform;

namespace Networking {
	const int MAX_PLAYERS = 32;
	const int MAX_OBJECTS = 2048;

	static const float NETWORK_UPDATE_DELTA = 1 / 32.f;
	static const float PHYSICS_UPDATE_DELTA = 1 / 64.f;

	static const float PLAYER_MOVE_SPEED = 0.1f;

	struct Player {
		Transform *transform;
		float yaw;
	};
}