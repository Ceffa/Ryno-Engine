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
		
		bool sent = false;
		while (true) {

			std::string s("hei");
			if (!sent && !sock.send(&s)) {
				close();
				return;
			}

			else {
				sent = true;
			}
			
			std::string ss;
			if (sock.recv(&ss))
				NetUtil::print(ss);
		}
		close();
	}
	
	void Client::close() {
		sock.close();
	}
}