#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "ErrorManager.h"

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
	To be called in the game loop after handle_input().
	It renders on screen.
	*/
	void draw();

	GameState m_game_state;
	SDL_Window* m_window;


	


	
};

