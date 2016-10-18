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
		Message mess;
		I32 res = 0;

		res = sock.recv_struct(&mess, addr);

		if (res > 0) {
			NetUtil::print(mess.x);
			Game::get_instance()->get_scene()->network_object_created(mess);
		}
		for (NetObject* net_obj : NetObject::net_objects) {
			Message m;
			m.id = local_address;
			glm::vec3 p = net_obj->game_object->transform.get_position();
			m.x = p.x;
			m.y = p.y;
			m.z = p.z;
			sock.send_struct(m, server_address);
		}
	}
	
	void Client::close() {
		sock.close();
	}
}