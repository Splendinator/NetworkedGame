#include "ManagerBase.h"
#include <random>
#include <thread>
#include <iostream>


ManagerBase::ManagerBase(float loss, float lat)
{
	_failChance = loss;
	_latency = lat;

}

ManagerBase::~ManagerBase()
{
	
}

bool ManagerBase::dropPacket()
{
	return rand() <= int(RAND_MAX * _failChance);
}

void ManagerBase::delaySend(int millis,domnet::BaseMessage *m, bool useTCP) {

	domnet::BaseMessage *message = storeMessage(m);
	_delayedFuncs.push_back({ millis,useTCP, message});
	 
}

domnet::BaseMessage *ManagerBase::storeMessage(domnet::BaseMessage * m)
{
	domnet::BaseMessage *temp = (domnet::BaseMessage *)malloc(m->getSize());
	memcpy(temp, m, m->getSize());
	_storedMessages.push_back(temp);
	return (_storedMessages.back());
}

void ManagerBase::delMessage(domnet::BaseMessage *m) {
	for (auto it = _storedMessages.begin(); it != _storedMessages.end(); ++it) {
		if (m == *it) {
			_storedMessages.erase(it);
			//std::cout << "Deleted!\n";
			break;
		}
	}
}
