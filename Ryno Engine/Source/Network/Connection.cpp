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
		return true;
	}

	bool Connection::want_write() {
		return true;
	}

	bool Connection::do_read() {
		socket->recv(socket, &message);
	}

	bool Connection::do_write() {
		socket->send(socket, &message);
	}

}