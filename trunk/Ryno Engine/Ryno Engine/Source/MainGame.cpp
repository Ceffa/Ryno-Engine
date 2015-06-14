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
	SDL_GL_SetSwapInterval(1);
	m_game_state = GameState::Running;
}

void MainGame::exit_game(){
	SDL_Quit();
	exit(0);
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
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//draw initializations
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw in back buffer

	U32 vao, vbo[2];
	const GLfloat vertices[4][2] = 
	{
		{ 0.0, 1.0 }, /* Top point */
		{ 1.0, 0.0 }, /* Right point */
		{ 0.0, -1.0 }, /* Bottom point */
		{ -1.0, 0.0 } /* Left point */
	};
	const GLfloat colors[4][3] = 
	{
		{ 1.0, 0.0, 0.0 }, /* Red */
		{ 0.0, 1.0, 0.0 }, /* Green */
		{ 0.0, 0.0, 1.0 }, /* Blue */
		{ 1.0, 1.0, 1.0 }  /* White */
	};

	C *vertex_source, *fragment_source; //shader code
	U32 vertex_id, fragment_id; //shader identifiers
	U32 shader_program;

	glGenVertexArrays(1, &vao);				//generato vao
	glBindVertexArray(vao);					//set it as current vao
	glGenBuffers(2, vbo);					//generate two vbos
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);	//bind the first one
	//specify data to send...
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(F32), vertices, GL_STATIC_DRAW);
	//...how they are made, and in which vbo send them
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);			//enable the vbo

	//repeat for color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(F32), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	I32 i;
	
	/* Make our background black */
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	glDrawArrays(GL_QUADS,0,4);

	/* Swap our buffers to make our changes visible */
	SDL_GL_SwapWindow(m_window);

		
	

	


}


void MainGame::run(){
	init_systems();
	m_program.init();
	m_program.load_shaders("prova");
	m_program.compile_shaders();
	m_program.bind_attributes();
	m_program.link_shaders();
	while (m_game_state == GameState::Running){
		m_program.use();
		handle_input();
		draw();
		m_program.unuse();
	}
	m_program.destroy();
	exit_game();
	

}

