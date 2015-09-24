#include "MainGame.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "GJK.h"
#include "SphereCollider.h"

int main(int argc, char* argv[]) {
	Ryno::MainGame main_game;
	main_game.run();
	//Ryno::SphereCollider* a = new Ryno::SphereCollider(19,57, 0, 10);
	//Ryno::SphereCollider* b = new Ryno::SphereCollider(18, 57, 0, 8);
	//glm::vec3 dir = b->center - a->center;
	//std::cout << Ryno::GJK::gjk(a, b)<< std::endl;

	return 0;
}
