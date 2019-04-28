#include "../Engine/Quaternion.h"
#include "Message.h"


namespace Messages {

	enum MESSAGETYPES {
		MT_PLAYER_POSITION,

	};

	//PlayerPosition
	struct PayloadPlayerPosition {
		int id;
		Vec3f pos;
	};
	domnet::Message<PayloadPlayerPosition> *p_PlayerPosition();
	domnet::Message<PayloadPlayerPosition> &m_PlayerPosition();
	
	//MessageTwo


}