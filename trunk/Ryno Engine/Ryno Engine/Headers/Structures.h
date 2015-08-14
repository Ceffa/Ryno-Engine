#pragma once
#include <GLm/glm.hpp>
#include "Types.h"

namespace Ryno{
	struct ColorRGB{

		ColorRGB() : r(0), g(0), b(0){}
		ColorRGB(U8 R, U8 G, U8 B) : r(R), g(G), b(B){}
		void set_color(U8 _r, U8 _g, U8 _b){
			r = _r;
			g = _g;
			b = _b;
		}
		glm::vec3 to_vec3(){
			return glm::vec3(r, g, b);
		}

		U8 r; U8 g; U8 b;


	};

	struct ColorRGBA{

			ColorRGBA() : r(0), g(0), b(0), f(0){}
			ColorRGBA(U8 R, U8 G, U8 B, U8 F) : r(R), g(G), b(B), f(F){}
			void set_color(U8 _r, U8 _g, U8 _b){
				r = _r;
				g = _g;
				b = _b;
			}
			void set_flatness(U8 _f){
				f = _f;
			}

			void set_color_and_flatness(U8 _r, U8 _g, U8 _b, U8 _f){
				set_color(_r, _g, _b);
				set_flatness(_f);
			}
			glm::vec4 to_vec4(){
				return glm::vec4(r, g, b, f);
			}
			glm::vec3 to_vec3(){
				return glm::vec3(r, g, b);
			}

			U8 r; U8 g; U8 b; U8 f;


		

	};



}