#pragma once
#include "Socket.h"

namespace Ryno{

	class Client{
	public:
		Client::Client(C* _server_ip, U32 _server_port) : server_ip(_server_ip), server_port(_server_port) {}
		Client::~Client() { close(); }

		void init();
		void close();

		Socket sock;
		C* server_ip;
		U32 server_port;
	};
}