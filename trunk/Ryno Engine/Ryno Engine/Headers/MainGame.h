#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "ErrorManager.h"
#include "GLSLProgram.h"
#include "Model.h"

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

	GameState m_game_state;
	GLSLProgram m_program;
	SDL_Window* m_window;
	Model* m_model;
	U32 vao, vbo;


	


	
};

