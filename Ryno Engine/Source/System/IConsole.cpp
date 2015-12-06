#include "IConsole.h"

namespace Ryno {

	void IConsole::init()
	{

		input_manager = InputManager::get_instance();
		
		//Load textures
		TextureManager* texture_manager = TextureManager::get_instance();
		Texture background_texture = texture_manager->load_png("background", ENGINE);
		
		//Create background
		background = new Sprite();
		background->set_texture(background_texture);
		background->angle = 0;
		background->set_color(0,0,0,240);
		background->depth = 5;
		background->use = SHELL;


		font = new Font("inconsolata", 24, ENGINE);


		iterator = history.begin();
		history_length = 0;
		
	}



	void IConsole::set_text_color(U8 r, U8 g, U8 b)
	{
		for (Text*t : lines){
			t->set_color(r, g, b, 255);
		}
	}

}
