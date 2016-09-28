#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma once
#include "Global.h"
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVERIP "127.0.0.1"
#define SERVERPORT 5555
#define MESSAGESIZE 40

namespace Ryno{

	class Client {
	public:
		SOCKET sock = INVALID_SOCKET;
		sockaddr_in server_addr;
		bool is_connected;
		bool is_created;

		void init();
		void connect();
		void die(std::string s);
		void print(std::string s);

	};
}