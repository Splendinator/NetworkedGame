#include "ManagerServer.h"
#include "DomNet.h"


ManagerServer::ManagerServer()
{

	domnet::initialize();
	_server = new domnet::Server();
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
	_server->host(players);
}

void ManagerServer::send(domnet::BaseMessage *m, unsigned int player, bool useTCP) {
	_server->sendMessage(m, player, (useTCP?domnet::DN_TCP:domnet::DN_UDP));
}

void ManagerServer::broadcast(domnet::BaseMessage *m, bool useTCP) {
	_server->broadcast(m, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
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


