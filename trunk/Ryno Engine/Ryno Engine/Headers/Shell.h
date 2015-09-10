#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "Sprite.h"
#include "Text.h"
#include <vector>

#define SHELL_NUM_LINES 15

namespace Ryno{
	
	class Shell{
	public:
		
		void init();
		void show();
		void hide();
		void toggle();
		void process_input();
		void parse_input();
		void parse_command(const std::string& command);
		void rotate_lines();
		
	protected:

		InputManager* im;
		bool active = true;
		Sprite* background;
		Font* font;
		Text* lines[SHELL_NUM_LINES];
		std::string shell_path = "Ryno> ";
		U8 path_size;
		U32 line_0_size;
		U32 parse_from;

	

	private:
		void set(bool b);
		void print_message(const std::string& message);
		std::string get_argument();
		std::string string_argument();

		I32 int_argument();


	};

}