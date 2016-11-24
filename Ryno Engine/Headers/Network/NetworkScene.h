#pragma once

#include "Scene.h"
#include "Game.h"
#include "NetStructs.h"
#include "Client.h"

#include <vector>


namespace Ryno {

	class NetworkScene : public Scene
	{
		friend class Client;
		friend class Server;

	public:
		NetworkScene() : Scene() {}
	protected:

		virtual void start() = 0;
		virtual void input() = 0;
		virtual void on_network_recv(const NetMessage* message) = 0;
		virtual void on_network_send(NetObject* sender, NetMessage* message) = 0;
		//Called periodically by the server to give info to the clients
		virtual void on_periodic_update(NetMessage* message) = 0;
		virtual void on_client_started() = 0;
		NetObject* create_net_obj(const NetId& id);
		void remove_unused_net_objects();	//clean objects that didn't receive updates for a while
		void remove_every_net_object();		//clean every net objects if the client is disconnected (except his)
		std::list<GameObject> net_game_objects;
	};
}
