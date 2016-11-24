#include "Connection.h"
#include "NetUtil.h"
#include "Network.h"

namespace Ryno {

	U32 Connection::last_client_id = 0;
	std::list<U32> Connection::unused_client_ids;

	Connection::Connection(Socket& _sock, const SmallAddress& _addr)
		: Connection()
	{
		sock = &_sock;
		address = _addr;


	}

	Connection::Connection() {
		U32 size = unused_client_ids.size();
		if (size > 0){
			U32 min = 999;
			for (auto i : unused_client_ids)
				if (i < min)
					min = i;
			client_id = min;
		}
		else
			client_id = last_client_id++;
		NetUtil::print(std::string("New connection: ") + address.to_string());
	}

	Connection::~Connection() {
		unused_client_ids.push_back(client_id);
		NetUtil::print(std::string("Closed connection: ") + address.to_string());
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