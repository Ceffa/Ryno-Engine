#include "Font.h"

#include <iostream>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Batch2DFont.h"
#include <GLM/gtx/string_cast.hpp>



int closestPow2(int i) {
    i--;
    int pi = 1;
    while (i > 0) {
        i >>= 1;
        pi <<= 1;
    }
    return pi;
}

#define MAX_TEXTURE_RES 4096

namespace Ryno {

	
	std::vector<FontGlyph> FontGlyph::font_glyphs;
	U32 FontGlyph::current_glyph;


	void FontGlyph::generate_model_matrix()
	{

		F32 new_sc_x = scale.x / WINDOW_WIDTH;
		F32 new_sc_y = scale.y / WINDOW_HEIGHT;

		F32 new_x = (position.x / WINDOW_WIDTH - 0.5) * 2;
		F32 new_y = (position.y / WINDOW_HEIGHT - 0.5) * 2;





		model_matrix = glm::mat3(
			glm::vec3(new_sc_x, 0, 0),
			glm::vec3(0, new_sc_y, 0),
			glm::vec3(new_x, new_y, 1)
			);

	}


	Font::Font(std::string font, U32 size,LocationOfResource loc, C cs, C ce) {

	
        // Initialize SDL_ttf
        if (!TTF_WasInit()) {
            TTF_Init();
        }

		static const std::string middle_path = "Resources/Fonts/";

		font = BASE_PATHS[loc] + middle_path + font + ".ttf";
        TTF_Font* f = TTF_OpenFont(font.c_str(), size);
        if (f == nullptr) {
            fprintf(stderr, "Failed to open TTF font %s\n", font);
            fflush(stderr);
            throw 281;
        }
        _fontHeight = TTF_FontHeight(f);
        _regStart = cs;
        _regLength = ce - cs + 1;
        int padding = size / 8;

        // First neasure all the regions
        glm::ivec4* glyphRects = new glm::ivec4[_regLength];
        int i = 0, advance;
        for (char c = cs; c <= ce; c++) {
            TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
            glyphRects[i].z -= glyphRects[i].x;
            glyphRects[i].x = 0;
            glyphRects[i].w -= glyphRects[i].y;
            glyphRects[i].y = 0;
            i++;
        }

        // Find best partitioning of glyphs
        int rows = 1, w, h, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
		t.width = 0;
		t.height = 0;
        std::vector<int>* bestPartition = nullptr;
        while (rows <= _regLength) {
            h = rows * (padding + _fontHeight) + padding;
            auto gr = createRows(glyphRects, _regLength, rows, padding, w);

            // Desire a power of 2 texture
            w = closestPow2(w);
            h = closestPow2(h);

            // A texture must be feasible
            if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
                rows++;
                delete[] gr;
                continue;
            }

            // Check for minimal area
            if (area >= w * h) {
                if (bestPartition) delete[] bestPartition;
                bestPartition = gr;
                t.width = w;
                t.height = h;
                bestRows = rows;
                area = t.width * t.height;
                rows++;
            } else {
                delete[] gr;
                break;
            }
        }

