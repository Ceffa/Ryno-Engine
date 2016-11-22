#include "Server.h"
#include "NetUtil.h"
#include "TimeManager.h"
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



	bool Server::update()
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
					if (mess.header.code == NetCode::CLIENT_UPDATE) {
						Connection* conn_handle = find_connection(addr);
						if (!conn_handle)
							conn_handle = add_to_connections(addr);

						NetMessage m;
						m.header.code = NetCode::SERVER_UPDATE;
						m.header.to_network_order();
						
						m.server_update.client_id = conn_handle->client_id;

						m.server_update.net_time = NetStruct::convert<U32>(TimeManager::time);
						m.server_update.to_network_order();
						conn_handle->do_write(&m);
					}
					else {
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
		return true;
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
	Connection* Server::find_connection(const SmallAddress& addr) {
		for (Connection* c : conns)
			if (addr.equals(c->address)) 
				return c;
		return nullptr;
	}
}