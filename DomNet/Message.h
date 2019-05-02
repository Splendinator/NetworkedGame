#pragma once
#include <stddef.h>

namespace domnet {

	
	struct BaseMessage {

		BaseMessage(short messageType = 0) { 
			type = messageType;
			size = sizeof(BaseMessage);
		}


		void setPayloadSize(unsigned int x) { size = x + sizeof(BaseMessage); }
		unsigned int getSize() const { return size; }
		



		//Message type (CONNECT,DISCONNECT,...)
		short type;

		//ID of the sender (used by server - UDP)
		short id;	

	protected:
		//sizeof(message), *** TAKE CARE WHEN MANUALLY CHANGING ***
		short size;	//These values likely not needed and can be derrived.
		short padding;

		friend class MessageQueue;

	};


	template <class T>
	struct Message : BaseMessage {

		T payload;

		
		Message(short messageType = 0) {
			size = sizeof(*this);
			type = messageType;
		}


	};
}