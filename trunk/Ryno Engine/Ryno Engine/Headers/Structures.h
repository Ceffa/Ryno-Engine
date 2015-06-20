#pragma once
#include <GLm/glm.hpp>
#include "Types.h"

struct ColorRGBA{
	
	ColorRGBA() : r(0), g(0), b(0), a(0){}
	ColorRGBA(U8 R, U8 G, U8 B, U8 A) : r(R), g(G), b(B), a(A){}

	U8 r; U8 g; U8 b; U8 a;
	

};
