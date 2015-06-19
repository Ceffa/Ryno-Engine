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
		if ((m_window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL){
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
		SDL_GL_SetSwapInterval(0); //disable vsync


		m_game_state = GameState::Running;
	}

	void MainGame::exit_game(){
		SDL_Quit();
		exit(0);
	}

	

	void MainGame::update(){

		for (GameObject* o : m_game_objects)
			o->generate_model_matrix();


	}

	void MainGame::start(){
		
		m_time_manager.init(300);

		m_camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);
		
		m_batch3d = new Batch3D();
		
		U32 texture_metal = m_texture_loader.loadPNG("metal");
		U32 texture_wood = m_texture_loader.loadPNG("wood");
		I32 cube_mesh = m_mesh_loader.load_mesh("cube");
		I32 sphere_mesh = m_mesh_loader.load_mesh("sphere");
		I32 rock_mesh = m_mesh_loader.load_mesh("rock");
		for (I32 i = 0; i < 30; i++){
			for (I32 j = 0; j < 30; j++){
				for (I32 k = 0; k < 30; k++){

					GameObject* new_go = new GameObject();
					new_go->position = glm::vec3(i *1.2f, j*1.2f, k*1.2f);
					//if (((i+j)%2)==0)
					//	new_go->model.mesh = sphere_mesh;
					//else
					new_go->model.mesh = cube_mesh;

					if (((i + j) % 2) == 0)
						new_go->model.texture = texture_metal;
						else
					new_go->model.texture = texture_wood;
					new_go->scale = glm::vec3(1, 1, 1);
					m_game_objects.push_back(new_go);

				}
			}
		}
	


		m_camera->position = glm::vec3(0,0, -3);
		m_camera->scale = glm::vec3(1, 1, 1);

		m_batch3d->init(m_camera);
		
		m_program.create("prova");
		

	

		


	}

	void MainGame::handle_input(){
		m_input_manager.update();
		if (m_input_manager.get_input() == Input::EXIT_REQUEST)
			m_game_state = GameState::Exit;

		if (m_input_manager.is_key_down(SDLK_d)){
			m_camera->position += glm::vec3(.5f, 0, 0);
		}
		if (m_input_manager.is_key_down(SDLK_a)){
			m_camera->position -= glm::vec3(.5f, 0, 0);
		}
		if (m_input_manager.is_key_down(SDLK_w)){
			m_camera->position += glm::vec3(0,.5f, 0);
		}
		if (m_input_manager.is_key_down(SDLK_s)){
			m_camera->position -= glm::vec3(0, .5f, 0);
		}
		if (m_input_manager.is_key_down(SDLK_q)){
			m_camera->scale += glm::vec3(0,0,.1f);
		}
		if (m_input_manager.is_key_down(SDLK_e)){
			m_camera->scale -= glm::vec3(0,0,.1f);
		}
	}

	void MainGame::run(){
		init_systems();
		start();
				
		while (m_game_state == GameState::Running){
			m_time_manager.begin_frame();
			handle_input();
			update();
			m_program.use();
			draw();
			m_program.unuse();
			m_time_manager.end_frame();
			m_time_manager.print_fps();

		}
		m_program.destroy();
		exit_game();


	}



	void MainGame::draw(){

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_batch3d->begin(); 

		
	
		
		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		
		m_batch3d->end();


		I32 t_locations = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_locations, 0);


		m_batch3d->render_batch();

		SDL_GL_SwapWindow(m_window);


	}
}