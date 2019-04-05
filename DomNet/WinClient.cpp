#include "WinClient.h"






domnet::WinClient::~WinClient()
{
}

int domnet::WinClient::connect(const Address &a)
{
	int err;

	//address = a;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(a.port);
	inet_pton(AF_INET, a.hostname, &serverAddress.sin_addr.S_un.S_addr);


	struct addrinfo
		*result = NULL,
		*ptr = NULL,

		hintsTCP,
		hintsUDP;

	ZeroMemory(&hintsTCP, sizeof(hintsTCP));
	hintsTCP.ai_family = AF_INET;
	hintsTCP.ai_socktype = SOCK_STREAM;
	hintsTCP.ai_protocol = IPPROTO_TCP;



	char servPort[6];
	
	sprintf_s(servPort,"%d",a.port);

	//Resolve the server address and port
	err = getaddrinfo(a.hostname, servPort, &hintsTCP, &result);
	if (err) {
		WSACleanup();
		return ERR_CONNECT;
	}

	ListenSocket = INVALID_SOCKET;

	//Attempt to connect to the first address returned by
	//the call to getaddrinfo
	ptr = result;

	//Create a SOCKET for connecting to server
	ListenSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);




	//unsigned int myAddr = 2130706433;
	//sockaddr_in clientAddr;
	//ZeroMemory(&clientAddr, sizeof(clientAddr));
	//clientAddr.sin_family = AF_INET;
	//clientAddr.sin_port = port;
	//clientAddr.sin_addr = *(in_addr *)&myAddr;
	////printf("BIND: %d,\n",bind(ListenSocket,(sockaddr *)&clientAddr,sizeof(clientAddr)));


	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return ERR_CONNECT;
	}

	err = ::connect(ListenSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (err == SOCKET_ERROR) {
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ListenSocket == INVALID_SOCKET) {
		return ERR_CONNECT;
	}


	recv(ListenSocket, (char *)&myPort, sizeof(myPort), 0);

	//std::cout << "RECV'd PORT: " << myPort << "\n";
	
	//UDP SETUP

	SendingSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//std::cout << "UDP Sending Socket: " << SendingSocketUDP << std::endl;


	ListenSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//std::cout << "UDP Listen Socket: " << ListenSocketUDP << std::endl;

	SOCKADDR_IN receiverAddr;
	ZeroMemory(&receiverAddr, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	receiverAddr.sin_port = htons(myPort);
	receiverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	
	err = bind(ListenSocketUDP, (SOCKADDR *)&receiverAddr, sizeof(receiverAddr));
	//std::cout << "UDP Socket Bind: " << err << std::endl;

	threadUDP = std::thread(&WinClient::listenFuncUDP, this);
	threadUDP.detach();

	//********



	listenThread = std::thread(&WinClient::listenFunc, this);
	listenThread.detach();


	return 0;
}

int domnet::WinClient::sendMessage(BaseMessage *m, char protocal)
{
	int err;

	switch (protocal) {

	case DN_TCP: {	//TCP
			err = send(ListenSocket, (char *)m, m->getSize(), 0);

			if (err == SOCKET_ERROR) {
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
		}
		break;
	case DN_UDP://UDP
		{
			m->id = myPort;
			err = sendto(SendingSocketUDP, (char *)m, m->getSize(), 0, (sockaddr *)&serverAddress, sizeof(serverAddress));

			if (err == SOCKET_ERROR) {
				closesocket(ListenSocketUDP);
				WSACleanup();
				return 1;
			}
		}
	}


	return 0;
}

void domnet::WinClient::listenFunc()
{
	int err;

	for (;;) {

		//printf("Thread Listening\n");

		while (!mq.isEmpty()) {}	//TODO: Cosider mutex as a safer option. (slower?)

		err = recv(ListenSocket, (useBufOne ? bufOne : bufTwo), BUF_SIZE, 0);

		//printf("%d bytes of data into buffer %d\n", err, (useBufOne ? 1 : 2));

		if (err < 0) {
			exit(1);
		}

		mq.addMessagesToQueue((useBufOne ? bufOne : bufTwo), err);
		useBufOne = !useBufOne;
	}
}

int domnet::WinClient::listenFuncUDP()
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

		std::cout << err << std::endl;

		messageQueueUDP.addMessagesToQueue((useBufOneUDP ? bufOneUDP : bufTwoUDP), err);

		useBufOneUDP = !useBufOneUDP;
	}

}
