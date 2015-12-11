#include "MainScene.h"
#include "SecondScene.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "GJK.h"
#include "SphereCollider.h"



int main(int argc, char* argv[]) {
	Ryno::Game* game = Ryno::Game::get_instance();
	game->init();
	Ryno::MainScene* scene = new Ryno::MainScene();
	game->set_scene(scene);
	game->run();
	
	return 0;
}
