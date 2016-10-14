#include "Server.h"
#include "NetUtil.h"

namespace Ryno {

	void Server::start() {

		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
		sock.bind(server_ip,server_port);
		sock.listen();
	}
	
	struct pos {
		C c;
	};


	void Server::update()
	{
		sockaddr_in addr;
		pos p;
		I32 res = 0;
		res = sock.recv_struct(&p, res, &addr);

		//fd_set readable;
		//fd_set writeable;

		//FD_ZERO(&readable);
		//FD_ZERO(&writeable);

		//FD_SET(sock.get(), &readable);


		//for (auto conn : conns)
		//{
		//	if (conn->want_read())
		//		FD_SET(conn->sock->get(), &readable);
		//	if (conn->want_write())
		//		FD_SET(conn->sock->get(), &writeable);
		//}

		//int count = select(0, &readable, &writeable, NULL, &timeout);
		//if (count == SOCKET_ERROR)
		//{
		//	NetUtil::print_error("Select error: ");
		//}

		////Handle server socket
		//if (FD_ISSET(sock.get(), &readable))
		//{
		//	Socket* client_sock = sock.accept();
		//	client_sock->set_blocking(false);

		//	if (conns.size() >= max_conns - 1)
		//		client_sock->close();
		//	else
		//		conns.push_back(new Connection(client_sock));
		//}

		////Handle clients
		//for (auto it = conns.begin(); it != conns.end(); )  //No increment
		//{
		//	Connection *conn = *it;
		//	bool alive = true;

		//	if (FD_ISSET(conn->sock->get(), &readable)) {
		//		std::string recv_message;
		//		alive &= conn->do_read(recv_message);

		//		if (!recv_message.empty() && alive) {

		//			for (auto new_it = conns.begin(); new_it != conns.end(); new_it++) {
		//				Connection* temp_c = *new_it;
		//				if (FD_ISSET(temp_c->sock->get(), &writeable)) {
		//					NetUtil::print(recv_message);

		//					alive &= temp_c->do_write(recv_message);

		//				}
		//			}
		//		}
		//	}
		//	
		//

		//	if (!alive)
		//	{
		//		delete conn;
		//		it = conns.erase(it);
		//	}
		//	else
		//		++it;
		//}
	}

	void Server::close() {
		sock.close();
	}

	void Server::set_timeout(U32 microseconds) {
		timeout.tv_sec = microseconds/1000000;
		timeout.tv_usec = microseconds%1000000;
	}
}