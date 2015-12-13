#include "IConsole.h"

namespace Ryno {

	void IConsole::init()
	{

		input_manager = InputManager::get_instance();
		
		//Load textures
		TextureManager* texture_manager = TextureManager::get_instance();
		Texture background_texture = texture_manager->load_png("background", ENGINE);
		
		//Create background
		Mallocator* r = Mallocator::get_instance();
		background.create(r);

		Sprite* s = background->sprite.create(r);
		s->set_texture(background_texture);
		s->angle = 0;
		s->set_color(0,0,0,240);
		s->depth = 5;
		s->use = SHELL;
		


		font.create(r,"inconsolata", 24, ENGINE);


		iterator = history.begin();
		history_length = 0;
		
	}


	void IConsole::set_text_color(U8 r, U8 g, U8 b)
	{
		for (New<GUIObject>& go : lines){
			go->text->set_color(r, g, b, 255);
		}
	}

}
