#include "Game.h"
#include "Scene.h"

namespace Ryno{
	void Game::init_external_systems(){

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cout<<"Failed to initialize SDL: " + std::string(SDL_GetError())<<std::endl;
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		if ((window = SDL_CreateWindow("Ryno Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL){
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
		if ((gl_context = SDL_GL_CreateContext(window)) = nullptr){
			std::cout<<"Failed to create GL_context: " + std::string(SDL_GetError())<<std::endl;
		}

		glewExperimental = GLU_TRUE;
		GLenum error;
		if ((error = glewInit()) != GLEW_OK){
			std::cout << "Failed to initialize glew." << std::endl;
		}


		SDL_GL_SetSwapInterval(0); //disable vsync

	

		game_state = GameState::Running;
	}

	void Game::init_internal_systems(){

		
	
		
		camera = new Camera3D(WINDOW_WIDTH, WINDOW_HEIGHT);

		shell = Shell::get_instance();
		shell->init();
		log = Log::get_instance();
		log->init();

		//Memory
		/*Allocator::stack_allocator = StackAllocator::get_instance();
		Allocator::reference_allocator = ReferenceAllocator::get_instance();
		Allocator::pool_allocator = Pool::get_instance();
		Allocator::stack_allocator->init(1024);
		Allocator::reference_allocator->init();
		Allocator::pool_allocator->init();*/

		particle_manager = ParticleManager::get_instance();
		particle_manager->init();
		audio_manager = AudioManager::get_instance();
		audio_manager->init();
		time_manager = TimeManager::get_instance();
		time_manager->init(60);
	    texture_manager = TextureManager::get_instance();
		input_manager = InputManager::get_instance();
		input_manager->init(window);
		mesh_manager = MeshManager::get_instance();
		deferred_renderer = DeferredRenderer::get_instance();
		deferred_renderer->init(camera);
		simple_drawer = SimpleDrawer::get_instance();
		mesh_builder = MeshBuilder::get_instance();
		
		
				
	}

	Game* Game::get_instance()
	{
		static Game game;
		return &game;
	}

	void Game::init()
	{
		init_external_systems();
		init_internal_systems();
	}

	void Game::run(){
	
		while (game_state != GameState::Exit){
			select_scene();
			time_manager->begin_frame();
			handle_input();
			camera_update();
			if (game_state != GameState::Paused) scene->update();
			draw();
			delta_time = time_manager->end_frame();
			if (game_state != GameState::Paused) time_manager->print_fps();
		}
		end();
		exit_game();


	}

	void Game::set_scene(Scene* _scene)
	{
			new_scene = _scene;
	}

	//Called the next frame to avoid problems.
	void Game::select_scene(){
		if (new_scene != scene){
			if (scene)
				destroy_scene(scene);
			scene = new_scene;
			scene->start();
		}
	}

	void Game::destroy_scene(Scene* s){
		particle_manager->reset();
		texture_manager->reset();
		audio_manager->reset();
		CPUProfiler::reset();
		GPUProfiler::reset();
		mesh_builder->reset();
		delete scene;
	}

	void Game::camera_update()
	{
		if (!shell->active){
			

			if (input_manager->is_key_down(SDLK_d, KEYBOARD)){
				camera->move_right(0.5f * delta_time);

			}
			if (input_manager->is_key_down(SDLK_a, KEYBOARD)){
				camera->move_left(0.5f * delta_time);
			}
			if (input_manager->is_key_down(SDLK_w, KEYBOARD) || input_manager->is_key_down(SDL_BUTTON_LEFT, MOUSE)){
				camera->move_forward(0.5f * delta_time);
			}
			if (input_manager->is_key_down(SDLK_s, KEYBOARD) || input_manager->is_key_down(SDL_BUTTON_RIGHT, MOUSE)){
				camera->move_back(0.5f * delta_time);
			}
		}
		camera->generate_VP_matrix();
		glm::vec2 mouse_coords = input_manager->get_mouse_movement();
		camera->rotate(0.0005f * mouse_coords.x * delta_time, 0.0005f* mouse_coords.y* delta_time);
		glm::vec2 rotation_view = input_manager->get_controller_RX_coords();
		camera->rotate(0.01f * rotation_view.x* delta_time, 0.01f* rotation_view.y* delta_time);
		glm::vec2 direction = input_manager->get_controller_LX_coords();
		camera->move_forward(delta_time *1.0f * -direction.y);
		camera->move_right(delta_time * 1.0f * direction.x);
	}

	void Game::draw(){

		deferred_renderer->init_frame();
		deferred_renderer->geometry_pass();
		deferred_renderer->spot_light_pass();
		deferred_renderer->point_light_pass();
		deferred_renderer->directional_light_pass();
		deferred_renderer->skybox_pass();
		deferred_renderer->GUI_pass();
		deferred_renderer->final_pass();

		SDL_GL_SwapWindow(window);

	}

	void Game::end()
	{
		SDL_JoystickClose(game_controller);
		deferred_renderer->destroy();
	}

	void Game::exit_game(){
		SDL_Quit();
		exit(0);
	}	

	void Game::handle_input(){

		//Reads input from user
		input_manager->update();
		//Exits if requested
		if (input_manager->get_input() == Input::EXIT_REQUEST){
			game_state = GameState::Exit;
			return;
		}

		//Process console inputs
		shell->process_input();

		 if (shell->request_exit)
			 game_state = GameState::Exit;
		 else if (shell->request_pause)
			 game_state = GameState::Paused;
		 else
			 game_state = GameState::Running;
		 

		//Process user inputs
		if (game_state != GameState::Paused) scene->input();
	}

		
}