#include "Messages.h"



using namespace Messages;

domnet::Message<PayloadPlayerPosition> *p_PlayerPosition;
domnet::Message<PayloadPlayerPosition> m_PlayerPosition(MT_PLAYER_POSITION);

namespace Messages {



	domnet::Message<PayloadPlayerPosition> *p_PlayerPosition() { return ::p_PlayerPosition; }
	domnet::Message<PayloadPlayerPosition> &m_PlayerPosition(){ return ::m_PlayerPosition; }

}