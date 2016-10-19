#pragma once

#include "Scene.h"
#include "Game.h"
#include "Network.h"


#include <vector>


namespace Ryno{
	class NetworkScene : public Scene
	{

	public:
		NetworkScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void network_object_created(const Message& message) override;
		NetObject* create_net_obj(const NetId& id);

		NetEntity* net_entity;
		GameObject cube;
		NetObject* controlled = nullptr;
		std::list<GameObject> net_cubes;
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	};
}
