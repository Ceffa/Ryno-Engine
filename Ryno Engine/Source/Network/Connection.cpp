#include "Connection.h"
#include "NetUtil.h"
#include "Network.h"

namespace Ryno {

	Connection::Connection(Socket& _sock, const SmallAddress& _addr)
		: sock(&_sock),address(_addr)
	{
	}

	Connection::Connection() : Connection() {}

	Connection::~Connection() {
	}

	bool Connection::want_read() {
		return true;
	}

	bool Connection::want_write() {
		return false;
	}

	

	bool Connection::do_read(NetMessage* message)
	{
		return sock->recv_struct(message, address) >= 0;
	}

	bool Connection::do_write(const NetMessage* message)
	{
		return sock->send_struct(message, address) >= 0;
	}

}