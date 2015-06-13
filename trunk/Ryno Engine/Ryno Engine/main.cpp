#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "ErrorManager.h"

int main(int argc, char* argv[]) {

	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		ErrorManager::Error("Failed to initialize SDL: " + SDL_GetError());
	}
	return 0;
}