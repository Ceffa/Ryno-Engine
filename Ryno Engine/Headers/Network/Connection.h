#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* sock;
		std::string message;

		Connection::Connection();
		Connection::Connection(Socket* _sock);
		Connection::~Connection();
		bool want_read();
		bool want_write();
		bool do_read();
		bool do_write(const std::string& send_message);

		bool is_reading = false;
		bool is_writing = false;
	};

}