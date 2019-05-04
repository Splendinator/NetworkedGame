#pragma once
#include <map>
#include <functional>
#include "Message.h"


class ManagerBase
{
public:
	ManagerBase(float udpFailPct = 0.f, float latency = 0.f);
	~ManagerBase();

	bool dropPacket();
	
	void processBandwidth(domnet::BaseMessage *m);


protected:
	void delaySend(int millis, domnet::BaseMessage *m, bool useTCP = false);

	domnet::BaseMessage *storeMessage(domnet::BaseMessage *m);
	void delMessage(domnet::BaseMessage *m);
	void updateBandwidth();

	struct entry {
		int _millis;
		bool useTCP;
		domnet::BaseMessage *m;
	};
	
	float _failChance;
	float _latency;
	int millis;
	std::vector<domnet::BaseMessage *> _storedMessages;
	std::vector<entry> _delayedFuncs;

private:
	int bitties = 0;
	const char *logFilePath = "../Log/Bandwidth.txt";

};

