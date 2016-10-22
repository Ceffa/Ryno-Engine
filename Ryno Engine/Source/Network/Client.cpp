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


		while (sock.recv_struct(&mess, addr) > 0) {
			mess.to_hardware_order();
			Game::get_instance()->get_scene()->network_object_created(mess);
		

		for (NetObject* net_obj : NetObject::net_objects) {
			if (net_obj->moved && local_address.equals(net_obj->id.addr)) {
				Message m;
				m.id = net_obj->id;
				glm::vec3 p = net_obj->game_object->transform.get_position();
				ColorRGBA* col = (ColorRGBA*)net_obj->game_object->get_script<Model>()->sub_models[0].material.get_attribute("in_DiffuseColor");

				m.x = *(U32*)&p.x;
				m.y = *(U32*)&p.y;
				m.z = *(U32*)&p.z;
				m.color = *(U32*)col;


				m.to_network_order();
				sock.send_struct(m, server_address);
			}
		}
	}
	
	void Client::close() {
		sock.close();
	}
}
