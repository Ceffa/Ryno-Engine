#include "MainGameInterface.h"


namespace Ryno{
	void MainGameInterface::init_external_systems(){

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cout<<"Failed to initialize SDL: " + std::string(SDL_GetError())<<std::endl;
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		if ((m_window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL){
			std::cout<<"Failed to create SDL window: "+ std::string(SDL_GetError())<<std::endl;
		}

		//MOUSE INITIALIZATIONS
		SDL_ShowCursor(GL_FALSE);

		//JOYSTICK INITIALIZATIONS
		SDL_Joystick *joy;
		if (SDL_NumJoysticks() > 0){
			joy = SDL_JoystickOpen(0);
			SDL_JoystickEventState(SDL_ENABLE);
		}


		SDL_GLContext gl_context;
		if ((gl_context = SDL_GL_CreateContext(m_window)) = nullptr){
			std::cout<<"Failed to create GL_context: " + std::string(SDL_GetError())<<std::endl;
		}

		glewExperimental = GLU_TRUE;
		GLenum error;
		if ((error = glewInit()) != GLEW_OK){
			std::cout << "Failed to initialize glew." << std::endl;
		}

		
	//	glEnable(GL_MULTISAMPLE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe

		SDL_GL_SetSwapInterval(0); //disable vsync

	

		m_game_state = GameState::Running;
	}

	void MainGameInterface::init_internal_systems(){

		
	
		
		m_camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);

		shell = Shell::get_instance();
		shell->init();
		log = Log::get_instance();
		log->init();
		m_audio_manager = AudioManager::get_instance();
		m_audio_manager->init();
		m_time_manager = TimeManager::get_instance();
		m_time_manager->init(60);
	    m_texture_manager = TextureManager::get_instance();
		m_input_manager = InputManager::get_instance();
		m_input_manager->init(m_window);
		m_mesh_manager = MeshManager::get_instance();
		m_deferred_renderer = DeferredRenderer::get_instance();
		m_deferred_renderer->init(m_camera);
		m_simple_drawer = SimpleDrawer::get_instance();

		
		
				
	}

	void MainGameInterface::run(){
		init_external_systems();
		init_internal_systems();
		start();

		while (m_game_state != GameState::Exit){
			m_time_manager->begin_frame();
			handle_input();
			if (m_game_state != GameState::Paused) m_camera->generate_VP_matrix();
			if (m_game_state != GameState::Paused) update();
			draw();
			delta_time = m_time_manager->end_frame();
			if (m_game_state != GameState::Paused) m_time_manager->print_fps();

		}
		end();
		exit_game();


	}

	void MainGameInterface::draw(){

		m_deferred_renderer->init_frame();
		m_deferred_renderer->geometry_pass();
		m_deferred_renderer->prepare_for_light_passes();
		m_deferred_renderer->spot_light_pass();
		m_deferred_renderer->point_light_pass();
		m_deferred_renderer->directional_light_pass();
		m_deferred_renderer->skybox_pass();
		m_deferred_renderer->draw_HUD_pass();
		m_deferred_renderer->final_pass();

		SDL_GL_SwapWindow(m_window);

	}

	void MainGameInterface::end()
	{
		SDL_JoystickClose(game_controller);
		m_deferred_renderer->destroy();
	}

	void MainGameInterface::exit_game(){
		SDL_Quit();
		exit(0);
	}	

	void MainGameInterface::handle_input(){

		//Reads input from user
		m_input_manager->update();
		//Exits if requested
		if (m_input_manager->get_input() == Input::EXIT_REQUEST){
			m_game_state = GameState::Exit;
			return;
		}

		//Process console inputs
		shell->process_input();

		 if (shell->request_exit)
			 m_game_state = GameState::Exit;
		 else if (shell->request_pause)
			 m_game_state = GameState::Paused;
		 else
			 m_game_state = GameState::Running;
		 

		//Process user inputs
		if (m_game_state != GameState::Paused) input();
	}

		
}