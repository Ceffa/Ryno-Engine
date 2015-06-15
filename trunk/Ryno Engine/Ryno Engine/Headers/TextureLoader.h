#pragma once
#include <GL\glew.h>
#include <string> 
#include <fstream>
#include <vector>
#include "Types.h"

struct GLTexture{
	U32 id;
	U64 width;
	U64 height;
};
class TextureLoader{
public:
	static  GLTexture loadPNG(const std::string& name);
	static U8 read_file_to_buffer(const std::string& f_path, std::vector<U8>& buffer);
};
