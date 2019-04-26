#include "ManagerServer.h"
#include "DomNet.h"


ManagerServer::ManagerServer()
{
	_server = new domnet::Server;
}


ManagerServer::~ManagerServer()
{
	delete _server;
}

void ManagerServer::addListener(unsigned int messageType, std::function<void(domnet::BaseMessage*, int)> func)
{
	_listeners.emplace(messageType, func);
}

void ManagerServer::host(unsigned int players)
{
}

void ManagerServer::update()
{
	static domnet::BaseMessage *m;
	
	//TCP
	for (int i = 0; i < _numPlayers; ++i) {
		while (m = _server->getMessage(i)) {
			auto tuple = _listeners.find(i);
			if (tuple != _listeners.end()) {
				tuple->second(m, i);
			}
		}
	}

	//UDP
	unsigned char player;
	while (m = _server->getMessageUDP(player)) {
		auto tuple = _listeners.find(m->type);
		if (tuple != _listeners.end()) {
			tuple->second(m, player);
		}
	}
}


