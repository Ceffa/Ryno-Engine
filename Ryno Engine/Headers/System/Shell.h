#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "TimeManager.h"
#include "Log.h"
#include "GUIObject.h"
#include "AudioManager.h"
#include <list>
#include <vector>
#include <map>
#include <functional>

#define NUM_LINES 10
#define HISTORY_LENGTH 50

namespace Ryno{
	class Shell{
		
	public:
		~Shell(){}
		static Shell* get_instance();
		void init();
		void show();
		void hide();
		void toggle();

		void process_input();
		void parse_input();
		void parse_command(const std::string& command);
		void rotate_lines();

		bool request_exit = false;
		bool request_pause = false;
		bool active = true;


	private:
		Shell(){}

		std::map < std::string, std::function<std::string(bool hint)>> commands;
		void create_commands();

		DeferredRenderer* deferred_renderer;
		TimeManager* time_manager;
		Log* log;
		
		std::string active_line;
		std::string base_path ="Ryno> ";
		U8 base_path_size;
		U32 active_line_size;
		U32 parse_starting_point;
		void set_text_color(U8 r, U8 g, U8 b);

		GUIObject background;
	
		InputManager* input_manager;
		Font font;
		GUIObject lines[NUM_LINES];
		std::list<std::string> history;
		std::list<std::string>::iterator iterator;
		U32 history_length;
		void set(bool b);
		
		void print_message(const std::string& message);
		std::string get_argument();
		std::string string_argument(bool& error);
		I32 int_argument(bool& error);
		F32 float_argument(bool& error);

		static Shell* instance;


	};

}