#include "Shell.h"
#include "TextureManager.h"
#include <iostream>
#include <unordered_map>

#define MISSING_INT INT_MIN-1
#define ERROR_INT INT_MIN

namespace Ryno {


	void Shell::init()
	{

		im = InputManager::get_instance();
		path_size = shell_path.size();
		//Load textures
		TextureManager* texture_manager = TextureManager::get_instance();
		Texture background_texture = texture_manager->loadPNG("background", ENGINE_FOLDER);
		
		//Create background
		background = new Sprite();
		background->anchor_point = BOTTOM_LEFT;
		background->set_texture(background_texture);
		background->set_position(0, 0);
		background->angle = 0;
		background->set_scale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
		background->set_color(0, 0, 0, 240);
		background->depth = 5;

		font = new Font("inconsolata", 24, ENGINE_FOLDER);

		//Create Texts

		lines[0] = new Text();
		lines[0]->anchor_point = BOTTOM_LEFT;
		lines[0]->font = font;
		lines[0]->text = shell_path;
		lines[0]->set_scale(0.7,0.7);
		lines[0]->depth = 4;
		lines[0]->set_color(255, 255, 255, 255);
		lines[0]->set_position(0.005, .005);


		for (U8 i = 1; i < SHELL_NUM_LINES; i++)
		{
			lines[i] = new Text(lines[0]);
			lines[i]->set_position(0.005, .005 + 0.5 * i / SHELL_NUM_LINES);
			lines[i]->text = shell_path;
		}


		
	}

	void Shell::set(bool b)
	{
		active = b;
		for (Text* t : lines)
			t->active = b;
		background->active = b;
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

		
		if (im->is_key_pressed(SDLK_TAB, KEYBOARD)){
			toggle();
			return;
		}

		if (!active)
			return;

		line_0_size = lines[0]->text.size();

		if (im->is_key_pressed(SDLK_RETURN, KEYBOARD)){
			parse_input();
			rotate_lines();
		}
		if (im->is_key_pressed(SDLK_BACKSPACE, KEYBOARD)){
			
			if (line_0_size > path_size)
				lines[0]->text = lines[0]->text.substr(0, line_0_size - 1);
		}
		
		lines[0]->text += im->frame_text;


	}

	//Read input and searches for commands between '\' and ' '
	void Shell::parse_input()
	{

		std::string command;
		bool read_command = false;
		input = lines[0]->text;
		C c;
		for (U32 i = path_size; i < line_0_size; i++){

			c = input[i];

			if (!read_command){
				if (c == '\\')
					read_command = true;
			}
			else {
				if (c == ' '){
					read_command = false;
					parse_from = i + 1;
					parse_command(command);
					command.resize(0);
				}
				else
					command += c;
			}
			
		}
		//Even if no space is after
		if (read_command){
			parse_from = line_0_size;
			parse_command(command);
		}
	}


	void Shell::parse_command(const std::string& command)
	{
		if (command.compare("hide")==0)
			hide();

		else if (command.compare("textcolor") == 0){
			
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
		
			for (Text* t : lines)
			t->set_color(args[0],args[1],args[2],255);

		}

		else if (command.compare("shellcolor") == 0){

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

			background->set_color(args[0], args[1], args[2], 240);

		}

		else if (command.compare("echo") == 0){
			std::string s = string_argument();
			if (s.empty()){
				print_message("missing argument(s)."); return;
			}
			print_message(s);
		}

		else if (command.compare("fuckyou") == 0){
			
			print_message("fuck you too.");
		}

		else
			print_message("Command not found");
	}

	void Shell::rotate_lines()
	{
		for (U8 i = SHELL_NUM_LINES-1; i > 0; i--)
		{
			lines[i]->text = lines[i - 1]->text;
		}
		lines[0]->text = shell_path;
		
	}

	void Shell::print_message(const std::string& message)
	{
		rotate_lines();
		lines[0]->text += message;
	}

	

	std::string Shell::get_argument()
	{
		std::string argument;
		bool still_spaces = true;
		C c;
		for (; parse_from < line_0_size; parse_from++){
			
			c = input[parse_from];
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
			return INT_MIN-1;

		I64 r = std::strtol(s.c_str(),&error,0);
		
		if (*error!='\0'){
			return INT_MIN;
		}
		return (I32)r;
		
	}

}
