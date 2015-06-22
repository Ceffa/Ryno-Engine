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

		SDL_ShowCursor(GL_FALSE);
	

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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		SDL_GL_SetSwapInterval(1); //disable vsync


		m_game_state = GameState::Running;
	}

	void MainGame::exit_game(){
		SDL_Quit();
		exit(0);
	}


	void MainGame::update(){
		U64 time = SDL_GetTicks();
		I32 i = 0;
		for (GameObject* o : m_game_objects){

			
	
			
			o->position.y =  5 * sin((time) / 300.0f) * sin(o->position.x / 5.0f) + 5 * sin(time / 600.0f) * sin(o->position.z / 5.0f);
			o->model.set_color(255.0f, (o->position.y + 10.0f)*255.0f / 20.0f,0.0f, 255.0f);
			//o->position.y = sin(time/400.0f) * (pow(o->position.x, 3)*o->position.z + pow(o->position.z, 3)*o->position.x) / 50000.0f;
			//o->model.set_color(0, 255, (o->position.y + 125.0f)*255.0f / 250.0f, 255);
			
			/*F32 ticks = .1f * time / sqrt(i);
			o->position = glm::vec3(10*cos(ticks), i/20.0f, 10*sin(ticks));
			o->model.set_color(255.0f, i/12500.0f*255.0f, 0.0f, 255.0f);
			i++;*/
			o->generate_model_matrix();
		}

	}

	void MainGame::start(){
		
		m_time_manager.init(60);
		m_input_manager.init(m_window);

		m_camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);
		
		m_batch3d = new Batch3D();
		
		U32 texture_metal = m_texture_loader.loadPNG("white");
		
		I32 cube_mesh = m_mesh_loader.load_mesh("cube");
	
		for (I32 x = -50; x < 51; x++){
			for (I32 z = -50; z <51; z++){
				

					GameObject* new_go = new GameObject();
					new_go->position = glm::vec3(x,0,z);
					
					new_go->model.mesh = cube_mesh;
					
					new_go->model.texture = texture_metal;
						
					
					new_go->scale = glm::vec3(1, 1, 1);
					new_go->model.set_color(255,255,255, 255);
					m_game_objects.push_back(new_go);

				
			}
		}
	


		m_camera->position = glm::vec4(0,0, -3,1);

		m_batch3d->init(m_camera);
		
		m_program.create("prova");
		

	

		


	}

	void MainGame::handle_input(){
		m_input_manager.update();
		if (m_input_manager.get_input() == Input::EXIT_REQUEST)
			m_game_state = GameState::Exit;
		//std::cout << glm::to_string(m_input_manager.m_mouse_coords)<< std::endl;
		if (m_input_manager.is_key_down(SDLK_d)){
			m_camera->move_right(1.0f);

		}
		if (m_input_manager.is_key_down(SDLK_a)){
			m_camera->move_left(1.0f);
		}
		if (m_input_manager.is_key_down(SDLK_w) || m_input_manager.is_key_down(SDL_BUTTON_LEFT)){
			m_camera->move_forward(1.0f);
		}
		if (m_input_manager.is_key_down(SDLK_s) || m_input_manager.is_key_down(SDL_BUTTON_RIGHT)){
			m_camera->move_back(1.0f);
		}
		

		glm::vec2 mouse_coords = m_input_manager.get_mouse_movement();
		m_camera->rotate(.005f* mouse_coords.x, .005f* mouse_coords.y);

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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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