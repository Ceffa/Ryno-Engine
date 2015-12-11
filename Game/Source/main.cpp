#include "MainScene.h"
#include "SecondScene.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "GJK.h"
#include "SphereCollider.h"



int main(int argc, char* argv[]) {
	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::MainScene* scene = new Ryno::MainScene("main");
	Ryno::SecondScene* second_scene = new Ryno::SecondScene("second");

	game->run("main");
	
	return 0;
}
