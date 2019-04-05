

#include "BaseHost.h"
#include "MessageQueue.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>


namespace domnet {

#pragma once
	class WinHost : public BaseHost
	{
	public:
		WinHost(uint16_t portNumber = DEFAULT_PORT_SERV);
		~WinHost();

		
		BaseMessage *getMessageUDP() { return messageQueueUDP.getMessage(); };


	protected:
		SOCKET ListenSocket;
		SOCKET ListenSocketUDP;
		SOCKET SendingSocketUDP;
		
		uint16_t port;

		
	
		MessageQueue messageQueueUDP;
		bool useBufOneUDP;
		std::thread threadUDP;
		char bufOneUDP[BUF_SIZE * SERVER_MAX_CLIENTS];
		char bufTwoUDP[BUF_SIZE * SERVER_MAX_CLIENTS];

		

	};
	
}