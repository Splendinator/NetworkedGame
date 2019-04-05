#pragma once

#include "Interfaces\BaseNetwork.h"
#include "WinHost.h"




#include <WinSock2.h>
#include <ws2tcpip.h>

namespace domnet {

	class Server : public WinHost
	{
	public:
		Server(uint16_t portNumber = DEFAULT_PORT_SERV);
		~Server();

		int host(int numClients = 1);
		int broadcast(BaseMessage *m, char protocal = DN_TCP);
		int sendMessage(BaseMessage *m, int index, char protocal = DN_TCP);

		BaseMessage *getMessage(unsigned char index);
		BaseMessage *getMessageUDP(unsigned char &index); //Index to be filled with the client's ID
	private:

		SOCKET clients[SERVER_MAX_CLIENTS];
		sockaddr_in clientAddress[SERVER_MAX_CLIENTS];
		int addrLen[SERVER_MAX_CLIENTS];
		SOCKET clientsUDP[SERVER_MAX_CLIENTS];

		MessageQueue mq[SERVER_MAX_CLIENTS];
		MessageQueue mqUDP[SERVER_MAX_CLIENTS];

		std::thread t[SERVER_MAX_CLIENTS];
		std::thread tUDP[SERVER_MAX_CLIENTS];

		void listenFunc(unsigned char index);
		int listenFuncUDP();
		//void listenFuncUDP(unsigned char index);


		bool useBufOne[SERVER_MAX_CLIENTS];
		char bufOne[BUF_SIZE * SERVER_MAX_CLIENTS];
		char bufTwo[BUF_SIZE * SERVER_MAX_CLIENTS];

		//bool useBufOneUDP[SERVER_MAX_CLIENTS];
		//char bufOneUDP[BUF_SIZE * SERVER_MAX_CLIENTS];
		//char bufTwoUDP[BUF_SIZE * SERVER_MAX_CLIENTS];

		unsigned short clientPort = DEFAULT_PORT_CLIENT;

		

	};

}

