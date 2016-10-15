#include "Connection.h"
#include "NetUtil.h"

namespace Ryno {

	Connection::Connection(const sockaddr_in& _addr)
		: addr(_addr),written_data(0)
	{
		is_reading = false;
		is_writing = false;
		NetUtil::print("New connection");
	}

	Connection::Connection() : Connection() {}

	Connection::~Connection() {
		NetUtil::print("Close connection");
	}

	bool Connection::want_read() {
		return !is_writing;
	}

	bool Connection::want_write() {
		return !is_reading;
	}

	bool Connection::do_read(std::string& recv_message) {
		return true;
	}

	bool Connection::do_write(const std::string& send_message) {
		return false;
	}

}