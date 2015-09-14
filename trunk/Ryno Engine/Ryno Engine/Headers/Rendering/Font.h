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


#include <GLM/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include "Structures.h"
#include "ITransform2D.h"


namespace Ryno {

	class Text;
	class Batch2DFont;

    struct CharGlyph {
    public:
        C character;
        glm::vec4 uvRect;
        glm::vec2 size;
    };

#define FIRST_PRINTABLE_CHAR ((C)32)
#define LAST_PRINTABLE_CHAR ((C)126)

	class FontGlyph : public ITransform2D{


	public:
		FontGlyph(){}
		~FontGlyph(){}
		void generate_model_matrix();

		U32 get_texture_id() const { return texture.id; };
		void set_color(U8 r, U8 g, U8 b, U8 a){ color.set_color_and_alpha(r, g, b, a); }
		ColorRGBA get_color() { return color; }

		glm::mat3 model_matrix;
		U8 depth;
		glm::vec4 uv;
		Texture texture;

		static U32 current_glyph;
		static std::vector<FontGlyph> font_glyphs;
		ColorRGBA color;


	};
	

    class Font {

		friend class FontGlyph;

    public:
		Font(std::string font, U32 size, LocationOfResource loc, C cs, C ce);
		Font(std::string font, U32 size, LocationOfResource loc) :
			Font(font, size,loc, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
        }

		Texture t;



        /// Destroys the font resources
        void dispose();

        int getFontHeight() const {
            return _fontHeight;
        }
        /// Measures the dimensions of the text and of each line
		glm::vec2 measure(const std::string& s, std::vector<F32>* line_widths);

		void draw(Batch2DFont* batch, Text* message);

		void anchoring(Text* message, glm::vec2 &tp);

		void anchor_y(AnchorPoint ap, F32* position, F32 scale, F32 size);

		void anchor_x(AnchorPoint ap, F32* position,F32 scale, F32 size, F32 line_size);



	private:
        static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

        int _regStart, _regLength;
        CharGlyph* _glyphs;
        int _fontHeight;

        unsigned int _texID;
    };

}

