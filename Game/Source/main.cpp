#include "ScenesList.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>


int main(int argc, char* argv[]) {

	RynoEngine::Game* game = RynoEngine::Game::get_instance();
	RynoEngine::ScenesList::create_scenes();

	game->init();
	game->run();
	
	return 0;
}
