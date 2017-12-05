#include "Log.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision



#define MISSING_INT INT_MIN-1
#define ERROR_INT INT_MIN

namespace Ryno {

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

		background_log.sprite = new Sprite();
		Sprite* s = background_log.sprite;
		s->set_texture(background_texture);
		s->angle = 0;
		s->set_color(0, 0, 0, 200);
		s->depth = 5;
		s->use = SHELL;
		s->anchor_point = TOP_LEFT;
		s->set_position(0, .935);
		s->set_scale(350, .603 * WindowSize::h);

		background_stats.sprite = new Sprite(s);
		s = background_stats.sprite;
		s->set_position(0, 1);
		s->set_scale(350, .06 * WindowSize::h);



		font.create("inconsolata", 24, ENGINE);


		iterator = history.begin();
		history_length = 0;
		input_manager = InputManager::get_instance();
		

			
		lines[0].text = new Text();
		Text* t = lines[0].text;
		t->anchor_point = BOTTOM_LEFT;
		t->font = &font;
		t->text = "";
		t->set_scale(0.7f,0.7f);
		t->depth = 4;
		t->set_color(255, 255, 0, 255);
		t->set_position(0.005f, .34f);
		t->use = SHELL;


		for (U8 i = 1; i < NUM_LINES; i++)
		{
			lines[i].text = new Text(lines[0].text);
			lines[i].text->set_position(0.005f, .34f + 0.6f * i / NUM_LINES);
		}

		stats[0].text = new Text(lines[0].text);
		t = stats[0].text;
		t->anchor_point = TOP_LEFT;
		t->set_position(0.005f, .994f);
		t->text = "DRAW";
		t->set_color(255, 0,0, 255);


		stats[1].text = new Text(stats[0].text);
		stats[2].text = new Text(stats[0].text);
		stats[3].text = new Text(stats[0].text);
		stats[1].text->set_position(0.13f * 1280 / WindowSize::w, .994f);
		stats[2].text->set_position(0.005f, .969f);
		stats[3].text->set_position(0.13f * 1280 / WindowSize::w, .969f);


		Log::print("");
		hide();
	
		
	}

	void Log::set(bool b)
	{
		active = b;
		for (auto& go : lines)
			go.text->active = b;
		for (auto& go : stats)
			go.text->active = b;
		background_log.sprite->active = b;
		background_stats.sprite->active = b;
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

	void Log::print(const glm::vec3& vec)
	{
		print("(x: ");
		print(vec.x);
		print(" , y: ");
		print(vec.y);
		print(" , z: ");
		print(vec.z);
		print(")");
	}
	void Log::println(const glm::vec3& vec)
	{
		println("");
		print(vec);
	}

	void Log::update_stats()
	{
		auto dr = DeferredRenderer::get_instance();
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << TimeManager::get_instance()->current_fps;
		instance->stats[0].text->text = "Fps: " + stream.str();
		instance->stats[1].text->text = "Draw calls: " + std::to_string(dr->m_geometry_batch3d.draw_calls);
		instance->stats[2].text->text = "Vertices: " + std::to_string(dr->m_geometry_batch3d.total_vertices);
		instance->stats[3].text->text = "Lights: " + std::to_string(dr->total_active_lights);

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
