#include "InputManager.h"
#include <iostream>
#include <string>


namespace Ryno {

	InputManager::InputManager() : m_mouse_coords(0.0f)
	{
		
	}


	InputManager::~InputManager()
	{
	}

	InputManager* InputManager::get_instance(){
		static InputManager instance;//only at the beginning
		return &instance;
	}


	void InputManager::init(SDL_Window* window){
		m_window = window;
		SDL_WarpMouseInWindow(m_window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

	}

	Input InputManager::get_input(){
		static bool exit = false;

		frame_text.resize(0);

		SDL_Event evnt;
		SDL_StartTextInput();

		//Will keep looping until there are no more events to process
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT:
				return Input::EXIT_REQUEST;
				break;
			case SDL_MOUSEMOTION:

				set_mouse_coords((F32)evnt.motion.x, (F32)evnt.motion.y);

				break;
			case SDL_TEXTINPUT:
				frame_text += evnt.text.text;
				break;
			case SDL_KEYDOWN:
				key_press(evnt.key.keysym.sym , KEYBOARD);
				break;
			case SDL_KEYUP:
				key_release(evnt.key.keysym.sym , KEYBOARD);
				break;
			case SDL_MOUSEBUTTONDOWN:
				key_press(evnt.button.button , MOUSE);
				break;
			case SDL_MOUSEBUTTONUP:
				key_release(evnt.button.button , MOUSE);
				break;
			case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
				key_press(evnt.jbutton.button , CONTROLLER);
				break;
			case SDL_JOYBUTTONUP:  /* Handle Joystick Button Presses */
				key_release(evnt.jbutton.button , CONTROLLER);
				break;
			case SDL_JOYAXISMOTION:
				
				switch (evnt.jaxis.axis){
				case 0: set_controller_axis_coord(&m_controller_LX_coords.x, evnt.jaxis.value); break;
				case 1: set_controller_axis_coord(&m_controller_LX_coords.y, evnt.jaxis.value); break;
				case 2: set_controller_axis_coord(&m_controller_RX_coords.x, evnt.jaxis.value); break;
				case 3: set_controller_axis_coord(&m_controller_RX_coords.y, evnt.jaxis.value); break;
				}
				
					
			}
		}

		SDL_StopTextInput();

		if (is_key_pressed(SDLK_ESCAPE,KEYBOARD))exit = !exit;
		if(!exit)SDL_WarpMouseInWindow(m_window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

		return Input::OK;
	}

	void InputManager::update() {
		// Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
		for (auto& it : m_key_map) {
			m_previous_key_map[it.first] = it.second;
		}

		
	}

	void InputManager::key_press(U32 keyID, InputDevice device) {
		// Here we are treating _keyMap as an associative array.
		// if keyID doesn't already exist in _keyMap, it will get added
		m_key_map[keyID + device] = true;
	}

	void InputManager::key_release(U32 keyID, InputDevice device) {
		m_key_map[keyID+device] = false;
	}

	void InputManager::set_mouse_coords(F32 x, F32 y) {
		
		m_mouse_coords.x = x;
		m_mouse_coords.y = y;
	}

	

	void InputManager::set_controller_axis_coord(F32* axis, I32 value)
	{
		
		*axis = abs(value) <  CONTROLLER_DEADZONE ? 0 : (F32)value / (F32)CONTROLLER_RANGE;
		
	}


	U8 InputManager::is_key_down(U32 keyID, InputDevice device) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_key_map.find(keyID + device);
		if (it != m_key_map.end()) {
			// Found the key
			return it->second;
		}
		else {
			// Didn't find the key
			return false;
		}
	}

	U8 InputManager::is_key_pressed(U32 keyID,InputDevice device) {
		// Check if it is pressed this frame, and wasn't pressed last frame
		if (is_key_down(keyID,device) == true && was_key_down(keyID,device) == false) {
			return true;
		}
		return false;
	}

	U8 InputManager::was_key_down(U32 keyID, InputDevice device) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_previous_key_map.find(keyID + device);
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
