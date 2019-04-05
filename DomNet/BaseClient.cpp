#include "BaseClient.h"

using namespace domnet;

BaseMessage * domnet::BaseClient::getMessage()
{
	return mq.getMessage();
}
