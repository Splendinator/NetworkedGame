#include "Server.h"

using namespace domnet;

Server::Server(uint16_t portNumber) : WinHost(portNumber)
{
	for (int i = 0; i < SERVER_MAX_CLIENTS; ++i) {
		clients[i] = INVALID_SOCKET;
	}


}


Server::~Server()
{
}


#include <stdio.h>

int Server::host(int numClients)
{

	int err;
	struct addrinfo *result = NULL, *ptr = NULL, hints;


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	char servPort[6];

	sprintf_s(servPort, "%d", port);

	// Resolve the local address and port to be used by the server
	err = getaddrinfo(NULL, servPort, &hints, &result);
	if (err != 0) {
		WSACleanup();
		return ERR_HOST;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return ERR_HOST;
	}

	err = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	
	if (err == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return ERR_HOST;
	}

	freeaddrinfo(result);


	

	printf("Listening...\n");

	//*** LISTENING ***
	err = listen(ListenSocket, SOMAXCONN);
	

	if (err == SOCKET_ERROR) {
		closesocket(ListenSocket);
		WSACleanup();
		return ERR_HOST;
	}


	int x = sizeof(clientAddress[0]);
	
	for (int i = 0; i < numClients; ++i) {

		clients[i] = accept(ListenSocket, (sockaddr *)&clientAddress[i], &x);
		if (clients[i] == INVALID_SOCKET) {
			std::cout << WSAGetLastError() << std::endl;
			closesocket(clients[0]);
			WSACleanup();
			return ERR_HOST;
		}

		send(clients[i], (const char *)&clientPort, sizeof(clientPort),NULL);
		++clientPort;
		t[i] = std::thread(&Server::listenFunc, this, i);
		t[i].detach();
	} 
	
	closesocket(ListenSocket);




	//UDP SETUP
	SendingSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//std::cout << "UDP Sending Socket: " << SendingSocketUDP << std::endl;

	SOCKADDR_IN receiverAddr;

	ListenSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//std::cout << "UDP Listen Socket: " << ListenSocketUDP << std::endl;



	ZeroMemory(&receiverAddr, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	receiverAddr.sin_port = htons(port);
	receiverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	

	err = bind(ListenSocketUDP, (SOCKADDR *)&receiverAddr, sizeof(receiverAddr));
	//std::cout << "UDP Socket Bind: " << err << std::endl;


	threadUDP = std::thread(&Server::listenFuncUDP, this);
	threadUDP.detach();

	//********


	





	printf("Connected!\n");



	

	return 0;
}

int domnet::Server::broadcast(BaseMessage *m, char protocal)
{

	
	int err;
	SOCKADDR_IN s;
	s.sin_addr.S_un.S_addr = clientAddress->sin_addr.S_un.S_addr;
	s.sin_family = AF_INET;
	
	switch (protocal) {
	case(DN_TCP):
		for (int i = 0; i < SERVER_MAX_CLIENTS; ++i) {
			if (clients[i] != INVALID_SOCKET) {
				err = send(clients[i], (char *)m, m->getSize(), 0);
				if (err == SOCKET_ERROR) {
					closesocket(clients[i]);
					WSACleanup();
					return 1;
				}
			}
		}
		break;
	case(DN_UDP):
		for (int i = 0; i < SERVER_MAX_CLIENTS; ++i) {
			if (clients[i] != INVALID_SOCKET) {
				
		
				s.sin_port = htons(DEFAULT_PORT_CLIENT + i);
				err = sendto(SendingSocketUDP, (char *)m, m->getSize(), 0, (sockaddr *)&s, sizeof(s));
				
				if (err == SOCKET_ERROR) {

					std::cout << WSAGetLastError() << std::endl;
					//closesocket(clients[i]);
					//WSACleanup();
					return 1;
				}
			}
		}
	

	
	}


	return 0;
}

int domnet::Server::sendMessage(BaseMessage *m, int index, char protocal)
{
	SOCKADDR_IN s;
	if (protocal == DN_TCP) {
		if (clients[index] != INVALID_SOCKET) {
			int err = send(clients[index], (char *)m, m->getSize(), 0);
			if (err == SOCKET_ERROR) {
				closesocket(clients[index]);
				WSACleanup();
				return 1;
			}
		}
	}
	else if(protocal == DN_UDP) {

		s.sin_family = AF_INET;
		s.sin_port = htons(DEFAULT_PORT_CLIENT + index);
		s.sin_addr = clientAddress[index].sin_addr;
		sendto(SendingSocketUDP, (const char*)m, m->getSize(), NULL, (sockaddr *)&s, sizeof(s));
	}
	return 0;
}

BaseMessage * domnet::Server::getMessage(unsigned char index)
{
	return mq[index].getMessage();
}

BaseMessage * domnet::Server::getMessageUDP(unsigned char &index)
{
	BaseMessage *b = WinHost::getMessageUDP();
	if(b) index = b->id - DEFAULT_PORT_CLIENT;
	return b;
}

void domnet::Server::listenFunc(unsigned char index)
{

	
	int err;
	char *buf = (useBufOne[index] ? bufOne : bufTwo) + BUF_SIZE * index;
	for (;;) {
		while (!mq[index].isEmpty()) {}	//TODO: Cosider mutex as a safer option. (slower?)receiverAddr
		
		
		err = recv(clients[index], buf, BUF_SIZE, 0);
		

		if (err == SOCKET_ERROR) {
			printf("Server listenFunc error: %d - %d\n",index, WSAGetLastError());
			//continue;	//TODO: Temporary solution, calling recV on a socket that is INVALID_SOCKET causes SOCKET_ERROR. Should disconnect client.
			exit(1);
		}

		mq[index].addMessagesToQueue(buf, err);
		useBufOne[index] = !useBufOne[index];

	}

}



int domnet::Server::listenFuncUDP()
{
	int err;
	sockaddr_in s;
	int len = sizeof(s);

	for (;;) {

		//printf("Thread Listening\n");

		while (!messageQueueUDP.isEmpty()) {}	//TODO: Cosider mutex as a safer option. (slower?)

		err = recv(ListenSocketUDP, (useBufOneUDP ? bufOneUDP : bufTwoUDP), BUF_SIZE, 0);

		//printf("%d bytes of data into buffer %d\n", err, (useBufOne ? 1 : 2));

		if (err < 0) {
			std::cout << "ERR";
			std::cout << WSAGetLastError() << std::endl;
			continue;
		}


		messageQueueUDP.addMessagesToQueue((useBufOneUDP ? bufOneUDP : bufTwoUDP), err);

		useBufOneUDP = !useBufOneUDP;
	}

}
