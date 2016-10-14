#include "Client.h"
#include "NetUtil.h"

namespace Ryno {

	struct pos {
		C c;
	};

	void Client::start() {

		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
	}

	void Client::update() {
		if (!sock.create_state.up())
			return;

		std::string ss;
		pos p;

		int res = 0;
		sockaddr_in addr;

		res = sock.recv_struct(&p,res,&addr);
		if (res == 1) {
			NetUtil::print(p.c);
		}		
	}
	
	void Client::close() {
		sock.close();
	}
}