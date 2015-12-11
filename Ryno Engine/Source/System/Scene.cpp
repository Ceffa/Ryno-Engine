#include "Game.h"
#include "Scene.h"

namespace Ryno{

	Scene::Scene()
	{
		game = Game::get_instance();
	}
}