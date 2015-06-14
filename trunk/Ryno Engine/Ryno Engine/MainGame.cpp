#include "MainGame.h"



void MainGame::init_systems(){

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ErrorManager::Error("Failed to initialize SDL: ", SDL_GetError());
	}
	
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
	m_game_state = GameState::Running;
}

void MainGame::handle_input(){
	SDL_Event e;
	while (SDL_PollEvent(&e)){
		switch (e.type){
		case SDL_QUIT: m_game_state = GameState::Exit;
		}
	}
}


void MainGame::draw(){
	//draw initializations
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw in back buffer
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	//swap the buffer back in
	SDL_GL_SwapWindow(m_window);

}


void MainGame::run(){
	init_systems();
	while (m_game_state == GameState::Running){
		handle_input();
		draw();
		//need to add here a time manager to limit the frames
	}
}
