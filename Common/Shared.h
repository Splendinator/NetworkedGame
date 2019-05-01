class Transform;

namespace shared {

	Transform *getDynamic(int id);
	Transform *getPlayer(int id);

	void setDynamic(Transform *t, int id);
	void setPlayer( Transform *t, int id);

	Transform *getCurrPlayer();
	void setCurrPlayer(Transform *t);
}