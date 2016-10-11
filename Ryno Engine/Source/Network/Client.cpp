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


		sock.set_blocking(true);

		if (!sock.connect_state.up()){
			if (sock.connect(server_ip, server_port) < 0) {
				return;
			}
		}
		if (sock.connect_state.up()) {
			sock.set_blocking(false);
			std::string ss;
			if (sock.recv(&ss) == 1)
				NetUtil::print(ss);
		}
	}
	
	void Client::close() {
		sock.close();
	}
}