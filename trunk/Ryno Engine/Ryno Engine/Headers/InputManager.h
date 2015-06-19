#pragma once
#include <unordered_map>
#include "Types.h"
#include <GLM/glm.hpp>


namespace Ryno{

	enum Input{OK,EXIT_REQUEST,NONE};
	class InputManager{


	public:
		InputManager();
		~InputManager();

		Input get_input();
		void key_press(U32 key);
		void key_release(U32 key);
		U8 is_key_down(U32 key);
		U8 is_key_pressed(U32 key);
		void set_mouse_coords(F32 x, F32 y);
		glm::vec2 get_mouse_coords() const { return m_mouse_coords; }
		void update();


	private:
		U8 was_key_down(U32 key);
		std::unordered_map<U32, U8> m_key_map;
		std::unordered_map<U32, U8> m_previous_key_map;

		glm::vec2 m_mouse_coords;

	};
}
