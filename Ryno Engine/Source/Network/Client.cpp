#include "Client.h"
#include "NetStructs.h"
#include "Scene.h"
#include "Game.h"
#include "NetworkScene.h"


namespace Ryno {


	void Client::start() {

		game = Game::get_instance();
		net_scene = (NetworkScene*)Game::get_instance()->get_scene();

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
		NetMessage mess;
		I32 res = 0;


		while (sock.recv_struct(&mess, addr) > 0) {
			mess.header.to_hardware_order();
			mess.pos_and_color.to_hardware_order();
			net_scene->network_recv(&mess);
		}

		for (NetObject* net_obj : NetObject::net_objects) {
			bool owned = local_address.equals(net_obj->id.addr);
			bool need_update = net_obj->last_update + net_obj->send_delay <= game->time;
			bool need_disconnect = net_obj->last_modified + net_obj->disconnect_delay <= game->time;

			if (need_update && owned) {
				net_obj->last_update = game->time;
				NetMessage m;
				net_scene->network_send(net_obj,&m);
				m.header.to_network_order();
				m.pos_and_color.to_network_order();

				sock.send_struct(&m, server_address);
			}
			else if (need_disconnect && !owned) {
				net_obj->mark_for_destruction = true;
			}
		}
		net_scene->remove_unused_net_objects();
	}
	
	void Client::close() {
		sock.close();
	}
}
