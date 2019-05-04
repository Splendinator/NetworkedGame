#include "ManagerServer.h"
#include "DomNet.h"


ManagerServer::ManagerServer(float loss, float lat) : ManagerBase(loss, lat)
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
	_numPlayers = players;
	_server->host(players);
}

void ManagerServer::send(domnet::BaseMessage *m, unsigned int player, bool useTCP, bool useLatency) {
	if (!useTCP && dropPacket()) return;
	
	if (_latency && useLatency) {
		delaySend(_latency, m, player, useTCP);
	}
	else {
		processBandwidth(m);
		_server->sendMessage(m, player, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));;
	}
	
}

void ManagerServer::broadcast(domnet::BaseMessage *m, bool useTCP, bool useLatency) {
	if (!useTCP && dropPacket()) return;
	if (_latency && useLatency) {
		delaySend(_latency, m, useTCP);
	}
	else {
		processBandwidth(m);
		_server->broadcast(m, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
	}
}



void ManagerServer::update()
{
	static domnet::BaseMessage *m;
	
	//TCP
	for (int i = 0; i < _numPlayers; ++i) {
		while (m = _server->getMessage(i)) {
			auto tuple = _listeners.find(m->type);
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
	delayedUpdate();
	updateBandwidth();
}


domnet::BaseMessage *ManagerServer::storeMessageSend(domnet::BaseMessage * m)
{
	domnet::BaseMessage *temp = (domnet::BaseMessage *)malloc(m->getSize());
	memcpy(temp, m, m->getSize());
	_storedMessagesSend.push_back(temp);
	return (_storedMessagesSend.back());
}

void ManagerServer::delMessageSend(domnet::BaseMessage *m) {
	for (auto it = _storedMessagesSend.begin(); it != _storedMessagesSend.end(); ++it) {
		if (m == *it) {
			_storedMessagesSend.erase(it);
			break;
		}
	}
}

void ManagerServer::delaySend(int millis, domnet::BaseMessage *m, int player, bool useTCP) {
	domnet::BaseMessage *message = storeMessageSend(m);
	_delayedFuncsSend.push_back({ millis,player,useTCP, message });
}


void ManagerServer::delayedUpdate()
{
	static std::chrono::system_clock::time_point prev = std::chrono::system_clock::now();
	static std::chrono::system_clock::time_point curr;
	static int millis;


	//for (;;) {
	curr = std::chrono::system_clock::now();
	millis = 1000 * (curr - prev).count() / std::chrono::system_clock::period::den;
	for (auto it = _delayedFuncs.begin(); it != _delayedFuncs.end(); ++it) {
		it->_millis -= millis;
	
		if (it->_millis < 0) {
			send(it->m, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP), false);
			delMessage(it->m);
			it = _delayedFuncs.erase(it);
		}
	}
	for (auto it = _delayedFuncsSend.begin(); it != _delayedFuncsSend.end(); ++it) {
		it->_millis -= millis;

		if (it->_millis < 0) {
			send(it->m,it->playerId, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP), false);
			delMessageSend(it->m);
			it = _delayedFuncsSend.erase(it);
		}
	}
	prev = curr;


}