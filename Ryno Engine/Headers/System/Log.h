#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "GUIObject.h"
#include <list>


#define NUM_LINES 20
#define HISTORY_LENGTH 50

namespace RynoEngine{
	class Log{
		friend class Shell;
	public:
		
		~Log(){}
		static Log* get_instance();
		void init();
		void show();
		void hide();
		void toggle();
		void set_text_color(U8 r, U8 g, U8 b);

		bool active = true;
		bool fps_log = false;

		static void print(const std::string& message);
		static void print(F32 f);
		static void print(const glm::vec3& vec);
		static void println(const std::string& message);
		static void println(F32 f);
		static void println(const glm::vec3& vec);

		



	
	private:
		Log(){}
		InputManager* input_manager;
		Font font;
		GUIObject background;

		GUIObject lines[NUM_LINES];
		std::list<std::string> history;
		std::list<std::string>::iterator iterator;
		U32 history_length;
		static Log* instance;
		void set(bool b);
		void refresh();
		void read_up();
		void read_down();
		void read_beginning();
		void read_end();
	};

}