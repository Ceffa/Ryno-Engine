#include "Shell.h"
#include "TextureManager.h"
#include "Game.h"
#include "Network.h"
#include <iostream>
#include <unordered_map>

#define MISSING_INT INT_MAX/2-1
#define ERROR_INT INT_MAX/2
#define MISSING_FLOAT FLT_MAX/2-1
#define ERROR_FLOAT INT_MAX/2

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
		s->set_color(0, 0, 0, 240);
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
		t->set_color(255, 230, 0, 255);
		t->set_position(0.003f, 0.005f);
		t->use = SHELL;

		for (U8 i = 1; i < NUM_LINES; i++)
		{
			lines[i].text = new Text(lines[0].text);
			lines[i].text->set_position(0.005f, 0.003f + 0.33f * i / NUM_LINES);
		}
				
		iterator = history.begin();
		history_length = 0;

		
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


	void Shell::parse_command(const std::string& command)
	{
		if (command.compare("winpos") == 0) {
			U32 x = int_argument();
			U32 y = int_argument();
			Game::get_instance()->set_window_pos(x, y);
		}
		else if (command.compare("winsize") == 0) {
			U32 w = int_argument();
			U32 h = int_argument();
			Game::get_instance()->set_window_pos(w,h);
		}

		else if (command.compare("server") == 0) {
			Network::get_instance()->start_server();
		}
		else if (command.compare("send") == 0) {
			Message mess;
			mess.id.set(Network::get_instance()->net_entity->local_address);
			SmallAddress addr("127.0.0.1", 5555);
			NetUtil::print(addr.to_string());
			Network::get_instance()->net_entity->sock.send_struct(mess,addr);
		}
		else if (command.compare("client") == 0) {
			Network::get_instance()->start_client();
		}
		else if (command.compare("pausemusic") == 0)
			Music::pause();
		else if (command.compare("resumemusic") == 0)
			Music::resume();
		else if (command.compare("mutemusic") == 0)
			Music::set_volume(0);
		else if (command.compare("mutesound") == 0)
			Sound::set_volume(0);
		else if (command.compare("musicvolume") == 0){

			//read args
			F32 f;


			f = float_argument();
			if (f == ERROR_FLOAT){
				print_message("argument(s) is not an int."); return;
			}
			else if (f == MISSING_FLOAT){
				print_message("missing argument(s)."); return;
			}

			Music::set_volume(f);

		}
		else if (command.compare("soundvolume") == 0){

			//read args
			F32 f;


			f = float_argument();
			if (f == ERROR_FLOAT){
				print_message("argument(s) is not an int."); return;
			}
			else if (f == MISSING_FLOAT){
				print_message("missing argument(s)."); return;
			}

			Sound::set_volume(f);

		}
		else if (command.compare("p") == 0){
			request_pause = !request_pause;
		}
		else if (command.compare("r") == 0){
			Game::get_instance()->reset_scene();
		}
		else if (command.compare("n") == 0) {
			Game::get_instance()->next_scene();
		}
		else if (command.compare("fps") == 0) {
			Log::get_instance()->fps_log = !Log::get_instance()->fps_log;
		}
		else if (command.compare("t") == 0){

			//read args
			F32 f;


			f = float_argument();
			if (f == ERROR_FLOAT){
				print_message("argument(s) is not an int."); return;
			}
			else if (f == MISSING_FLOAT){
				print_message("missing argument(s)."); return;
			}
			
			
			time_manager->slow_factor = f;

		}
		else if (command.compare("shelltextcolor") == 0){
			
			//read args
			I32 args[3];

			for (U8 i = 0; i < 3; i++){
				args[i] = int_argument();
				if (args[i] == ERROR_INT){
					print_message("argument(s) is not an int."); return;
				}
				else if (args[i] == MISSING_INT){
					print_message("missing argument(s)."); return;
				}
			}
		
			set_text_color(args[0], args[1], args[2]);

		}
		else if (command.compare("shellbackcolor") == 0){

			//read args
			I32 args[4];

			for (U8 i = 0; i < 4; i++){
				args[i] = int_argument();
				if (args[i] == ERROR_INT){
					print_message("argument(s) is not an int."); return;
				}
				else if (args[i] == MISSING_INT && i!=3){
					print_message("missing argument(s)."); return;
				}
			}
			
			background.sprite->set_color(args[0], args[1], args[2], args[3]);

		}
		else if (command.compare("logtextcolor") == 0){

			//read args
			I32 args[3];

			for (U8 i = 0; i < 3; i++){
				args[i] = int_argument();
				if (args[i] == ERROR_INT){
					print_message("argument(s) is not an int."); return;
				}
				else if (args[i] == MISSING_INT){
					print_message("missing argument(s)."); return;
				}
			}

			log->set_text_color(args[0], args[1], args[2]);

		}
		else if (command.compare("logbackcolor") == 0){

			//read args
			I32 args[4];

			for (U8 i = 0; i < 4; i++){
				args[i] = int_argument();
				if (args[i] == ERROR_INT){
					print_message("argument(s) is not an int."); return;
				}
				else if (args[i] == MISSING_INT && i != 3){
					print_message("missing argument(s)."); return;
				}
			}

			log->background.sprite->set_color(args[0], args[1], args[2], args[3]);

		}
		else if (command.compare("echo") == 0){
			std::string s = string_argument();
			if (s.empty()){
				print_message("missing argument(s)."); return;
			}
			log->print(s);
		}
		else if (command.compare("s") == 0){
			int i = int_argument();
			if (i == ERROR_INT){
				print_message("missing argument(s)."); return;
			}
			Game::get_instance()->set_scene(i);
		}
		else if (command.compare("ss") == 0) {
			std::string s = string_argument();
			if (s.empty()) {
				print_message("missing argument(s)."); return;
			}
			Game::get_instance()->set_scene(s);
		}
		else if (command.compare("fuckyou") == 0){
			
			print_message("well, fuck you too.");
		}
		else if (command.compare("clearshellhistory") == 0){
			history.clear();
			history_length = 0;
			iterator = history.begin();
		}
		else if (command.compare("clearshellscreen") == 0){
			for (GUIObject& go : lines)
				go.text->text = base_path;

		}
		else if (command.compare("clearloghistory") == 0){
			log->history.clear();
			log->history_length = 0;
			log->iterator = log->history.begin();
		}
		else if (command.compare("clr") == 0){
			for (GUIObject& go : log->lines)
				go.text->text = "";

		}
		else if (command.compare("exit") == 0){
			
			request_exit = true;
		}
		else if (command.compare("nopl") == 0){
			deferred_renderer->point_light_enabled = false;
		}
		else if (command.compare("pl") == 0){
			deferred_renderer->point_light_enabled = true;
		}
		else if (command.compare("nosl") == 0){
			deferred_renderer->spot_light_enabled = false;
		}
		else if (command.compare("sl") == 0){
			deferred_renderer->spot_light_enabled = true;
		}
		else if (command.compare("nodl") == 0){
			deferred_renderer->directional_light_enabled = false;
		}
		else if (command.compare("dl") == 0){
			deferred_renderer->directional_light_enabled = true;
		}
		else if (command.compare("nops") == 0){
			deferred_renderer->point_shadow_enabled = false;
		}
		else if (command.compare("ps") == 0){
			deferred_renderer->point_shadow_enabled = true;
		}
		else if (command.compare("noss") == 0){
			deferred_renderer->spot_shadow_enabled = false;
		}
		else if (command.compare("ss") == 0){
			deferred_renderer->spot_shadow_enabled = true;
		}
		else if (command.compare("nods") == 0){
			deferred_renderer->directional_shadow_enabled = false;
		}
		else if (command.compare("ds") == 0){
			deferred_renderer->directional_shadow_enabled = true;
		}
		else if (command.compare("nosb") == 0){
			deferred_renderer->skybox_enabled = false;
		}
		else if (command.compare("sb") == 0){
			deferred_renderer->skybox_enabled = true;
		}
		else if (command.compare("nogs") == 0){
			deferred_renderer->gui_sprites_enabled = false;
		}
		else if (command.compare("gs") == 0){
			deferred_renderer->gui_sprites_enabled = true;
		}
		else if (command.compare("nogt") == 0){
			deferred_renderer->gui_text_enabled = false;
		}
		else if (command.compare("gt") == 0){
			deferred_renderer->gui_text_enabled = true;
		}
		else if (command.compare("nogm") == 0){
			deferred_renderer->geometry_enabled = false;
		}
		else if (command.compare("gm") == 0){
			deferred_renderer->geometry_enabled = true;
		}
		else if (command.compare("nolights") == 0){
			deferred_renderer->point_light_enabled = false;
			deferred_renderer->spot_light_enabled = false;
			deferred_renderer->directional_light_enabled = false;

		}
		else if (command.compare("lights") == 0){
			deferred_renderer->point_light_enabled = true;
			deferred_renderer->spot_light_enabled = true;
			deferred_renderer->directional_light_enabled = true;
		}
		else if (command.compare("nogui") == 0){
			deferred_renderer->gui_sprites_enabled = false;
			deferred_renderer->gui_text_enabled = false;

		}
		else if (command.compare("gui") == 0){
			deferred_renderer->gui_sprites_enabled = true;
			deferred_renderer->gui_text_enabled = true;
		}
		else
			print_message("Command not found");
	}

	void Shell::rotate_lines()
	{
		for (U8 i = NUM_LINES-1; i > 0; i--)
		{
			lines[i].text->text = lines[i - 1].text->text;
		}
		lines[0].text->text = base_path;
		
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


	std::string Shell::string_argument()
	{
		std::string s = get_argument();
		return s;

	}

	I32 Shell::int_argument()
	{
		C* error;
		
		std::string s = get_argument();

		if (s.empty())
			return MISSING_INT;

		I64 r = std::strtol(s.c_str(),&error,0);
		
		if (*error!='\0'){
			return ERROR_INT;
		}
		return (I32)r;
		
	}

	F32 Shell::float_argument()
	{
		C* error;

		std::string s = get_argument();

		if (s.empty())
			return MISSING_FLOAT - 1;

		F64 r = std::strtod(s.c_str(), &error);

		if (*error != '\0'){
			return ERROR_FLOAT;
		}
		return (F32)r;

	}

}
