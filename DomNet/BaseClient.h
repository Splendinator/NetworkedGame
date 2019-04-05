#pragma once

#include "Interfaces\BaseNetwork.h"
#include "BaseHost.h"
#include "MessageQueue.h"

namespace domnet {






	class BaseClient
	{
	public:

		virtual int connect(const Address &a) = 0;
		virtual int sendMessage(BaseMessage *m, char protocal) = 0;
		
		BaseMessage *getMessage();

	protected:
		MessageQueue mq;

		bool useBufOne = true;
		char bufOne[BUF_SIZE]{};
		char bufTwo[BUF_SIZE]{};
	};

	

}
