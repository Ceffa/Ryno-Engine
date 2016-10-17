#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* sock;
		Address address;

		Connection::Connection();
		Connection::Connection(Socket& _sock,const Address& _addr);
		Connection::~Connection();
		bool want_read();
		bool want_write();

		template <class T>
		bool do_read(T* message) {
			return sock->recv_struct(message, &address) > 0;
		}
		template <class T>
		bool do_write(const T& message) {
			return sock->send_struct(message, address) > 0;
		}

	};

}