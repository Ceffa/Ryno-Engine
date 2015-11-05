/* 
    Original code from Benjamin Arnold from seed of andromeda.
	I just modified a few things.
	What the class does is the following:
	Loads info about chars and their sizes from the ttf.
	Then it creates a texture big enough to contain them, and place them in it by optimizing the space.
	When required to draw, the textured is sampled depending on the uvs (calculated from the position of the glyphs).
	Each sampled value is written in a quad 
*/

#pragma once


#include "Font.h"
#include "ITransform2D.h"
#include <GLM/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <list>
#include "Structures.h"


namespace Ryno {




#define FIRST_PRINTABLE_CHAR ((C)32)
#define LAST_PRINTABLE_CHAR ((C)126)

    /// For text justification
    enum class Justification {
        LEFT, MIDDLE, RIGHT
    };

	//Similar to the sprite class
	

    class Text : public ITransform2D {

		friend class FontGlyph;

    public:
        Text();
		Text(const Text* t);
		~Text();

		//Getter and setter
		void set_color(U8 r, U8 g, U8 b, U8 a){ color.set_color_and_alpha(r, g, b, a); }
		ColorRGBA get_color() { return color; }
		
		Font* font;
		bool active = true;
		Use use = GUI;
		std::string text;
		
		U8 depth = 1;
		AnchorPoint anchor_point;
		

		static void preallocate_memory_for_glyphs();
		static std::list<Text*> texts;

	protected:
		ColorRGBA color;


  
    };

}

