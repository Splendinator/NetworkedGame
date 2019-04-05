#pragma once
#include "Interfaces\BaseNetwork.h"
#include "Message.h"
#include <stddef.h>
#include "WinClient.h"
#include "WinHost.h"
#include "Server.h"


namespace domnet {

	//Initialise the networking library, must be called before anything else.
	//Returns a non-zero value if any errors occur.
	int initialize();
	typedef WinClient Client;
	typedef WinHost Host;

}







