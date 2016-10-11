#include "Connection.h"
#include "NetUtil.h"

namespace Ryno {

	Connection::Connection(Socket* sock)
		: sock(sock)
	{
		NetUtil::print("New connection");
	}

	Connection::Connection()
		: sock(nullptr)
	{
		NetUtil::print("New connection");
	}

	Connection::~Connection() {
		sock->close();
		sock = nullptr;
		NetUtil::print("Close connection");
	}

	bool Connection::want_read() {
		return !is_writing;
	}

	bool Connection::want_write() {
		return !is_reading;
	}

	bool Connection::do_read() {
		return sock->recv(&message);
	}

	bool Connection::do_write(const std::string& send_message) {
		return sock->send(&send_message);
	}

}