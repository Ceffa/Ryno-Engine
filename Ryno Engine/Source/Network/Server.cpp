#include "Server.h"
#include "NetUtil.h"
#include "TimeManager.h"
#include "Network.h"
#include "NetworkScene.h"

namespace Ryno {

	void Server::start() {

		//Connect and initialize socket
		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
		sock.bind(local_address);
		NetUtil::print(local_address.to_string());
		net_scene = (NetworkScene*)Game::get_instance()->get_scene();

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

			//**RECEIVE MESSAGES**
			while ((res = sock.recv_struct(&mess, addr)) != 0) {
				if (res < 0)
					remove_from_connections(addr);
				else {
					U32 code = ntohl(mess.header.code);
					//If a client requests the net time, send it to him.
					//Also, if the client is not yet in the connections, add it
					if (code == NetCode::CLIENT_TIME) {
						Connection* conn_handle = find_connection(addr);
						if (!conn_handle)
							conn_handle = add_to_connections(addr);

						NetMessage m;
						m.header.code = NetCode::SERVER_TIME;
						m.header.to_network_order();
						
						m.server_update.client_id = conn_handle->client_id;

						m.server_update.set_time(TimeManager::time);
						m.server_update.to_network_order();
						conn_handle->do_write(&m);
					}
					else if (code == NetCode::OBJECT){
						//Forward all the object messages to every client except the sender 
						for (auto it = conns.begin(); it != conns.end(); )  
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

			//**SEND MESSAGES**
			//Every once in a while send an update to every client.
			//The network scene is responsible to define the protocol for it,
			//both in sending and reception
			if (TimeManager::time > last_periodic_update + update_frequence) {
				last_periodic_update = TimeManager::time;
				NetMessage m;
				m.header.code = NetCode::UPDATE;
				m.header.to_network_order();
				net_scene->on_periodic_update_send(&m);
				m.net_array.to_network_order();
				//Dispatch to every client
				for (auto it = conns.begin(); it != conns.end(); )  //No increment
				{
					Connection *conn = *it;
					m.server_update.client_id = conn->client_id;

					if (!conn->address.equals(addr) && !conn->do_write(&m)) {
						delete conn;
						it = conns.erase(it);
					}
					else it++;
				}
			}
		}
		return true;
	}

	void Server::close() {
		sock.close();
	}

	Connection* Server::add_to_connections(const SmallAddress& addr) {
		request_update();
		Connection* new_conn = new Connection(sock, addr);
		conns.push_back(new_conn);
		return new_conn;
	}

	void Server::remove_from_connections(const SmallAddress& addr) {
		request_update();
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