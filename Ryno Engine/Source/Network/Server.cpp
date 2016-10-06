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
		Socket* client_sock = sock.accept();
		while (true) {
			std::string s;
			if (!sock.recv(client_sock, &s)) {
				close();
				return;
			}
			NetUtil::print(s);
		}
		close();
	}
	
	void Server::close() {
		sock.close();
	}
}