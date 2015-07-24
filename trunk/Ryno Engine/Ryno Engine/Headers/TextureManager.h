#pragma once
#include <GL\glew.h>
#include <string> 
#include <fstream>
#include <vector>
#include <map>
#include "Types.h"


namespace Ryno{
	struct GLTexture{
		U64 width;
		U64 height;
	};
	class TextureManager{
	public:
		static TextureManager* get_instance();
		U32 loadPNG(const std::string& name);
		U32 loadCubeMap(const std::string& name);
	private:
		TextureManager::TextureManager();
		U8 read_file_to_buffer(const std::string& f_path, std::vector<U8>& buffer);
	};
}