#include "Connection.h"
#include "NetUtil.h"

namespace Ryno {

	Connection::Connection(Socket& _sock, const SmallAddress& _addr)
		: sock(&_sock),address(_addr)
	{
	}

	Connection::Connection() : Connection() {}

	Connection::~Connection() {
		NetUtil::print("Close connection");
	}

	bool Connection::want_read() {
		return true;
	}

	bool Connection::want_write() {
		return false;
	}

	

}