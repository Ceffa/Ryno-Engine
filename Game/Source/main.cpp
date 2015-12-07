#include "MainGame.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "GJK.h"
#include "SphereCollider.h"



int main(int argc, char* argv[]) {
	Ryno::MainGame main_game;
	main_game.run();
	
	return 0;
}
