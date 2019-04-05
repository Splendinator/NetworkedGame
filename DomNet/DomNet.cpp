#include "DomNet.h"
#pragma comment (lib,"Ws2_32.lib")

#include <WinSock2.h>
#include <ws2tcpip.h>


int domnet::initialize() {


	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		return ERR_INITIALIZE;
	}

	return ERR_OK;
}