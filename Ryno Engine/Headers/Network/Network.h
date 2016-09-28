#pragma once
#include "Global.h"
#include "Server.h"
#include "Client.h"
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVERIP "127.0.0.1"

#define SERVERPORT 5555

#define MESSAGESIZE 40

namespace Ryno{

	static class Network {
	public:
		static void init();
	};

}