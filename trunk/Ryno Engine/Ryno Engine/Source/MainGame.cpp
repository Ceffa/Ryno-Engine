#include "MainGame.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>


namespace Ryno{
	void MainGame::init_systems(){

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			Log::FatalError("Failed to initialize SDL: ", SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetSwapInterval(1);

		if ((m_window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL)) == NULL){
			Log::FatalError("Failed to create SDL window: ", SDL_GetError());
		}



		SDL_GLContext gl_context;
		if ((gl_context = SDL_GL_CreateContext(m_window)) = nullptr){
			Log::FatalError("Failed to create GL_context: ", SDL_GetError());
		}

		glewExperimental = GLU_TRUE;
		GLenum error;
		if ((error = glewInit()) != GLEW_OK){
			Log::FatalError("Failed to initialize glew.");
		}


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

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
		m_camera->generate_camera_matrix(); 
		m_game_object->generate_model_matrix();

	}

	void MainGame::start(){
		m_batch3d = new Batch3D();
		m_batch3d->init();
		m_camera = new Camera3D(800, 600);
		
		m_program.create("prova");
		m_model = Model::load_model("cube");
		m_model->texture = TextureLoader::loadPNG("metal");

		m_game_object = new GameObject();
		m_game_object->model = m_model;

		m_game_object->position = glm::vec3(-200, -100, 0);


	}

	void MainGame::run(){
		init_systems();
		start();
				
		while (m_game_state == GameState::Running){
			
			handle_input();
			m_program.use();
			draw();
			m_program.unuse();
		}
		m_program.destroy();
		exit_game();


	}



	void MainGame::draw(){

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_batch3d->begin(); 

		
	
		I32 t_locations = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_locations, 0);

		I32 m_loc = m_program.getUniformLocation("MVP_matrix");
		glm::mat4 final_mat = m_camera->camera_matrix * m_game_object->model_matrix;
		glUniformMatrix4fv(m_loc, 1, GL_FALSE, &(final_mat[0][0]));
	
		m_batch3d->draw(*m_model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_model->texture.id);
		////orphan buffer
		//glBufferData(GL_ARRAY_BUFFER, m_model->vertices.size() * sizeof(Vertex3D), nullptr, GL_STATIC_DRAW);
		////upload the data
		//glBufferSubData(GL_ARRAY_BUFFER, 0, m_model->vertices.size() * sizeof(Vertex3D), m_model->vertices.data()); //data return pointer to first element somehow


		//glBindBuffer(GL_ARRAY_BUFFER, 0);



		//glDrawArrays(GL_TRIANGLES, 0, m_model->vertices.size());

		///* Swap our buffers to make our changes visible */

		m_batch3d->end();
		m_batch3d->render_batch();

		SDL_GL_SwapWindow(m_window);
		

	


	}
}