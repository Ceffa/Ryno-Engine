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

	public:
		NetworkScene() : Scene() {}
	protected:

		virtual void start() = 0;
		virtual void input() = 0;
		virtual void network_recv(const NetMessage* message) = 0;
		virtual void network_send(NetObject* sender, NetMessage* message) = 0;
		NetObject* create_net_obj(const NetId& id);
		void remove_unused_net_objects();
		Client* client;
		std::list<GameObject> net_game_objects;
	};
}
