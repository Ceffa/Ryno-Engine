#include "Connection.h"
#include "NetUtil.h"

namespace Ryno {

	Connection::Connection(Socket* sock)
		: socket(sock)
	{
		NetUtil::print("New connection");
	}

	Connection::Connection()
		: socket(nullptr)
	{
		NetUtil::print("New connection");
	}

	Connection::~Connection() {
		socket->close();
		socket = nullptr;
		NetUtil::print("Close connection");
	}

	bool Connection::want_read() {

	}

	bool Connection::want_write() {

	}

	bool Connection::do_read() {

	}

	bool Connection::do_write() {

	}

}