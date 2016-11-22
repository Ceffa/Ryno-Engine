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

	bool Client::update() {
		if (!sock.create_state.up())
			return false;

		SmallAddress addr;
		NetMessage mess;
		I32 res = 0;

		send_client_update();

		//Receive and dispatch messages.
		//It handles directly only updates from the server.
		//Every other message is forwarded to the scene
		while ((res = sock.recv_struct(&mess, addr)) != 0) {
			if (res < 0) {
				net_scene->remove_every_net_object();
				return false;
			}
			mess.header.to_hardware_order();
			if (mess.header.code == NetCode::SERVER_UPDATE) {
				mess.server_update.to_hardware_order();
				F32 new_time = NetStruct::convert<F32>(mess.server_update.net_time);

				if (!connected) {
					client_id = mess.server_update.client_id;
					connected = true;
					net_scene->on_client_started();
				}
				net_time.recv_time(new_time);


			}
			else if (connected){
				mess.pos_and_color.to_hardware_order();
				net_scene->on_network_recv(&mess);
			}
		}
		//If connected starts to send updates about net object
		if (connected) {
			for (NetObject* net_obj : NetObject::net_objects) {
				bool need_update = net_obj->last_update + net_obj->send_delay <= TimeManager::time;
				bool need_disconnect = net_obj->last_modified + net_obj->disconnect_delay <= TimeManager::time;

				if (need_update && net_obj->owned) {
					net_obj->last_update = TimeManager::time;
					NetMessage m;
					net_scene->on_network_send(net_obj, &m);
					m.header.client_id = client_id;
					m.header.to_network_order();
					m.pos_and_color.to_network_order();

					sock.send_struct(&m, server_address);
				}
				else if (need_disconnect && !net_obj->owned) {
					net_obj->mark_for_destruction = true;
				}
			}
		}
		net_scene->remove_unused_net_objects();
		return true;
	}
	
	void Client::send_client_update() {
		//Return if just sent a message
		if (net_time.last_sent + net_time.update_frequence > TimeManager::time)
			return;

		NetMessage message;
		message.header.id = NetId(local_address);
		message.header.code = NetStruct::convert<U32>(NetCode::CLIENT_UPDATE);
		message.header.to_network_order();
		message.client_update.to_network_order();
		net_time.last_sent = TimeManager::time;
		sock.send_struct(&message, server_address);

	}
	void Client::close() {
		sock.close();
		connected = false;
	}
}
