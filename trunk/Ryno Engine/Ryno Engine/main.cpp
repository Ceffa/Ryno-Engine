#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "ErrorManager.h"

int main(int argc, char* argv[]) {

	;

	if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
		ErrorManager::Error("Failed to initialize SDL: " ,SDL_GetError());
	}

	
	SDL_Window* m_window;
	if ((m_window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL)) == NULL){
		ErrorManager::Error("Failed to create SDL window: ", SDL_GetError());
	}

	SDL_GLContext gl_context;
	if ((gl_context = SDL_GL_CreateContext(m_window)) = nullptr){
		ErrorManager::Error("Failed to create GL_context: ", SDL_GetError());
	}

	glewExperimental = GLU_TRUE;
	GLenum error;
	if ((error = glewInit()) != GLEW_OK){
		ErrorManager::Error("Failed to initialize glew.");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	
	

	while (true){
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
			case SDL_QUIT: return 0; 
			}
		}

		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(m_window);

	}

	return 0;
}