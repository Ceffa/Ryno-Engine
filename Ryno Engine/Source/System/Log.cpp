#include "Log.h"
#include "TextureManager.h"
#include <iostream>
#include <sstream>


#define MISSING_INT INT_MIN-1
#define ERROR_INT INT_MIN

namespace RynoEngine {

	Log* Log::instance = nullptr;


	void Log::set_text_color(U8 r, U8 g, U8 b)
	{
		for (auto& go : lines){
			go.text->set_color(r, g, b, 255);
		}
	}


	Log* Log::get_instance()
	{
		if (!instance)
			instance = new Log();
		return instance;
	}

	void Log::init()
	{
		input_manager = InputManager::get_instance();

		//Load textures
		TextureManager* texture_manager = TextureManager::get_instance();
		Texture background_texture = texture_manager->load_png("background.png", ENGINE);

		background.sprite = new Sprite();
		Sprite* s = background.sprite;
		s->set_texture(background_texture);
		s->angle = 0;
		s->set_color(0, 0, 0, 240);
		s->depth = 5;
		s->use = SHELL;



		font.create("inconsolata", 24, ENGINE);


		iterator = history.begin();
		history_length = 0;
		input_manager = InputManager::get_instance();
		
		background.sprite->anchor_point = TOP_LEFT;
		background.sprite->set_position(0, 1);
		background.sprite->set_scale(350, WINDOW_HEIGHT / 1.506f);

			
		lines[0].text = new Text();
		auto* t = lines[0].text;
		t->anchor_point = BOTTOM_LEFT;
		t->font = &font;
		t->text = "";
		t->set_scale(0.7f,0.7f);
		t->depth = 4;
		t->set_color(255, 230, 0, 255);
		t->set_position(0.005f, .34f);
		t->use = SHELL;


		for (U8 i = 1; i < NUM_LINES; i++)
		{
			lines[i].text = new Text(lines[0].text);
			lines[i].text->set_position(0.005f, .34f + 0.66f * i / NUM_LINES);
		}

		
		Log::print("");
	
		
	}

	void Log::set(bool b)
	{
		active = b;
		for (auto& go : lines)
			go.text->active = b;
		background.sprite->active = b;
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
				lines[i].text->text = "";
				write = false;
			}
			else if (!write)
				lines[i].text->text = "";
			else
				lines[i].text->text = *temp;

			if (temp!= history.end()) temp++;
		}
		
		
	}


	

	void Log::println(const std::string& message)
	{
		if (instance->history_length < HISTORY_LENGTH)
			instance->history_length++;
		else{
			instance->history.pop_back();
		}
		instance->history.push_front(message);
		instance->iterator = instance->history.begin();
		
		instance->refresh();

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
		instance->iterator = instance->history.begin();
		if (instance->iterator == instance->history.end())
			println(message);
		else{
			*instance->iterator += message;
			instance->refresh();
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
