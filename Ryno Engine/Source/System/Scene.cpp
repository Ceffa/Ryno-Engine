#include "Game.h"
#include "Scene.h"

namespace Ryno{

	std::map<std::string, Scene*> Scene::scenes;
	Scene::Scene(std::string name)
	{
		game = Game::get_instance();
		scenes[name] = this;
	}
}