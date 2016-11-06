#include "Server.h"
#include "NetUtil.h"
#include "Network.h"

namespace Ryno {

	void Server::start() {

		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
		sock.bind(local_address);
		NetUtil::print(local_address.to_string());
	}



	void Server::update()
	{
		FD_ZERO(&readable);
		FD_SET(sock.get(), &readable);

		if (select(0, &readable, nullptr, NULL, &timeout) == SOCKET_ERROR)
			NetUtil::print_error("Select error: ");

		SmallAddress addr;
		NetMessage mess;

		if (FD_ISSET(sock.get(), &readable))
		{
			I32 res = 0;
			while ((res = sock.recv_struct(&mess, addr)) != 0) {

				//Delete 
				if (res < 0)
					remove_from_connections(addr);
				else {
					if(!is_connection(addr))
						add_to_connections(addr);
					for (auto it = conns.begin(); it != conns.end(); )  //No increment
					{
						Connection *conn = *it;

						if (!conn->address.equals(addr) && !conn->do_write(&mess)) {
							delete conn;
							it = conns.erase(it);
						}
						else it++;
					}
				}
			}
		}
	}

	void Server::close() {
		sock.close();
	}

	void Server::set_timeout(U32 microseconds) {
		timeout.tv_sec = microseconds / 1000000;
		timeout.tv_usec = microseconds % 1000000;
	}

	Connection* Server::add_to_connections(const SmallAddress& addr) {
		Connection* new_conn = new Connection(sock, addr);
		conns.push_back(new_conn);
		return new_conn;
	}

	void Server::remove_from_connections(const SmallAddress& addr) {
		for (Connection* c : conns)
			if (c->address.equals(addr))
			{
				conns.remove(c);
				delete c;
				return;
			}
	}
	bool Server::is_connection(const SmallAddress& addr) {
		for (Connection* c : conns)
			if (addr.equals(c->address)) 
				return true;
		return false;
	}
}