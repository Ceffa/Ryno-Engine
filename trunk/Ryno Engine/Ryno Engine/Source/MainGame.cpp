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

void MainGame::update(){


}
void MainGame::draw(){
	

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	I32 t_locations = m_program.getUniformLocation("myTextureSampler");
	glUniform1i(t_locations, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_model->texture.id);
	//orphan buffer
	glBufferData(GL_ARRAY_BUFFER, m_model->vertices.size() * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW);
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_model->vertices.size() * sizeof(Vertex3D), m_model->vertices.data()); //data return pointer to first element somehow


	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glDrawArrays(GL_TRIANGLES, 0, m_model->vertices.size() / 3);

	/* Swap our buffers to make our changes visible */
	SDL_GL_SwapWindow(m_window);


	glBindTexture(GL_TEXTURE_2D,0);

	glUseProgram(0);
	

		
}


void MainGame::run(){
	init_systems();

	m_program.create("prova");
	m_model = Model::load_model("cube");
	m_model->texture = TextureLoader::loadPNG("metal");

	glGenVertexArrays(1, &vao);				//generato vao
	glBindVertexArray(vao);					//set it as current vao
	glGenBuffers(1, &vbo);					//generate two vbos
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//bind the first one
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (m_game_state == GameState::Running){
		m_program.use();
		handle_input();
		draw();
		m_program.unuse();
	}
	m_program.destroy();
	exit_game();
	

}

