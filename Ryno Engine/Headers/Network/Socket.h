#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "ws2_32.lib")

#pragma once
#include "Global.h"
#include <winsock2.h>

namespace Ryno{

	class Socket {
	private:
		SOCKET sock = SOCKET_ERROR;
	public:
		Socket::Socket();
		Socket::Socket(SOCKET _sock);

		const SOCKET get();
		bool init();
		void close();
		bool bind(const C* ip, U32 port);
		bool connect(const C* server_ip, U32 server_port);
		bool listen();
		Socket* accept();

		bool send(const std::string* message);
		bool recv(std::string* message);

		bool create_ok = false;
		bool bind_ok = false;
		bool listen_ok = false;
		bool connect_ok = false;
		bool accept_ok = false;


	private:
		void verify_socket();
	};




}