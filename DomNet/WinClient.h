#pragma once

#include "BaseClient.h"
#include "WinHost.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <thread>


namespace domnet {

	class WinClient : public BaseClient, public WinHost
	{
	public:
		WinClient(uint16_t portNumber = DEFAULT_PORT_SERV) : WinHost(portNumber) {};
		~WinClient();

		int connect(const Address &a);
		int sendMessage(BaseMessage *m, char protocal = DN_TCP);
		
		int listenFuncUDP();
		



	private:
	
		std::thread listenThread;
		unsigned short myPort;
		//Address address;

		SOCKADDR_IN serverAddress;

		void listenFunc();

	};

}