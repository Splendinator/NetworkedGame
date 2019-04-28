#pragma once


namespace domnet {

	#define DEFAULT_PORT_SERV 2345
	#define DEFAULT_PORT_CLIENT 2346
	#define DEFAULT_PORT_STR "2345"
	#define BUF_SIZE 1024
	#define MESSAGE_QUEUE_SIZE 64
	#define SERVER_MAX_CLIENTS 16
	

	//TODO: Make these more useful.
	enum ErrorCode {
		ERR_OK,
		ERR_UNSUPPORTED_OS,
		ERR_INITIALIZE,
		ERR_CONNECT,
		ERR_HOST
	};

	enum Protocal { DN_UDP, DN_TCP };

	//port is automatically set to DEFAULT_PORT
	struct Address {
		const char *hostname;
		unsigned short port = DEFAULT_PORT_SERV;
	};
	



}






