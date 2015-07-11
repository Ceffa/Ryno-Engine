#include "InputManager.h"

namespace Ryno {

	InputManager::InputManager() : m_mouse_coords(0.0f)
	{
		
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::init(SDL_Window* window){
		m_window = window;
		SDL_WarpMouseInWindow(m_window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

	}

	Input InputManager::get_input(){
		static bool exit = false;
		SDL_Event evnt;
		//Will keep looping until there are no more events to process
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT:
				return Input::EXIT_REQUEST;
				break;
			case SDL_MOUSEMOTION:

				set_mouse_coords((F32)evnt.motion.x, (F32)evnt.motion.y);

				break;
			case SDL_KEYDOWN:
				key_press(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				key_release(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				key_press(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				key_release(evnt.button.button);
				break;
		
			}
		}
		if (is_key_pressed(SDLK_ESCAPE))exit = !exit;
		if(!exit)SDL_WarpMouseInWindow(m_window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

		return Input::OK;
	}

	void InputManager::update() {
		// Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
		for (auto& it : m_key_map) {
			m_previous_key_map[it.first] = it.second;
		}

		
	}

	void InputManager::key_press(U32 keyID) {
		// Here we are treating _keyMap as an associative array.
		// if keyID doesn't already exist in _keyMap, it will get added
		m_key_map[keyID] = true;
	}

	void InputManager::key_release(U32 keyID) {
		m_key_map[keyID] = false;
	}

	void InputManager::set_mouse_coords(F32 x, F32 y) {
		
		m_mouse_coords.x = x;
		m_mouse_coords.y = y;
	}

	

	U8 InputManager::is_key_down(U32 keyID) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_key_map.find(keyID);
		if (it != m_key_map.end()) {
			// Found the key
			return it->second;
		}
		else {
			// Didn't find the key
			return false;
		}
	}

	U8 InputManager::is_key_pressed(U32 keyID) {
		// Check if it is pressed this frame, and wasn't pressed last frame
		if (is_key_down(keyID) == true && was_key_down(keyID) == false) {
			return true;
		}
		return false;
	}

	U8 InputManager::was_key_down(U32 keyID) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_previous_key_map.find(keyID);
		if (it != m_previous_key_map.end()) {
			// Found the key
			return it->second;
		}
		else {
			// Didn't find the key
			return false;
		}
	}
}
