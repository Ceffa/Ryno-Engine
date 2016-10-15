#include "Client.h"
#include "NetUtil.h"

namespace Ryno {

	void Client::start() {

		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
		sock.bind(server_ip, 0);

	}

	void Client::update() {
		if (!sock.create_state.up())
			return;

		sockaddr_in addr;
		pos p;
		I32 res = 0;


		res = sock.recv_struct(&p.c,res,&addr);
		if (res == 1) {
			NetUtil::print(p.c);
		}		
	}
	
	void Client::close() {
		sock.close();
	}
}