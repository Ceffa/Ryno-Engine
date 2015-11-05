#include "Log.h"
#include "TextureManager.h"
#include <iostream>
#include <sstream>


#define MISSING_INT INT_MIN-1
#define ERROR_INT INT_MIN

namespace Ryno {



	Log Log::instance;

	Log* Log::get_instance()
	{
		return &instance;
	}

	void Log::init()
	{
		IConsole::init();

		input_manager = InputManager::get_instance();
		
		background->anchor_point = TOP_LEFT;
		background->set_position(0, 1);
		background->set_scale(350, WINDOW_HEIGHT / 1.506f);

		//Create Texts
		lines.resize(NUM_LINES);

		lines[0] = new Text();
		lines[0]->anchor_point = BOTTOM_LEFT;
		lines[0]->font = font;
		lines[0]->text = "";
		lines[0]->set_scale(0.7f,0.7f);
		lines[0]->depth = 4;
		lines[0]->set_color(255, 230, 0, 255);
		lines[0]->set_position(0.005f, .34f);
		lines[0]->use = SHELL;


		for (U8 i = 1; i < NUM_LINES; i++)
		{
			lines[i] = new Text(lines[0]);
			lines[i]->set_position(0.005f, .34f + 0.66f * i / NUM_LINES);
		}

		
		Log::print("");
	
		
	}

	void Log::set(bool b)
	{
		active = b;
		for (Text* t : lines)
			t->active = b;
		background->active = b;
	}

	void Log::show()
	{
		set(true);
	}

	void Log::hide()
	{
		set(false);
	}


	void Log::toggle()
	{
		if (active)
			hide();
		else
			show();
	}

	

	

	void Log::refresh()
	{
		
		auto temp = iterator;

		bool write = true;
		for (U8 i = 0; i < NUM_LINES; i++){
			if (write && temp == history.end()){
				lines[i]->text = "";
				write = false;
			}
			else if (!write)
				lines[i]->text = "";
			else
				lines[i]->text = *temp;

			temp++;
		}
		
		
	}


	

	void Log::println(const std::string& message)
	{
		if (instance.history_length < HISTORY_LENGTH)
			instance.history_length++;
		else{
			instance.history.pop_back();
		}
		instance.history.push_front(message);
		instance.iterator = instance.history.begin();
		
		instance.refresh();

	}

	

	void Log::println(F32 f)
	{
		std::stringstream ss;
		std::string s;
		ss << f;
		ss >> s;
		println(s);
	}

	void Log::print(F32 f)
	{
		std::stringstream ss;
		std::string s;
		ss << f;
		ss >> s;
		print(s);
	}


	void Log::print(const std::string& message)
	{
		instance.iterator = instance.history.begin();
		if (instance.iterator == instance.history.end())
			println(message);
		else{
			*instance.iterator += message;
			instance.refresh();
		}
	}

	void Log::read_up()
	{
		if (iterator != history.end()){
			iterator++;
			refresh();
		}
	}
	void Log::read_down()
	{
		if (iterator != history.begin()){
			iterator--;
			refresh();
		}
	}
	void Log::read_beginning()
	{
		iterator = history.begin();
		refresh();
	}
	void Log::read_end()
	{
		iterator = history.end();
		refresh();
	}

}
