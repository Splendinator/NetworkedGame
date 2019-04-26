#include "ManagerClient.h"
#include "DomNet.h"

domnet::Client client;

ManagerClient::ManagerClient()
{
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

void ManagerClient::update()
{
	//TODO: this, also initialize domnet!
}
