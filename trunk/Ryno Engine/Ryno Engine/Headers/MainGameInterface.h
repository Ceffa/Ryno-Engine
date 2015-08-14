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
#include "SimpleDrawer.h"
#include "Log.h"
//other
#include "Global.h"
#include "GLSLProgram.h"
#include "GameObject.h"
#include "Batch3D.h"
#include "Camera3D.h"


namespace Ryno{
	class MainGameInterface
	{
	public:
		
		void run();

	protected:

		enum GameState
		{
			Running,
			Exit,
			None
		};

		
		void init_external_systems();
		void init_internal_systems();

		virtual void start() = 0;

		void handle_input();//make initializations, then call input
		virtual void input() = 0;

		virtual void update() = 0;

		virtual void draw() = 0;

		virtual void end() = 0;

		void exit_game();


		U32 vao, vbo;

		AudioManager m_audio_manager;
		InputManager m_input_manager;
		TimeManager m_time_manager;
		TextureManager* m_texture_manager;
		MeshManager* m_mesh_manager;
		SimpleDrawer* m_simple_drawer;
		Camera3D* m_camera;
		GameState m_game_state;
		SDL_Window* m_window;
		Batch3D* m_batch3d;

	};
}
