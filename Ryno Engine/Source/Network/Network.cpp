#include "Network.h"
#include "Log.h"

namespace Ryno {

	void Network::init() {
		// Initialise the WinSock library -- we want version 2.2.
		WSADATA w;
		int error = WSAStartup(0x0202, &w);
		if (error != 0)
			Log::println("WSAStartup failed");
	
		if (w.wVersion != 0x0202)
			Log::println("Wrong WinSock version");
	}

}