#include "Server.h"
#include "NetUtil.h"

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
		Address addr;
		pos p;
		I32 res;		

		fd_set readable;

		FD_ZERO(&readable);
		FD_SET(sock.get(), &readable);

		if (select(0, &readable, nullptr, NULL, &timeout) == SOCKET_ERROR)
			NetUtil::print_error("Select error: ");
		
		if (FD_ISSET(sock.get(), &readable))
		{
			res = sock.recv_struct(&p, addr);

			if (res > 0) {
				add_to_connections(addr);

				for (auto it = conns.begin(); it != conns.end(); )  //No increment
				{
					Connection *conn = *it;
					
					if (!conn->do_write(p)) {
						delete conn;
						it = conns.erase(it);
					}
					else
						it++;
				}
			}
		}
	}

	void Server::close() {
		sock.close();
	}

	void Server::set_timeout(U32 microseconds) {
		timeout.tv_sec = microseconds/1000000;
		timeout.tv_usec = microseconds%1000000;
	}

	Connection* Server::add_to_connections(const Address& addr) {
		bool exist = false;
		for (Connection* c : conns)
			if (addr.equals(c->address))
				exist = true;
		if (!exist) {
			Connection* new_conn = new Connection(sock,addr);
			conns.push_back(new_conn);
			NetUtil::print(std::string("New connection: ") + std::to_string(conns.size()));
			return new_conn;
		}
		return nullptr;
	}
}