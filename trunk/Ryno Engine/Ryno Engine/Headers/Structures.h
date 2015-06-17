#pragma once
#include <GLm/glm.hpp>


struct ColorRGBA{
	
	ColorRGBA() : r(0), g(0), b(0), a(0){}
	ColorRGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : r(R), g(G), b(B), a(A){}

	U8 r; U8 g; U8 b; U8 a;
	

};
struct Vertex3D{
	Vertex3D() : color(ColorRGBA::ColorRGBA()){}
	glm::vec3 position;
	glm::vec2 uv;
	ColorRGBA color;
	glm::vec3 normal;
};