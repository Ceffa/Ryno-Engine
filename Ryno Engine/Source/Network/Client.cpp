#include "Client.h"
#include "NetUtil.h"
#include "Scene.h"
#include "Game.h"


namespace Ryno {

	void Client::start() {

		if (!sock.init(true)) {
			close();
			return;
		}
		sock.set_blocking(false);
		sock.bind(local_address);
		NetUtil::print(local_address.to_string());

	}

	void Client::update() {
		if (!sock.create_state.up())
			return;

		Address addr;
		SmallAddress p;
		I32 res = 0;

		res = sock.recv_struct(&p,addr);

		if (res > 0) {
			Game::get_instance()->get_scene()->network_object_created(p);

		}		
	}
	
	void Client::close() {
		sock.close();
	}
}