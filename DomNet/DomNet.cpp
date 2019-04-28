#include "DomNet.h"
#pragma comment (lib,"Ws2_32.lib")

#include <WinSock2.h>
#include <ws2tcpip.h>

bool isInitialized = false;

int domnet::initialize() {

	if (isInitialized) return ERR_OK;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		return ERR_INITIALIZE;
	}

	isInitialized = true;
	return ERR_OK;
}