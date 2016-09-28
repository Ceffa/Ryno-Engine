#include "ScenesList.h"
#include <iostream>


int main(int argc, char* argv[]) {

	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::ScenesList::create_scenes();

	int window_pos = argv[1][0] == 'l' ? -1 : (argv[1][0] == 'r' ? 1 : 0);
	game->init(window_pos);
	game->run();
	
	return 0;
}
