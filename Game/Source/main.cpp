#include "ScenesList.h"
#include "Shell.h"
#include <iostream>


int main(int argc, char* argv[]) {

	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::ScenesList::create_scenes();

	bool server = argv[1][0] == 'l' ? true : false;
	int window_pos = server ? -1 : 1;
	game->init(window_pos);
	Ryno::Shell::get_instance()->parse_command(server ? "server" : "client");
	game->run();
	
	return 0;
}
