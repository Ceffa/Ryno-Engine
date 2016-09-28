#pragma once

#include "Scene.h"
#include "Game.h"
#include "Network.h"
#include "Thread.h"

#include <vector>


namespace Ryno{
	class NetworkScene : public Scene
	{

	public:
		NetworkScene() : Scene(){}
	private:
	
		void start() override;
		void update() override;
		void try_network();
		Server server;
		Thread t;
		
	};
}
