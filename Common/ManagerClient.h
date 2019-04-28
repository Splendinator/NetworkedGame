#pragma once
#include "ManagerBase.h"
#include "Interfaces/BaseNetwork.h"

namespace domnet {
	//class Client;
}

class ManagerClient final :
	public ManagerBase
{
public:
	ManagerClient();
	~ManagerClient();

	void addListener(unsigned int messageType, std::function<void(domnet::BaseMessage *)> func);

	void connect(domnet::Address a);
	void update();
	void send(domnet::BaseMessage *m, bool useTCP = true);

private:
	//domnet::Client *_client;
	std::map<unsigned int, std::function<void(domnet::BaseMessage *)>> _listeners;
};

