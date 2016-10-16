#pragma once

#include "Scene.h"
#include "Game.h"

#include <vector>


namespace Ryno{
	class NetworkScene : public Scene
	{

	public:
		NetworkScene() : Scene(){}
	private:
	
		void start() override;
		void update() override;
	};
}
