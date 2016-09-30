#pragma once
#include "Socket.h"

namespace Ryno{

	class ClientSocket : public Socket {
	public:
		SOCKET sock = INVALID_SOCKET;

		void init() override;
		void close() override;
		
		void connect();
	};

}