        // Can a bitmap font be made?
        if (!bestPartition) {
            fprintf(stderr, "Failed to Map TTF font %s to texture. Try lowering resolution.\n", font);
            fflush(stderr);
            throw 282;
        }

	
        // Create the texture
        glGenTextures(1, &t.id);
		glBindTexture(GL_TEXTURE_2D, t.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Now draw all the glyphs
        SDL_Color fg = { 255, 255, 255, 255 };
        int ly = padding;
        for (int ri = 0; ri < bestRows; ri++) {
            int lx = padding;
            for (int ci = 0; ci < bestPartition[ri].size(); ci++) {
                int gi = bestPartition[ri][ci];

                SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);

                // Pre-multiplication occurs here
                unsigned char* sp = (unsigned char*)glyphSurface->pixels;
                int cp = glyphSurface->w * glyphSurface->h * 4;
                for (int i = 0; i < cp; i += 4) {
                    float a = sp[i + 3] / 255.0f;
                    sp[i] *= (unsigned char) a;
                    sp[i + 1] = sp[i];
                    sp[i + 2] = sp[i];
                }

                // Save glyph image and update coordinates
                glTexSubImage2D(GL_TEXTURE_2D, 0, lx, t.height - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
                glyphRects[gi].x = lx;
                glyphRects[gi].y = ly;
                glyphRects[gi].z = glyphSurface->w;
                glyphRects[gi].w = glyphSurface->h;

                SDL_FreeSurface(glyphSurface);
                glyphSurface = nullptr;

                lx += glyphRects[gi].z + padding;
            }
            ly += _fontHeight + padding;
        }

        // Draw the unsupported glyph
        int rs = padding - 1;
        int* pureWhiteSquare = new int[rs * rs];
        memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
        delete[] pureWhiteSquare;
        pureWhiteSquare = nullptr;

        // Set some texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Create spriteBatch glyphs
        _glyphs = new CharGlyph[_regLength + 1];
        for (i = 0; i < _regLength; i++) {
            _glyphs[i].character = (char)(cs + i);
            _glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
            _glyphs[i].uvRect = glm::vec4(
                (float)glyphRects[i].x / (float)t.width,
                (float)glyphRects[i].y / (float)t.height,
                (float)glyphRects[i].z / (float)t.width,
                (float)glyphRects[i].w / (float)t.height
                );
        }
        _glyphs[_regLength].character = ' ';
        _glyphs[_regLength].size = _glyphs[0].size;
        _glyphs[_regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)t.width, (float)rs / (float)t.height);

        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] glyphRects;
        delete[] bestPartition;
        TTF_CloseFont(f);
    }
    void Font::dispose() {
        if (t.id != 0) {
            glDeleteTextures(1, &t.id);
            t.id = 0;
        }
        if (_glyphs) {
            delete[] _glyphs;
            _glyphs = nullptr;
        }
    }

    std::vector<int>* Font::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
        // Blank initialize
        std::vector<int>* l = new std::vector<int>[r]();
        int* cw = new int[r]();
        for (int i = 0; i < r; i++) {
            cw[i] = padding;
        }

        // Loop through all glyphs
        for (int i = 0; i < rectsLength; i++) {
            // Find row for placement
            int ri = 0;
            for (int rii = 1; rii < r; rii++)
            if (cw[rii] < cw[ri]) ri = rii;

            // Add width to that row
            cw[ri] += rects[i].z + padding;

            // Add glyph to the row list
            l[ri].push_back(i);
        }

        // Find the max width
        w = 0;
        for (int i = 0; i < r; i++) {
            if (cw[i] > w) w = cw[i];
        }

        return l;
    }

	glm::vec2 Font::measure(const std::string& s, std::vector<F32>* line_widths) {
        glm::vec2 size(0, _fontHeight);
		
        float cw = 0;
        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                size.y += _fontHeight;
				line_widths->push_back(cw);
                if (size.x < cw)
                    size.x = cw;
                cw = 0;
            } else {
                // Check for correct glyph
                int gi = c - _regStart;
                if (gi < 0 || gi >= _regLength)
                    gi = _regLength;
                cw += _glyphs[gi].size.x;
            }
        }
		line_widths->push_back(cw);
        if (size.x < cw)
            size.x = cw;
        return size;
    }

	void Font::draw(Batch2DFont* batch, Text* message) {

		FontGlyph* font_glyph;

		glm::vec2 position_zero = message->get_position() * glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
		glm::vec2 tp = position_zero;
		std::vector<F32> line_widths;//Size of each line of text
		glm::vec2 size = measure(message->text, &line_widths);//Global size of the text
		U32 line_counter = 0;

		glm::vec2 message_scale = message->get_scale();
		anchor_x(message->anchor_point, &tp.x, message_scale.x, size.x, line_widths[0]);
		anchor_y(message->anchor_point, &tp.y, message_scale.y, size.y);




		for (int si = 0; message->text[si] != 0; si++) {
			char c = message->text[si];
			if (message->text[si] == '\n') {
				tp.y -= _fontHeight * message_scale.y;
				tp.x = position_zero.x;
				anchor_x(message->anchor_point, &tp.x, message_scale.x, size.x, line_widths[++line_counter]);
			}
			else {
				// Check for correct glyph
				int gi = c - _regStart;
				if (gi < 0 || gi >= _regLength)
					gi = _regLength;

				font_glyph = &FontGlyph::font_glyphs[FontGlyph::current_glyph++];
				font_glyph->set_position(tp);
				font_glyph->depth = message->depth;
				font_glyph->set_scale(_glyphs[gi].size * message_scale);
				font_glyph->color = message->get_color();
				font_glyph->uv = _glyphs[gi].uvRect;
				font_glyph->uv.y = 1.0f - font_glyph->uv.y;
				font_glyph->texture = t;
				font_glyph->generate_model_matrix();
				batch->draw_glyph(font_glyph);
				tp.x += _glyphs[gi].size.x * message_scale.x;
			}
		}

	}



    

	void Font::anchor_y(AnchorPoint ap, F32* position, F32 scale, F32 size)
	{

		// Apply justification on y
		if (ap == TOP_LEFT || ap == TOP_MIDDLE || ap == TOP_RIGHT) {
			*position -=  _fontHeight * scale /2.0f ;
		}
		else if (ap == BOTTOM_LEFT || ap == BOTTOM_MIDDLE || ap == BOTTOM_RIGHT) {
			*position += (size - _fontHeight/2.0f)* scale ;
		}
		else {
			*position += (size - _fontHeight)* scale / 2.0f;
		}
	}

	void Font::anchor_x(AnchorPoint ap, F32* position, F32 scale, F32 size, F32 line_size)
	{
	
		// Apply justification on X
		if (ap == TOP_MIDDLE || ap == CENTER || ap == BOTTOM_MIDDLE) {
			*position -= line_size * scale / 2;
		}
		else if (ap == TOP_RIGHT || ap == MIDDLE_RIGHT || ap == BOTTOM_RIGHT) {
			*position -= line_size * scale;
		}
	}

}
