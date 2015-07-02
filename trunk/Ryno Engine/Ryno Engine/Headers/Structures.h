#pragma once
#include <GLm/glm.hpp>
#include "Types.h"

struct ColorRGBA{
	
	ColorRGBA() : r(0), g(0), b(0), a(0){}
	ColorRGBA(U8 R, U8 G, U8 B, U8 A) : r(R), g(G), b(B), a(A){}
	void set_color(U8 _r, U8 _g, U8 _b, U8 _a){
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	glm::vec4 to_vec4(){
		return glm::vec4(r, g, b, a);
	}

	U8 r; U8 g; U8 b; U8 a;
	

};
