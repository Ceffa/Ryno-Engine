#pragma once

#include "Scene.h"
#include "Game.h"
#include "NetStructs.h"
#include "Client.h"

#include <vector>


namespace Ryno{

	class NetworkScene : public Scene
	{

	public:
		NetworkScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void network_recv(const NetMessage* message) override;
		void network_send(NetObject* sender, NetMessage* message) override;

		NetObject* create_net_obj(const NetId& id);

		Client* client;
		GameObject cube;
		NetObject* controlled = nullptr;
		std::list<GameObject> net_cubes;
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	};
}
