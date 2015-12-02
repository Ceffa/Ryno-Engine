#pragma once
#include <GLM/glm.hpp>
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

		static ColorRGBA red;
		static ColorRGBA yellow;
		static ColorRGBA green;
		static ColorRGBA cyan;
		static ColorRGBA blue;
		static ColorRGBA magenta;
		static ColorRGBA black;
		static ColorRGBA gray;
		static ColorRGBA white;

			ColorRGBA() : r(0), g(0), b(0), a(0){}
			ColorRGBA(U8 R, U8 G, U8 B, U8 A) : r(R), g(G), b(B), a(A){}
			void set_color(U8 _r, U8 _g, U8 _b){
				r = _r;
				g = _g;
				b = _b;
			}
			void set_alpha(U8 _a){
				a = _a;
			}

			void set_color_and_alpha(U8 _r, U8 _g, U8 _b, U8 _a){
				set_color(_r, _g, _b);
				set_alpha(_a);
			}
			glm::vec4 to_vec4(){
				return glm::vec4(r, g, b, a);
			}
			glm::vec3 to_vec3(){
				return glm::vec3(r, g, b);
			}

			void dupe_color(ColorRGBA col){
				r = col.r;
				g = col.g;
				b = col.b;
				a = col.a;
			}


			U8 r; U8 g; U8 b; U8 a;
			
	};




	struct Vertex3D{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
		static bool Compare(Vertex3D a, Vertex3D b){
			if (a.normal == b.normal &&
				a.position == b.position &&
				a.uv == b.uv &&
				a.tangent == b.tangent)
				return true;
			return false;
		}
	};

	struct Vertex2D{
		Vertex2D(F32 x, F32 y, F32 u, F32 v){
			position = glm::vec2(x, y);
			uv = glm::vec2(u, v);
		}
		glm::vec2 position;
		glm::vec2 uv;
	};



	struct Texture{
		U32 id;
		U32 width;
		U32 height;
	};
	enum AnchorPoint{ TOP_LEFT,TOP_MIDDLE,TOP_RIGHT,
						MIDDLE_LEFT, CENTER,MIDDLE_RIGHT,
						BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT};




}