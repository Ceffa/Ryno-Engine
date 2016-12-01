#pragma once

#include "Scene.h"
#include "Game.h"
#include "NetStructs.h"
#include "Client.h"

#include <vector>


namespace Ryno {

	//Inherits from the Scene class.
	//It adds a lot of handler functions that must be overrided to define behaviour
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
		virtual void on_periodic_update_send(NetMessage* message) = 0;
		virtual void on_periodic_update_recv(NetMessage* message) = 0;
		virtual void on_client_started() = 0;
		NetObject* create_net_obj(const NetId& id);
		void remove_unused_net_objects();	//clean objects that didn't receive updates for a while
		void remove_every_net_object();		//clean every non-owned net objects if the client is disconnected

		//Keep track of list of net_objects in the scene
		std::list<GameObject> net_game_objects;
	};
}
