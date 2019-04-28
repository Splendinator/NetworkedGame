#include "ManagerClient.h"
#include "DomNet.h"

domnet::Client client;

ManagerClient::ManagerClient()
{
	domnet::initialize();
}


ManagerClient::~ManagerClient()
{
}

void ManagerClient::addListener(unsigned int messageType, std::function<void(domnet::BaseMessage*)> func)
{
	_listeners.emplace(messageType, func);
}

void ManagerClient::connect(domnet::Address a)
{
	client.connect(a);
}

void ManagerClient::send(domnet::BaseMessage *m, bool useTCP) {
	client.sendMessage(m, (useTCP ? domnet::DN_TCP : domnet::DN_UDP));
}

void ManagerClient::update()
{
	domnet::BaseMessage *m;
	while (m = client.getMessage()) {
		auto tuple = _listeners.find(m->type);
		if (tuple != _listeners.end()) {
			tuple->second(m);
		}
	}
	while (m = client.getMessageUDP()) {
		auto tuple = _listeners.find(m->type);
		if (tuple != _listeners.end()) {
			tuple->second(m);
		}
	}
}
