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
		delaySends(_latency, m, player, useTCP);
	}
	else {
		processBandwidth(m);
		_server->sendMessage(m, player, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
	}
	
}

void ManagerServer::broadcast(domnet::BaseMessage *m, bool useTCP, bool useLatency) {
	if (!useTCP && dropPacket()) return;
	if (_latency && useLatency) {
		delayBroadcast(_latency, m, useTCP);
	}
	else {
		processBandwidth(m);
		_server->broadcast(m, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
		if (m->type == 7) { puts("Sent!"); }
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

domnet::BaseMessage *ManagerServer::storeMessageBroadcast(domnet::BaseMessage * m)
{
	domnet::BaseMessage *temp = (domnet::BaseMessage *)malloc(m->getSize());
	memcpy(temp, m, m->getSize());
	_storedMessagesBroadcast.push_back(temp);
	return (_storedMessagesBroadcast.back());
}

void ManagerServer::delMessageSend(domnet::BaseMessage *m) {
	for (auto it = _storedMessagesSend.begin(); it != _storedMessagesSend.end(); ++it) {
		if (m == *it) {
			free(*it);
			_storedMessagesSend.erase(it);
			break;
		}
	}
}

void ManagerServer::delMessageBroadcast(domnet::BaseMessage *m) {
	for (auto it = _storedMessagesBroadcast.begin(); it != _storedMessagesBroadcast.end(); ++it) {
		if (m == *it) {
			free(*it);
			_storedMessagesBroadcast.erase(it);
			break;
		}
	}
}

void ManagerServer::delaySends(int millis, domnet::BaseMessage *m, int player, bool useTCP) {
	domnet::BaseMessage *message = storeMessageSend(m);
	_delayedFuncsSend.push_back({ millis,player,useTCP, message });
}


void ManagerServer::delayBroadcast(int millis, domnet::BaseMessage *m, bool useTCP) {
	domnet::BaseMessage *message = storeMessageBroadcast(m);
	_delayedFuncsBroadcast.push_back({ millis,useTCP, message });
}


void ManagerServer::delayedUpdate()
{
	static std::chrono::system_clock::time_point prev = std::chrono::system_clock::now();
	static std::chrono::system_clock::time_point curr;
	static int millis;


	curr = std::chrono::system_clock::now();
	millis = 1000 * (curr - prev).count() / std::chrono::system_clock::period::den;
	//for (auto it = _delayedFuncs.begin(); it != _delayedFuncs.end(); ++it) {
	//	it->_millis -= millis;
	//
	//	if (it->_millis < 0) {
	//		send(it->m, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP), false);
	//		delMessage(it->m);
	//		it = _delayedFuncs.erase(it);
	//	}
	//}
	for (auto it = _delayedFuncsSend.begin(); it != _delayedFuncsSend.end(); ++it) {
		it->_millis -= millis;

		if (it->_millis < 0) {
			send(it->m,it->playerId, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP), false);
			delMessageSend(it->m);
			it = _delayedFuncsSend.erase(it);
			if (_delayedFuncsSend.size() == 0) break;
		}
	}
	for (auto it = _delayedFuncsBroadcast.begin(); it != _delayedFuncsBroadcast.end(); ++it) {
		it->_millis -= millis;

		if (it->_millis < 0) {
			broadcast(it->m, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP), false);
			delMessageBroadcast(it->m);
			it = _delayedFuncsBroadcast.erase(it);
			if (_delayedFuncsBroadcast.size() == 0) break;
		}
	}
	prev = curr;


}