#include "Shell.h"
#include "TextureManager.h"
#include "Game.h"
#include "Network.h"
#include <iostream>
#include <unordered_map>


namespace Ryno {



	Shell* Shell::instance = nullptr;

	Shell* Shell::get_instance()
	{
		if(!instance);//only at the beginning
			instance = new Shell();
		return instance;
	}

	void Shell::init()
	{
		log = Log::get_instance();
		input_manager = InputManager::get_instance();

		//Load textures
		TextureManager* texture_manager = TextureManager::get_instance();
		Texture background_texture = texture_manager->load_png("background.png", ENGINE);

		background.sprite = new Sprite();
		auto* s = background.sprite;
		s->set_texture(background_texture);
		s->angle = 0;
		s->set_color(0, 0, 0, 200);
		s->depth = 5;
		s->use = SHELL;



		font.create("inconsolata", 24, ENGINE);


		iterator = history.begin();
		history_length = 0;
		input_manager = InputManager::get_instance();
		time_manager = TimeManager::get_instance();
		base_path_size = (U8)base_path.size();
		deferred_renderer = Game::get_instance()->deferred_renderer;
		log = Log::get_instance();
	
		
		background.sprite->anchor_point = BOTTOM_LEFT;
		background.sprite->set_position(0, 0);
		background.sprite->set_scale(350, WindowSize::h / 3.06f);

		//Create Texts 
		lines[0].text = new Text();
		auto *t = lines[0].text;
		t->anchor_point = BOTTOM_LEFT;
		t->font = &font;
		t->text = base_path;
		t->set_scale(0.7f,0.7f);
		t->depth = 4;
		t->set_color(50,255,0, 255);
		t->set_position(0.003f, 0.005f);
		t->use = SHELL;

		for (U8 i = 1; i < NUM_LINES; i++)
		{
			lines[i].text = new Text(lines[0].text);
			lines[i].text->set_position(0.005f, 0.003f + 0.33f * i / NUM_LINES);
		}
				
		iterator = history.begin();
		history_length = 0;
		hide();

		create_commands();
		
	}
	void Shell::set_text_color(U8 r, U8 g, U8 b)
	{
		for (auto& go : lines){
			go.text->set_color(r, g, b, 255);
		}
	}
	void Shell::set(bool b)
	{
		active = b;
		for (GUIObject& go : lines)
			go.text->active = b;
		background.sprite->active = b;
	}

	void Shell::show()
	{
		set(true);
	}

	void Shell::hide()
	{
		set(false);
	}

	void Shell::toggle()
	{
		if (active)
			hide();
		else
			show();
	}


	

	void Shell::process_input()
	{	
		
		if (input_manager->is_key_pressed(SDLK_TAB, KEYBOARD)){
				
				toggle();
				log->toggle();
			return;
		}

		if (input_manager->is_key_pressed(SDLK_BACKSLASH, KEYBOARD) && active == false){
			toggle();
			log->toggle();
		}

		if (!active)
			return;

		active_line_size = (U32)lines[0].text->text.size();

		if (input_manager->is_key_pressed(SDLK_RETURN, KEYBOARD)){
			parse_input();
			rotate_lines();
		}
		

		if (input_manager->is_key_down(SDLK_LSHIFT, KEYBOARD) && input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
			log->read_beginning();
		}
		if (input_manager->is_key_down(SDLK_LSHIFT, KEYBOARD) && input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
			log->read_end();
		}

		if (input_manager->is_key_down(SDLK_LSHIFT, KEYBOARD) && input_manager->is_key_pressed(SDLK_UP, KEYBOARD)){
			log->read_up();
		}
		else if (input_manager->is_key_pressed(SDLK_UP, KEYBOARD)){

			if (iterator != history.end()){
				lines[0].text->text = base_path + *iterator;
				iterator++;
			}
		}
		if (input_manager->is_key_down(SDLK_LSHIFT, KEYBOARD) && input_manager->is_key_pressed(SDLK_DOWN, KEYBOARD)){
			log->read_down();
		}
		else if (input_manager->is_key_pressed(SDLK_DOWN, KEYBOARD)){
			if (iterator == history.begin())
				lines[0].text->text = base_path;
			else{
				iterator--;
				if (iterator == history.begin())
					lines[0].text->text = base_path;
				else{
					lines[0].text->text = base_path + *(--iterator);
					iterator++;
				}

			}
				
			
		}


		if (input_manager->is_key_pressed(SDLK_BACKSPACE, KEYBOARD)){
			
			if (active_line_size > base_path_size)
				lines[0].text->text = lines[0].text->text.substr(0, active_line_size - 1);
		}
		
		lines[0].text->text += input_manager->frame_text;


	}

