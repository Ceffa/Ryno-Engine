#include "MainGameInterface.h"


namespace Ryno{
	void MainGameInterface::init_external_systems(){

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			Log::FatalError("Failed to initialize SDL: ", SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		if ((m_window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL){
			Log::FatalError("Failed to create SDL window: ", SDL_GetError());
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
			Log::FatalError("Failed to create GL_context: ", SDL_GetError());
		}

		glewExperimental = GLU_TRUE;
		GLenum error;
		if ((error = glewInit()) != GLEW_OK){
			Log::FatalError("Failed to initialize glew.");
		}

		
	//	glEnable(GL_MULTISAMPLE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe

		SDL_GL_SetSwapInterval(0); //disable vsync

	

		m_game_state = GameState::Running;
	}

	void MainGameInterface::init_internal_systems(){

		m_audio_manager.init();
		m_time_manager.init(60);
		m_input_manager.init(m_window);
		m_camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);


	    m_texture_manager = TextureManager::get_instance();
		m_mesh_manager = MeshManager::get_instance();
		m_deferred_renderer = new DeferredRenderer();
		m_deferred_renderer->init(m_camera);
		m_simple_drawer = SimpleDrawer::get_instance();
		
		
				
	}

	void MainGameInterface::run(){
		init_external_systems();
		init_internal_systems();
		start();

		while (m_game_state == GameState::Running){
			m_time_manager.begin_frame();
			handle_input();
			update();
			draw();
			m_time_manager.end_frame();
			m_time_manager.print_fps();

		}
		end();
		exit_game();


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
		m_input_manager.update();
		if (m_input_manager.get_input() == Input::EXIT_REQUEST)
			m_game_state = GameState::Exit;
		input();
	}

		
}