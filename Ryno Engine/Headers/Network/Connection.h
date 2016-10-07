#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* socket;

		Connection::Connection();
		Connection::Connection(Socket* _sock);
		Connection::~Connection();
		bool want_read();
		bool want_write();
		bool do_read();
		bool do_write();

	private:
	};

}