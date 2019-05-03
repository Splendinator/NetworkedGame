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
	ManagerClient(float udpFailPct = 0.f, float latency = 0.0f);
	~ManagerClient();

	void addListener(unsigned int messageType, std::function<void(domnet::BaseMessage *)> func);

	void connect(domnet::Address a);
	void update();
	void send(domnet::BaseMessage *m, bool useTCP = true, bool useLatency = true);

	void delayedUpdate();

private:
	//domnet::Client *_client;
	std::map<unsigned int, std::function<void(domnet::BaseMessage *)>> _listeners;
};

