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
			c->second(false);
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
		commands["server"] = [=](bool hint) {
			if(hint) return "Starts a server.";
			Network::get_instance()->start_server();
			return "";
		};
		commands["client"] = [=](bool hint) {
			if(hint) return "Starts a client.";
			Network::get_instance()->start_client();			return "";
			return "";
		};
		commands["pausemusic"] = [=](bool hint) {
			if(hint) return "Pauses the music.";
			Music::pause();
			return "";
		};
		commands["resumemusic"] = [=](bool hint) {
			if(hint) return "Resumes the music.";
			Music::resume();
			return "";
		};
		commands["mutemusic"] = [=](bool hint) {
			if(hint) return "Mutes the music.";
			Music::set_volume(0);
			return "";
		};
		commands["mutesound"] = [=](bool hint) {
			if(hint) return "Mutes the sound.";
			Sound::set_volume(0);
			return "";
		};
		commands["musicvolume"] = [=](bool hint) {
			if(hint) return "Sets music volume to [f0].";
			bool err = false;
			F32 f = float_argument(err);
			if (!err) Music::set_volume(f);
			return "";
		};
		commands["soundvolume"] = [=](bool hint) {
			if(hint) return "Sets sound volume to [f0].";
			bool err = false;
			F32 f = float_argument(err);
			if (!err) Sound::set_volume(f);
			return "";
		};
		commands["p"] = [=](bool hint) {
			if(hint) return "Pauses or unpauses game.";
			request_pause = !request_pause;
			return "";
		};
		commands["r"] = [=](bool hint) {
			if(hint) return "Reloads current scene.";
			Game::get_instance()->reset_scene();
			return "";
		};
		commands["n"] = [=](bool hint) {
			if(hint) return "Loads next scene.";
			Game::get_instance()->next_scene();
			return "";
		};
		commands["t"] = [=](bool hint) {
			if(hint) return "Multiply global time by [f0].";
			bool err = false;
			F32 f = float_argument(err);
			if (!err) time_manager->slow_factor = f;
			return "";
		};
		commands["echo"] = [=](bool hint) {
			if(hint) return "Prints back string [s0].";
			bool err = false;
			std::string s = string_argument(err);
			if (!err) log->print(s);
			return "";
		};
		commands["s"] = [=](bool hint) {
			if(hint) return "Loads scene number [i0].";
			bool err = false;
			int i = int_argument(err);
			if (!err) Game::get_instance()->set_scene(i);
			return "";
		};
		commands["fuckyou"] = [=](bool hint) {
			if(hint) return "Engages in philosophycal argument.";
			print_message("well, fuck you too.");
			return "";
		};
		commands["clr"] = [=](bool hint) {
			if(hint) return "Clears screen and history of shell and log.";
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
			return "";
		};
		commands["exit"] = [=](bool hint) {
			if(hint) return "Request exit from game.";
			request_exit = true;
			return "";
		};
		commands["nodl"] = [=](bool hint) {
			if(hint) return "Disables directional lights.";
			deferred_renderer->lightInfo[DIR].lights_enabled = false;
			return "";
		};
		commands["dl"] = [=](bool hint) {
			if(hint) return "Enables directional lights.";
			deferred_renderer->lightInfo[DIR].lights_enabled = true;
			return "";
		};
		commands["nopl"] = [=](bool hint) {
			if(hint) return "Disables point lights.";
			deferred_renderer->lightInfo[POINT].lights_enabled = false;
			return "";
		};
		commands["pl"] = [=](bool hint) {
			if(hint) return "Enables point lights.";
			deferred_renderer->lightInfo[POINT].lights_enabled = true;
			return "";
		};
		commands["nosl"] = [=](bool hint) {
			if(hint) return "Disables spot lights.";
			deferred_renderer->lightInfo[SPOT].lights_enabled = false;
			return "";
		};
		commands["sl"] = [=](bool hint) {
			if(hint) return "Enables spot lights.";
			deferred_renderer->lightInfo[SPOT].lights_enabled = true;
			return "";
		};
		commands["nods"] = [=](bool hint) {
			if(hint) return "Disables directional shadows.";
			deferred_renderer->lightInfo[DIR].shadows_enabled = false;
			return "";
		};
		commands["ds"] = [=](bool hint) {
			if(hint) return "Enables directional shadows.";
			deferred_renderer->lightInfo[DIR].shadows_enabled = true;
			return "";
		};
		commands["nops"] = [=](bool hint) {
			if(hint) return "Disables point shadows.";
			deferred_renderer->lightInfo[POINT].shadows_enabled = false;
			return "";
		};
		commands["ps"] = [=](bool hint) {
			if(hint) return "Enables point shadows.";
			deferred_renderer->lightInfo[POINT].shadows_enabled = true;
			return "";
		};
		commands["noss"] = [=](bool hint) {
			if(hint) return "Disables spot shadows.";
			deferred_renderer->lightInfo[SPOT].shadows_enabled = false;
			return "";
		};
		commands["ss"] = [=](bool hint) {
			if(hint) return "Enables spot shadows.";
			deferred_renderer->lightInfo[SPOT].shadows_enabled = true;
			return "";
		};
		commands["nosb"] = [=](bool hint) {
			if(hint) return "Disables skybox.";
			deferred_renderer->skybox_enabled = false;
			return "";
		};
		commands["sb"] = [=](bool hint) {
			if(hint) return "Enables skybox.";
			deferred_renderer->skybox_enabled = true;
			return "";
		};
		commands["nogui"] = [=](bool hint) {
			if(hint) return "Disables GUI.";
			deferred_renderer->gui_sprites_enabled = false;
			deferred_renderer->gui_text_enabled = false;
			return "";
		};
		commands["gui"] = [=](bool hint) {
			if(hint) return "Enables GUIss.";
			deferred_renderer->gui_sprites_enabled = true;
			deferred_renderer->gui_text_enabled = true;
			return "";
		};
		commands["nogm"] = [=](bool hint) {
			if(hint) return "Disables geometry.";
			deferred_renderer->geometry_enabled = false;
			return "";
		};
		commands["gm"] = [=](bool hint) {
			if(hint) return "Enables geometry.";
			deferred_renderer->geometry_enabled = true;
			return "";
		};
		commands["nopp"] = [=](bool hint) {
			if(hint) return "Disables postprocessor.";
			deferred_renderer->postprocessor_enabled = false;
			return "";
		};
		commands["pp"] = [=](bool hint) {
			if(hint) return "Enables postprocessor.";
			deferred_renderer->postprocessor_enabled = true;
			return "";
		};
		commands["noao"] = [=](bool hint) {
			if(hint) return "Disables SSAO.";
			deferred_renderer->ssao_enabled = false;
			return "";
		};
		commands["ao"] = [=](bool hint) {
			if(hint) return "Enables SSAO.";
			deferred_renderer->ssao_enabled = true;
			return "";
		};
		commands["help"] = [=](bool hint) {
			if (hint) return "Explains command [s0].";
			bool err = false;
			auto s = string_argument(err);
			auto& c = commands.find(s);
			if (c != commands.end())
				print_message(c->second(true));
			else
				print_message("Command not found.");
			return "";
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
