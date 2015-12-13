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
#include "Mallocator.h"
#include "StackAllocator.h"
#include "CPUProfiler.h"
#include "GPUProfiler.h"

//other
#include "Global.h"
#include "GLSLProgram.h"
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
		Paused,
		Exit,
		None
	};

	class Scene;
	class Game
	{

	public:

		static Game* get_instance();
		void init();
		void run();
		void set_scene(Scene* _scene);

		AudioManager* audio_manager;
		InputManager* input_manager;
		TimeManager* time_manager;
		TextureManager* texture_manager;
		MeshManager* mesh_manager;
		SimpleDrawer* simple_drawer;
		DeferredRenderer* deferred_renderer;
		ParticleManager* particle_manager;
		MeshBuilder* mesh_builder;
		Mallocator* mallocator;
		Shell* shell;
		Log* log;
		GameState game_state;

		F32 delta_time = 0;

	protected:

		void select_scene();

		void destroy_scene(Scene* s);
		void init_external_systems();
		void init_internal_systems();


		void handle_input();//make initializations, then call input

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
