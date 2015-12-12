#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "TimeManager.h"
#include "Log.h"
#include "IConsole.h"
#include "AudioManager.h"
#include <list>
#include <vector>

#define NUM_LINES 10
#define HISTORY_LENGTH 50

namespace Ryno{
	class Shell : public IConsole{
		
	public:
		~Shell(){}
		static Shell* get_instance();
		void init();
		void show() override;
		void hide() override;
		void toggle() override;

		void process_input();
		void parse_input();
		void parse_command(const std::string& command);
		void rotate_lines();

		bool request_exit = false;
		bool restart_physics = false;
		bool request_pause = false;
		bool phys_step;


	protected:
		
		DeferredRenderer* deferred_renderer;
		TimeManager* time_manager;
		Log* log;
		
		std::string active_line;
		std::string base_path ="Ryno> ";
		U8 base_path_size;
		U32 active_line_size;
		U32 parse_starting_point;
		
	

	private:
		Shell(){}
		void set(bool b) override;
		
		void print_message(const std::string& message);
		std::string get_argument();
		std::string string_argument();
		I32 int_argument();
		F32 float_argument();
	};

}