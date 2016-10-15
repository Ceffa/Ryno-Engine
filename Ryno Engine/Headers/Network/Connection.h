#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		U32 written_data = 0;
		sockaddr_in addr;

		Connection::Connection();
		Connection::Connection(const sockaddr_in& _addr);
		Connection::~Connection();
		bool want_read();
		bool want_write();
		bool do_read(std::string& recv_message);
		bool do_write(const std::string& send_message);

		bool is_reading;
		bool is_writing;
	};

}