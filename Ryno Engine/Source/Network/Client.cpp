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

		SmallAddress addr;
		PosAndColor mess;
		I32 res = 0;


		while (sock.recv_struct(&mess, addr) > 0) {
			mess.to_hardware_order();
			Game::get_instance()->get_scene()->network_recv(&mess);
		}

		for (NetObject* net_obj : NetObject::net_objects) {
			if (local_address.equals(net_obj->id.addr)) {
				PosAndColor m;
				Game::get_instance()->get_scene()->network_send(net_obj,&m);
				m.to_network_order();
				sock.send_struct(&m, server_address);
			}
		}
	}
	
	void Client::close() {
		sock.close();
	}
}
