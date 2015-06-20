#pragma once
#include <unordered_map>
#include "Types.h"
#include "Global.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <SDL\SDL.h>



namespace Ryno{

	enum Input{OK,EXIT_REQUEST,NONE};
	class InputManager{


	public:
		InputManager();
		~InputManager();

		void init(SDL_Window* window);
		Input get_input();
		void key_press(U32 key);
		void key_release(U32 key);
		U8 is_key_down(U32 key);
		U8 is_key_pressed(U32 key);
		void set_mouse_coords(F32 x, F32 y);
		glm::vec2 get_mouse_coords() const { return m_mouse_coords; }
		glm::vec2 get_mouse_movement()  { 
			//std::cout << glm::to_string(m_mouse_coords) << std::endl;
			return m_mouse_coords - glm::vec2(WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/2.0f);
		}
		
		void update();
		glm::vec2 m_mouse_coords;

	private:
		SDL_Window* m_window;
		U8 was_key_down(U32 key);
		std::unordered_map<U32, U8> m_key_map;
		std::unordered_map<U32, U8> m_previous_key_map;

		

	};
}
