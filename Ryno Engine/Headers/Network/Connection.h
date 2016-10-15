#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* sock;
		sockaddr_in addr;

		Connection::Connection();
		Connection::Connection(Socket& _sock,const sockaddr_in& _addr);
		Connection::~Connection();
		bool want_read();
		bool want_write();

		template <class T>
		bool do_read(T* message) {
			return sock->recv_struct(message, &addr) > 0;
		}
		template <class T>
		bool do_write(const T& message) {
			return sock->send_struct(message, addr) > 0;
		}

	};

}