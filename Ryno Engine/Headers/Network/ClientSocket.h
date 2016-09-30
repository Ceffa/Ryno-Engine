#pragma once
#include "Socket.h"

namespace Ryno{

	class ClientSocket : public Socket {
	public:
		SOCKET sock = INVALID_SOCKET;
		
		bool is_connected = false;

		void init() override;
		void close() override;
		
		void connect();
	};

}