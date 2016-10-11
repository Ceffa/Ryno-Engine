#include "Server.h"
#include "NetUtil.h"

namespace Ryno {

	void Server::init() {

		if (!sock.init()) {
			close();
			return;
		}
		sock.bind(ip,port);
		sock.listen();
		while (true)
		{
			fd_set readable;
			fd_set writeable;

			FD_ZERO(&readable);
			FD_ZERO(&writeable);


			FD_SET(sock.get(), &readable);


			for (auto conn : conns)
			{
				if (conn->want_read())
					FD_SET(conn->sock->get(), &readable);
				if (conn->want_write())
					FD_SET(conn->sock->get(), &writeable);
				
			}

		
			timeval timeout;
			timeout.tv_sec = 2;
			timeout.tv_usec = 500000;

			int count = select(0, &readable, &writeable, NULL, &timeout);
			if (count == SOCKET_ERROR)
			{
				NetUtil::print_error("Select error: ");
			}
	
			//Handle server socket
			if (FD_ISSET(sock.get(), &readable))
			{
				Socket* client_sock = sock.accept();

				if (conns.size() >= max_conns - 1)
					client_sock->close();
				else
					conns.push_back(new Connection(client_sock));
			}

			//Handle clients
			for (auto it = conns.begin(); it != conns.end(); )  //No increment
			{
				Connection *conn = *it;
				bool alive = true;

				if (FD_ISSET(conn->sock->get(), &readable)) {
					alive &= conn->do_read();
				}
				if (FD_ISSET(conn->sock->get(), &writeable)) {
					if (!conn->message.empty()) {
						conn->message += " + server";
						NetUtil::print("Write: " + conn->message);
						alive &= conn->do_write();
						conn->message.clear();
					}
				}

				if (!alive)
				{
					delete conn;
					it = conns.erase(it);
				}
				else
					++it;
				
			}
		}
		close();
	}
	
	void Server::close() {
		sock.close();
	}
}