#include "Text.h"

#include <iostream>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Batches/Batch2DFont.h"
#include "Shell.h"
#include <GLM/gtx/string_cast.hpp>




namespace Ryno {




	std::list<Text*> Text::texts;

	void Text::preallocate_memory_for_glyphs()
	{
		FontGlyph::current_glyph = 0;
		for (Text* s : texts){
			FontGlyph::current_glyph += s->text.length();
		}
		
		FontGlyph::font_glyphs.resize(FontGlyph::current_glyph);
		FontGlyph::current_glyph = 0;
	}
	


	Text::Text() {

		anchor_point = CENTER;
		texts.push_back(this);

    }

	Text::Text(const Text* t)
	{
		*this = *t;
		text = std::string(t->text);
		texts.push_back(this);
	}

	Text::~Text(){
		texts.remove(this);
	}
   

   

   

	

}
