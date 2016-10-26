#include "Client.h"
#include "NetUtil.h"
#include "Scene.h"
#include "Game.h"


namespace Ryno {


	void Client::start() {

		game = Game::get_instance();

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
			game->get_scene()->network_recv(&mess);
		}

		for (NetObject* net_obj : NetObject::net_objects) {
			bool need_update = net_obj->last_send + delay <= game->time;
			need_update &= local_address.equals(net_obj->id.addr);
			if (need_update) {
				net_obj->last_send = game->time;
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
