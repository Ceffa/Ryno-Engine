#pragma once
#include "Socket.h"

namespace Ryno{

	class ServerSocket : public Socket {
	public:
		sockaddr_in client_addr;
		SOCKET client_socket = INVALID_SOCKET;

		bool is_listening = false;
		bool is_bound = false;
		bool is_accepted = false;

		void init() override;
		void close() override;

		void listen();
		void bind();
		void accept();
		bool send(const std::string& message) override;
		bool recv(std::string& message) override;

		U32 get_client_port();
		std::string get_client_ip();



	};
}