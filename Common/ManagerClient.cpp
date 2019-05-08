#include "ManagerClient.h"
#include "DomNet.h"
#include <chrono>

domnet::Client *client;

ManagerClient::ManagerClient(float loss, float lat) : ManagerBase(loss,lat)
{
	domnet::initialize();
	client = new domnet::Client;
}


ManagerClient::~ManagerClient()
{
	delete client;
}

void ManagerClient::addListener(unsigned int messageType, std::function<void(domnet::BaseMessage*)> func)
{
	_listeners.emplace(messageType, func);
}

void ManagerClient::connect(domnet::Address a)
{
	client->connect(a);
}

void ManagerClient::send(domnet::BaseMessage *m, bool useTCP, bool useLatency) {
	if (!useTCP && dropPacket()) return;
	
	if (_latency && useLatency) {
		delaySend(_latency, m, useTCP);
	}
	else {
		client->sendMessage(m, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
	}
}

void ManagerClient::delayedUpdate()
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
			send(it->m, (it->useTCP ? domnet::DN_TCP : domnet::DN_UDP),false);
			delMessage(it->m);
			it = _delayedFuncs.erase(it);
			if (_delayedFuncs.size() == 0) break;
		}
	}
	prev = curr;


}

void ManagerClient::update()
{
	domnet::BaseMessage *m;
	while (m = client->getMessage()) {
		auto tuple = _listeners.find(m->type);
		if (tuple != _listeners.end()) {
			tuple->second(m);
		}
	}
	while (m = client->getMessageUDP()) {
		auto tuple = _listeners.find(m->type);
		if (tuple != _listeners.end()) {
			tuple->second(m);
		}
	}
	delayedUpdate();
}
