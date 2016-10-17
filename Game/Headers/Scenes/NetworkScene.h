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
		void update() override;
		void network_object_created(const SmallAddress& creator_address) override;

		NetEntity* net_entity;
		GameObject cube;
		std::vector<GameObject> net_cubes;
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	};
}
