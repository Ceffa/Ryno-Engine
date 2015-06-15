#pragma once
#include <vector>
#include "Types.h"
class PNGLoader{
	public:
		static I32 decodePNG(std::vector<U8>& out_image_32bit, U64& image_width, U64& image_height, const U8* in_png, size_t in_size);
};
