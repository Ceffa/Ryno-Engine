#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "Sprite.h"
#include "Text.h"
#include <list>

#define SHELL_NUM_LINES 15

namespace Ryno{
	class Shell{
		
	public:
		
		static void init();
		static void show();
		static void hide();
		static void toggle();
		static void process_input();
		static void parse_input();
		static void parse_command(const std::string& command);
		static void rotate_lines();

		static bool active;

	protected:
		static DeferredRenderer* deferred_renderer;
		static InputManager* input_manager;
		static Sprite* background;
		static Font* font;
		static Text* lines[SHELL_NUM_LINES];
		
		static std::string shell_path;
		static std::string input;
		static U8 path_size;
		static U32 line_0_size;
		static U32 parse_from;

	

	private:
		static void set(bool b);
		static void print_message(const std::string& message);
		static std::string get_argument();
		static std::string string_argument();

		static I32 int_argument();
		


	};

}