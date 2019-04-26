#pragma once
#include "ManagerBase.h"

namespace domnet {
	class Server;
}

class ManagerServer final:
	public ManagerBase
{
public:
	ManagerServer();
	~ManagerServer();

	void addListener(unsigned int messageType, std::function<void(domnet::BaseMessage *, int)> func);

	void host(unsigned int players);
	void update();

private:
	domnet::Server *_server;
	std::map<unsigned int, std::function<void(domnet::BaseMessage *, int)>> _listeners;
	int _numPlayers = 0;
};

