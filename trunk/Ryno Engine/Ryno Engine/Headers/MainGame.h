#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "Log.h"
#include "GLSLProgram.h"
#include "GameObject.h"
#include "Batch3D.h"
#include "Camera3D.h"
#include "Global.h"
#include "MeshLoader.h"
namespace Ryno{
	class MainGame
	{
	public:
		/**
		It handles the game cycle
		*/
		void run();

	private:

		enum GameState
		{
			Running,
			Exit,
			None
		};

		/**
		Initialize SDL, OpenGL, glew, doublebuffer etc..
		Needs to be called before everything else.
		*/
		void init_systems();

		/**
		To be called at the beginning of the game loop.
		It handles the various input that SDL can handle.
		I will wrap everything up with a InputManager in the future.
		*/
		void handle_input();

		/**
		To be called in the game loop before draw().
		It update the logic of the game.
		*/
		void update();

		/**
		To be called after init_system in run(). 
		Initialize here objects and stuff for the game
		*/
		void start();

		/**
		To be called in the game loop after handle_input().
		It renders on screen.
		*/
		void draw();

		/**
		Destroy stuff before exiting.
		Very important SDL_Quit()
		*/
		void exit_game();


		/**
		Load shader file inside a buffer
		@param: file path
		*/
		C* load_shader(C *file);


	
		MeshLoader& m_mesh_loader = MeshLoader::get_instance(); 
		Camera3D* m_camera;
		GameState m_game_state;
		std::vector<GameObject*> m_game_objects;
		GLSLProgram m_program;
		SDL_Window* m_window;
		Model* m_model;
		Batch3D* m_batch3d;
		U32 vao, vbo;

	};
}