	//Read input and searches for commands between '\' and ' '
	void Shell::parse_input()
	{

		
		active_line = lines[0].text->text;
		if (active_line.compare(base_path) == 0)
			return;

		//Add to history (and remove back of the history if too long

		history.push_front(active_line.substr(base_path_size, active_line.size()));
		if (history_length < HISTORY_LENGTH)
			history_length++;
		else
			history.pop_back();
		
		iterator = history.begin();

		std::string command;
		bool read_command = false;

		C c;
		for (U32 i = base_path_size; i < active_line_size; i++){

			c = active_line[i];

			if (!read_command){
				if (c == '\\')
					read_command = true;
			}
			else {
				if (c == ' '){
					read_command = false;
					parse_starting_point = i + 1;
					parse_command(command);
					command.resize(0);
				}
				else
					command += c;
			}
			
		}
		//Even if no space is after
		if (read_command){
			parse_starting_point = active_line_size;
			parse_command(command);
		}
	}

	//GARBAGE CODE - Could really use a refactoring
	void Shell::parse_command(const std::string& command)
	{
		auto& c = commands.find(command);
		if (c != commands.end())
			c->second();
		else
			print_message("Command not found.");	
	}

	void Shell::rotate_lines()
	{
		for (U8 i = NUM_LINES-1; i > 0; i--)
		{
			lines[i].text->text = lines[i - 1].text->text;
		}
		lines[0].text->text = base_path;
		
	}

