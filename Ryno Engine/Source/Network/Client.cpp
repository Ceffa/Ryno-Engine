#include "Client.h"
#include "NetUtil.h"

namespace Ryno {

	void Client::start() {

		if (!sock.init()) {
			close();
			return;
		}
		sock.set_blocking(true);

	}

	void Client::update() {
		if (!sock.create_state.up())
			return;

		if (!sock.connect_state.up()){
			if (sock.connect(server_ip, server_port) < 0) {
				return;
			}
		}
		if (sock.connect_state.up()) {
			sock.set_blocking(false);
			std::string ss;
			C c;

			while (true) {
				I8 res = sock.recv_char(&c);
				if (res == 1) {
					if (c == '\0') {
						NetUtil::print(ss);
						break;
					}
					else
						ss += c;
				}
				else 
					break;
			}

		}
	}
	
	void Client::close() {
		sock.close();
	}
}