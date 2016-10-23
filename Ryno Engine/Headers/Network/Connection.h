#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* sock;
		SmallAddress address;

		Connection::Connection();
		Connection::Connection(Socket& _sock,const SmallAddress& _addr);
		Connection::~Connection();
		bool want_read();
		bool want_write();

		bool do_read(Message* message) {
			return sock->recv_struct(message, address) >= 0;
		}
		bool do_write(const Message* message) {
			return sock->send_struct(message, address) >= 0;
		}

	};
}