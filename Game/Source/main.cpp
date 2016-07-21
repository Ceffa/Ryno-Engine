#include "ScenesList.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>


int main(int argc, char* argv[]) {

	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::ScenesList::create_scenes();

	game->init();
	game->run();
	
	return 0;
}