	void Shell::create_commands()
	{
		commands["server"] = [=]() {
			Network::get_instance()->start_server();
		};
		commands["client"] = [=]() {
			Network::get_instance()->start_client();
		};
		commands["pausemusic"] = [=]() {
			Music::pause();
		};
		commands["resumemusic"] = [=]() {
			Music::resume();
		};
		commands["mutemusic"] = [=]() {
			Music::set_volume(0);
		};
		commands["mutesound"] = [=]() {
			Sound::set_volume(0);
		};
		commands["musicvolume"] = [=]() {
			bool err = false;
			F32 f = float_argument(err);
			if (!err) Music::set_volume(f);
		};
		commands["soundvolume"] = [=]() {
			bool err = false;
			F32 f = float_argument(err);
			if (!err) Sound::set_volume(f);
		};
		commands["p"] = [=]() {
			request_pause = !request_pause;
		};
		commands["r"] = [=]() {
			Game::get_instance()->reset_scene();
		};
		commands["n"] = [=]() {
			Game::get_instance()->next_scene();
		};
		commands["t"] = [=]() {
			bool err = false;
			F32 f = float_argument(err);
			if (!err) time_manager->slow_factor = f;
		};
		commands["echo"] = [=]() {
			bool err = false;
			std::string s = string_argument(err);
			if (!err) log->print(s);
		};
		commands["s"] = [=]() {
			bool err = false;
			int i = int_argument(err);
			if (!err) Game::get_instance()->set_scene(i);
		};
		commands["fuckyou"] = [=]() {
			print_message("well, fuck you too.");
		};
		commands["clr"] = [=]() {
			for (GUIObject& go : log->lines)
				go.text->text = "";
			for (GUIObject& go : lines)
				go.text->text = base_path;
			log->history.clear();
			log->history_length = 0;
			log->iterator = log->history.begin();
			history.clear();
			history_length = 0;
			iterator = history.begin();
		};
		commands["exit"] = [=]() {
			request_exit = true;
		};
		commands["nodl"] = [=]() {
			deferred_renderer->lightInfo[DIR].lights_enabled = false;
		};
		commands["dl"] = [=]() {
			deferred_renderer->lightInfo[DIR].lights_enabled = true;
		};
		commands["nopl"] = [=]() {
			deferred_renderer->lightInfo[POINT].lights_enabled = false;
		};
		commands["pl"] = [=]() {
			deferred_renderer->lightInfo[POINT].lights_enabled = true;
		};
		commands["nosl"] = [=]() {
			deferred_renderer->lightInfo[SPOT].lights_enabled = false;
		};
		commands["sl"] = [=]() {
			deferred_renderer->lightInfo[SPOT].lights_enabled = true;
		};
		commands["nods"] = [=]() {
			deferred_renderer->lightInfo[DIR].shadows_enabled = false;
		};
		commands["ds"] = [=]() {
			deferred_renderer->lightInfo[DIR].shadows_enabled = true;
		};
		commands["nops"] = [=]() {
			deferred_renderer->lightInfo[POINT].shadows_enabled = false;
		};
		commands["ps"] = [=]() {
			deferred_renderer->lightInfo[POINT].shadows_enabled = true;
		};
		commands["noss"] = [=]() {
			deferred_renderer->lightInfo[SPOT].shadows_enabled = false;
		};
		commands["ss"] = [=]() {
			deferred_renderer->lightInfo[SPOT].shadows_enabled = true;
		};
		commands["nosb"] = [=]() {
			deferred_renderer->skybox_enabled = false;
		};
		commands["sb"] = [=]() {
			deferred_renderer->skybox_enabled = true;
		};
		commands["nogui"] = [=]() {
			deferred_renderer->gui_sprites_enabled = false;
			deferred_renderer->gui_text_enabled = false;
		};
		commands["gui"] = [=]() {
			deferred_renderer->gui_sprites_enabled = true;
			deferred_renderer->gui_text_enabled = true;
		};
		commands["nogm"] = [=]() {
			deferred_renderer->geometry_enabled = false;
		};
		commands["gm"] = [=]() {
			deferred_renderer->geometry_enabled = true;
		};
		commands["nopp"] = [=]() {
			deferred_renderer->postprocessor_enabled = false;
		};
		commands["pp"] = [=]() {
			deferred_renderer->postprocessor_enabled = true;
		};
		commands["noao"] = [=]() {
			deferred_renderer->ssao_enabled = false;
		};
		commands["ao"] = [=]() {
			deferred_renderer->ssao_enabled = true;
		};
	}


	

	void Shell::print_message(const std::string& message)
	{
		rotate_lines();
		lines[0].text->text += message;
	}

	

	std::string Shell::get_argument()
	{
		std::string argument;
		bool still_spaces = true;
		C c;
		for (; parse_starting_point < active_line_size; parse_starting_point++){
			
			c = active_line[parse_starting_point];
			if (c == ' ')
			{
				if (!still_spaces) return argument;
			}
			
			else
			{
				if (still_spaces) still_spaces = false;
				argument += c;
			}
		}
		return argument;
	}


	std::string Shell::string_argument(bool& error)
	{
		std::string s = get_argument();
		if (s.empty()) {
			print_message("Missing argument (expected string).");
			error = true;
		}
		return s;

	}

	I32 Shell::int_argument(bool& error)
	{
		C* error_int;
		
		std::string s = get_argument();
		I64 r = 0;
		if (s.empty()) {
			print_message("Missing argument (expected int).");
			error = true;
		}
		else {

			r = std::strtol(s.c_str(), &error_int, 0);

			if (*error_int != '\0') {
				print_message("Wrong argument (expected int).");
				error = true;
			}
		}
		return (I32)r;
		
	}

	F32 Shell::float_argument(bool& error)
	{
		C* error_float;

		std::string s = get_argument();
		F64 r = 0;

		if (s.empty()) {
			print_message("Missing argument (expected float).");
			error = true;
		}
		r = std::strtod(s.c_str(), &error_float);

		if (*error_float != '\0') {
			print_message("Wrong argument (expected float).");
			error = true;
		}
		return (F32)r;

	}

}
