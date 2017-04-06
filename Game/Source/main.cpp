#include "ScenesList.h"
#include "Shell.h"
#include <iostream>


void before_commands(char* argv[]) {
	Ryno::U32 x, y, w, h;
	SDL_Rect rect;
	SDL_GetDisplayBounds(0, &rect);
	bool server = argv[1][0] == '0' ? true : false;

	int borderY = 90;
	int borderX = 30;

	
	w = rect.w / 2 - borderX;
	h = rect.h/2 - borderY;
	char c = argv[1][0];
	if (c == '0' || c =='1' ) 
		x = borderX/2;
	else
		x = rect.w / 2 + borderX / 2;

	if (c == '0' || c == '2')
		y = borderY / 3;
	else
		y = borderY / 3 + rect.h / 2;
	
	//Swap these lines to toggle 4 windows multiplayer
	//Ryno::Game::get_instance()->set_window_pos(x, y);
	//Ryno::Game::get_instance()->set_window_size(w, h);
	Ryno::Game::get_instance()->set_window_pos(0,0);
	Ryno::Game::get_instance()->set_window_size(1920, 1200);


}
void after_commands(char* argv[]) {
	Ryno::WindowIndex::idx = argv[1][0] - '0';	
}

int main(int argc, char* argv[]) {
	Ryno::Game* game = Ryno::Game::get_instance();
	Ryno::ScenesList::create_scenes();

	game->init_SDL();

	before_commands(argv);
	game->init();
	
	after_commands(argv);
	
	game->run();
	
	return 0;
}
