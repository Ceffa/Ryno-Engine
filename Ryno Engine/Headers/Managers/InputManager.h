#pragma once
#include <unordered_map>
#include "Global.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <SDL\SDL.h>


#define CONTROLLER_DEADZONE 8000
#define CONTROLLER_RANGE 32768

enum InputDevice{ KEYBOARD = 0x00000000, MOUSE = 0x22222222, CONTROLLER = 0x44444444 };



namespace Ryno{

	enum Input{OK,EXIT_REQUEST,NONE};
	class InputManager{


	public:
		~InputManager();
		static InputManager* get_instance();

		void init(SDL_Window* window);
		Input get_input();
		void key_press(U32 key, InputDevice device);
		void key_release(U32 key, InputDevice device);
		U8 is_key_down(U32 key, InputDevice device);
		U8 is_key_pressed(U32 key, InputDevice device);
		void set_mouse_coords(F32 x, F32 y);
		glm::vec2 get_mouse_coords() const { return m_mouse_coords; }
		glm::vec2 get_mouse_movement()  { 
			return m_mouse_coords - glm::vec2(WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/2.0f);
		}



		void set_controller_axis_coord(F32* axis, I32 value);
		

		glm::vec2 get_controller_LX_coords(){ return m_controller_LX_coords; }
		glm::vec2 get_controller_RX_coords(){ return m_controller_RX_coords; }

		void update();
		void reset();
		glm::vec2 m_mouse_coords;
		glm::vec2 m_controller_LX_coords;
		glm::vec2 m_controller_RX_coords;
		std::string frame_text;

	private:
		InputManager();

		SDL_Window* m_window;
		U8 was_key_down(U32 key, InputDevice device);
		std::unordered_map<U32, U8> m_key_map;
		std::unordered_map<U32, U8> m_previous_key_map;

	};
}
