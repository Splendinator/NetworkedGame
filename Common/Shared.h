#include "Definitions.h"

class Transform;

namespace shared {

	Transform *getDynamic(int id);
	Networking::Player &getPlayer(int id);

	void setDynamic(Transform *t, int id);
	void setPlayer( Transform *t, int id, float yaw = -1.f);

	Networking::Player &getCurrPlayer();
	int getCurrPlayerId();
	void setCurrPlayer(int id);

	void setPlayersUpright();
}