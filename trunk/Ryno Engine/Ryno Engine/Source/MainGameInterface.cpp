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
	//	glEnable(GL_MULTISAMPLE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe
		SDL_GL_SetSwapInterval(1); //disable vsync

	

		m_game_state = GameState::Running;
	}

	void MainGameInterface::init_internal_systems(){
		m_frame_buffer = new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

		m_audio_manager.init();
		m_time_manager.init(60);
		m_input_manager.init(m_window);
		m_camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_batch3d = new Batch3D();
		
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
		m_program.destroy();
		m_program_dir.destroy();
		exit_game();


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