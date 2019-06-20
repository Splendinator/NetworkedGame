#pragma once

class Transform;

namespace Networking {
	const int MAX_PLAYERS = 32;
	const int MAX_OBJECTS = 2048;

	static const int INPUT_BUFFER_AMMOUNT = 8;

	static const float NETWORK_UPDATE_DELTA = 1 / 64.f;
	static const float PHYSICS_UPDATE_DELTA = 1 / 64.f;

	static const float PLAYER_MOVE_SPEED = 0.1f;

	static const float ESTIMATE_FLOAT_LEEWAY = 0.001f;

	static const float INTERP_PCT = 0.05f;
	static const float INTERP_MIN_DISE = 0.1f;

	struct Player {
		Transform *transform;
		float yaw;
	};
}