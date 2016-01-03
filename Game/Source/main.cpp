#include "ScenesList.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "GJK.h"
#include "SphereCollider.h"



int main(int argc, char* argv[]) {

	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::ScenesList::create_scenes();
	game->set_scene("house");
	game->init();
	game->run();
	
	return 0;
}
