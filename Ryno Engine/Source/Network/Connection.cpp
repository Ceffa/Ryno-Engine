#include "Connection.h"
#include "NetUtil.h"

namespace Ryno {

	Connection::Connection(Socket* sock)
		: sock(sock),written_data(0)
	{
		is_reading = false;
		is_writing = false;
		NetUtil::print("New connection");
	}

	Connection::Connection() : Connection(nullptr) {}

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

	bool Connection::do_read(std::string& recv_message) {
		C c;
		I32 res;
		is_reading = true;

		do {
			res = sock->recv_char(&c);
			if (res == -1) {
				read_buffer.clear();
				return false;
			}
			if (res == 1)
				read_buffer += c;
		} while (res == 1);


		if (c == '\0') {
			recv_message = read_buffer;

			is_reading = false;
			read_buffer.clear();
		}
		return true;
	}

	bool Connection::do_write(const std::string& send_message) {
		I32 temp_data;

		//Send data
		if (written_data > 0)
			temp_data = sock->send(&send_message.substr(written_data));
		else
			temp_data = sock->send(&send_message);

		//Error -> stop writing
		if (temp_data < 0) {
			is_writing = false;
			written_data = 0;
			return false;
		}
		//Message sent -> stop writing
		else if (temp_data >= send_message.size())
		{
			is_writing = false;
			written_data = 0;
			return true;
		}
		//Still sending
		else {
			is_writing = true;
			written_data += temp_data;
		}
	}

}