#include "Text.h"

#include <iostream>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Batch2DFont.h"
#include <GLM/gtx/string_cast.hpp>




namespace Ryno {


	std::list<Text*> Text::texts;


	


	Text::Text() {

		anchor_point = CENTER;
		texts.push_back(this);

    }
   

   

   

}
