#pragma once
#include "Socket.h"

namespace Ryno{

	class ClientSocket : public Socket {
	public:

		bool is_connected = false;

		void init() override;
		void close() override;
		bool send(const std::string& message) override;
		bool recv(std::string& message) override;

		
		void connect();
	};

}