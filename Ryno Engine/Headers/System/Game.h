#pragma once
//generic stuff
#include <iostream>
//graphic stuff
#include <SDL\SDL.h>
#include <GL\glew.h>
//managers
#include "MeshManager.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "ParticleManager.h"
#include "Log.h"
#include "MeshBuilder.h"
#include "CPUProfiler.h"
#include "GPUProfiler.h"
#include "PhysicsWorld.h"
#include "Network.h"

//other
#include "Global.h"
#include "Shader.h"
#include "GameObject.h"
#include "Batches/Batch3DGeometry.h"
#include "Batches/Batch3DShadow.h"
#include "Batches/Batch2DSprite.h"

#include "Camera3D.h"
#include "SimpleDrawer.h"
#include "DeferredRenderer.h"
#include "Shell.h"
#include "Log.h"

namespace Ryno{

	enum GameState
	{
		Running,
		ChangeScene,
		Paused,
		Exit,
		None
	};

	class Scene;
	class Game
	{
		friend class DeferredRenderer;
	public:

		static Game* get_instance();
		void init();
		void init_SDL();
		void set_window_pos(U32 x, U32 y);
		void set_window_size(U32 w, U32 h);
		void run();
		void set_scene(const std::string& scene_name);
		void set_scene(I32 scene_nr);

		void reset_scene();
		void reset();
		void next_scene();
		Scene* get_scene() { return scene; }


		AudioManager* audio_manager;
		InputManager* input_manager;
		TimeManager* time_manager;
		TextureManager* texture_manager;
		MeshManager* mesh_manager;
		SimpleDrawer* simple_drawer;
		DeferredRenderer* deferred_renderer;
		ParticleManager* particle_manager;
		MeshBuilder* mesh_builder;
		PhysicsWorld* physics_world;
		Network* network;
		Shell* shell;
		Log* log;
		GameState game_state;

		F32 delta_time = 0;
		F32 time = 0;


	protected:

		void destroy_scene(Scene* s);

		void init_external_systems();
		void init_internal_systems();


		void handle_input();//make initializations, then call input

		void handle_network();
		void update();

		void draw();

		void end();

		void exit_game();

		Scene *scene = nullptr;
		Scene *new_scene = nullptr;

		SDL_Joystick *game_controller;

		SDL_Window* window;


	private:
		Game::Game(){}
	};
}
