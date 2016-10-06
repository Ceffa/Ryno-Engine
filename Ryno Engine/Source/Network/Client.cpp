#include "Client.h"
#include "NetUtil.h"

namespace Ryno {

	void Client::init() {

		if (!sock.init()) {
			close();
			return;
		}
		
		if (!sock.connect(server_ip, server_port)) {
			close();
			return;
		}
		
		while (true) {
			if (!sock.send(sock, "Hello")) {
				close();
				return;
			}
		}
		close();
	}
	
	void Client::close() {
		sock.close();
	}
}