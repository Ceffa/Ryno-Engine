#include "Game.h"
#include "Scene.h"

namespace Ryno{

	void Game::init_SDL() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cout<<"Failed to initialize SDL: " + std::string(SDL_GetError())<<std::endl;
		}
	}
	void Game::init_external_systems(){


		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if ((window = SDL_CreateWindow("Ryno Engine", WindowPos::x, WindowPos::y, WindowSize::w, WindowSize::h, SDL_WINDOW_OPENGL)) == NULL){
			std::cout<<"Failed to create SDL window: "+ std::string(SDL_GetError())<<std::endl;
		}

		//MOUSE INITIALIZATIONS
		SDL_ShowCursor(GL_TRUE);

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

	    texture_manager = TextureManager::get_instance();
		deferred_renderer = DeferredRenderer::get_instance();
		deferred_renderer->init();
		shell = Shell::get_instance();
		shell->init();
		log = Log::get_instance();
		log->init();
	
		physics_world = PhysicsWorld::get_instance();
		physics_world->set_up();
		particle_manager = ParticleManager::get_instance();
		particle_manager->init();
		audio_manager = AudioManager::get_instance();
		audio_manager->init();
		time_manager = TimeManager::get_instance();
		time_manager->init(60);
		input_manager = InputManager::get_instance();
		input_manager->init(window);
		mesh_manager = MeshManager::get_instance();
		
		simple_drawer = SimpleDrawer::get_instance();
		mesh_builder = MeshBuilder::get_instance();

		network = Network::get_instance();
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
		set_scene(0);
	}

	void Game::run(){

		game_state = GameState::Running;
		scene->start();
		
		//Main Loop
		while (game_state != GameState::Exit){
			if (game_state == GameState::ChangeScene) {
				reset();
				scene->start();
				game_state = GameState::Running;
			}
			time_manager->begin_frame();
			handle_input();
			if (game_state == GameState::ChangeScene)
				continue;
			handle_network();
			scene->camera_update();
			if (game_state != GameState::Paused) update();
			physics_world->physics_step(TimeManager::delta_time);
			draw();
			time_manager->end_frame();
			Log::update_stats();
		}
		end();
		exit_game();

	}

	void Game::reset() {
		physics_world->reset();
	}
	void Game::set_scene(const std::string& scene_name)
	{
		set_scene(SceneManager::get_scene_nr(scene_name));
	}
	void Game::set_scene(I32 scene_nr)
	{
		if (!SceneManager::scene_exists(scene_nr))
			return;
		if (scene)
			destroy_scene(scene);
		scene = SceneManager::new_scene(scene_nr);
		deferred_renderer->set_camera(scene->camera.get());
		game_state = GameState::ChangeScene;
		shell->request_pause = false;
	
	}
	void Game::next_scene()
	{
		if (scene)
			destroy_scene(scene);
		scene = SceneManager::next_scene();
		deferred_renderer->set_camera(scene->camera.get());
		game_state = GameState::ChangeScene;
		shell->request_pause = false;

	}
	void Game::reset_scene()
	{
		if (scene)
			destroy_scene(scene);
		scene = SceneManager::reset_scene();
		deferred_renderer->set_camera(scene->camera.get());
		game_state = GameState::ChangeScene;
		shell->request_pause = false;
	}

	void Game::destroy_scene(Scene* s){
		particle_manager->reset();
		mesh_builder->reset();
		mesh_manager->reset();
		texture_manager->reset();
		audio_manager->reset();
		delete scene;
		scene = nullptr;
	}

	

	void Game::draw(){

		deferred_renderer->init_frame();
		deferred_renderer->fill_batches();
		deferred_renderer->geometry_pass();
		deferred_renderer->ssao_pass();
		deferred_renderer->directional_light_pass();
		deferred_renderer->point_light_pass();
		deferred_renderer->spot_light_pass();
		deferred_renderer->skybox_pass();
		deferred_renderer->post_processing_pass();
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
	
		delete log;
		delete shell;
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
		 else if (game_state != GameState::ChangeScene)
			game_state = GameState::Running;
		
		

		//Process user inputs
		 if (!shell->active && game_state != GameState::Paused && game_state!=GameState::ChangeScene) {
			 scene->input();
			 scene->input_scripts();
		 }
	}

		
	void Game::update()
	{
		particle_manager->update(TimeManager::delta_time);

		scene->update();
		scene->update_scripts();
	}


	void Game::handle_network() {
		network->update();
	}

	void Game::set_window_pos(U32 x, U32 y) {
		WindowPos::x = x;
		WindowPos::y = y;
	}
	void Game::set_window_size(U32 w, U32 h) {
		WindowSize::w = w;
		WindowSize::h = h;
	}

}