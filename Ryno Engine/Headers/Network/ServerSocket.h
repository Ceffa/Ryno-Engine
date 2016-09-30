#pragma once
#include "Socket.h"

namespace Ryno{

	class ServerSocket : public Socket {
	public:
		SOCKET sock = INVALID_SOCKET;
		sockaddr_in client_addr;

		void init() override;
		void close() override;

		void listen();
		void bind();
		void accept();

		U32 get_client_port();
		std::string get_client_ip();

	};
}