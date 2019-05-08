#pragma once
#include "ManagerBase.h"

namespace domnet {
	class Server;
}

class ManagerServer final:
	public ManagerBase
{
public:
	ManagerServer(float udpFailPct = 0.f, float latency = 0.0f);
	~ManagerServer();

	void addListener(unsigned int messageType, std::function<void(domnet::BaseMessage *, int)> func);

	void host(unsigned int players);
	void update();
	void send(domnet::BaseMessage *m, unsigned int player, bool useTCP = true, bool useLatency = true);
	void broadcast(domnet::BaseMessage *m, bool useTCP = true, bool useLatency = true);

	int getNumPlayers() { return _numPlayers; }


private:

	void delayedUpdate();

	domnet::BaseMessage *storeMessageSend(domnet::BaseMessage *m);
	void delMessageSend(domnet::BaseMessage *m);
	void delaySends(int millis, domnet::BaseMessage *m, int player, bool useTCP = false);
	
	domnet::BaseMessage *storeMessageBroadcast(domnet::BaseMessage *m);
	void delayBroadcast(int millis, domnet::BaseMessage *m, bool useTCP = false);
	void delMessageBroadcast(domnet::BaseMessage *m);

	domnet::Server *_server;
	std::map<unsigned int, std::function<void(domnet::BaseMessage *, int)>> _listeners;
	int _numPlayers = 0;

	struct entrySend {
		int _millis;
		int playerId;
		bool useTCP;
		domnet::BaseMessage *m;
	};
	std::vector<domnet::BaseMessage *> _storedMessagesSend;
	std::vector<entrySend> _delayedFuncsSend;

	struct entryBroadcast {
		int _millis;
		bool useTCP;
		domnet::BaseMessage *m;
	};
	std::vector<domnet::BaseMessage *> _storedMessagesBroadcast;
	std::vector<entryBroadcast> _delayedFuncsBroadcast;
};

