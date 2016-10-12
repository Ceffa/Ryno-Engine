#pragma once
#include "Socket.h"

namespace Ryno{

	class Connection {
	public:
		Socket* sock;
		std::string read_buffer;
		U32 written_data = 0;


		Connection::Connection();
		Connection::Connection(Socket* _sock);
		Connection::~Connection();
		bool want_read();
		bool want_write();
		bool do_read(std::string& recv_message);
		bool do_write(const std::string& send_message);

		bool is_reading;
		bool is_writing;
	};

}