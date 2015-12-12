#include "Text.h"

#include <iostream>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Batches/Batch2DFont.h"
#include "Shell.h"
#include <GLM/gtx/string_cast.hpp>




namespace Ryno {





	void Text::preallocate_memory_for_glyphs()
	{
		FontGlyph::current_glyph = 0;
		for (GUIObject* go : GUIObject::gui_objects){
			if (*go->text)
				FontGlyph::current_glyph += go->text->text.length();
		}
		
		FontGlyph::font_glyphs.resize(FontGlyph::current_glyph);
		FontGlyph::current_glyph = 0;
	}
	


	Text::Text() {
		anchor_point = CENTER;
    }

	Text::Text(const Text* t)
	{
		*this = *t;
		text = std::string(t->text);
	}

	Text::~Text(){
	}
   

   

   

	

}
