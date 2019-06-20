#include "ManagerBase.h"
#include <random>
#include <thread>
#include <iostream>
#include <fstream>

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

void ManagerBase::processBandwidth(domnet::BaseMessage * m)
{
	bitties += m->getSize() * 8;
}

void ManagerBase::updateBandwidth()
{
	static std::chrono::system_clock::time_point prev = std::chrono::system_clock::now();
	static std::chrono::system_clock::time_point curr;
	static int millis = 0;
	static std::ofstream fs(logFilePath);

	int index = 0;
	const char *suffix[5] = { "bps","Kbps","Mbps","Gbps","Tbps" };

	curr = std::chrono::system_clock::now();
	
	millis += (curr - prev).count() / (std::chrono::system_clock::period::den / 1000);

	prev = curr;

	if (millis > 1000) {
		//while (bitties > 1000) {
		//	++index;
		//	bitties /= 1000;
		//}

		fs << bitties << '\n';// << " " << suffix[index] << '\n';
		bitties = 0;
		millis -= 1000;
	}


}
