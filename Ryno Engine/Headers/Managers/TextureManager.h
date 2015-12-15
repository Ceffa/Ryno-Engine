#pragma once
#include <GL\glew.h>
#include <string> 
#include <fstream>
#include <vector>
#include <map>
#include "Global.h"
#include "Structures.h"

#define TEXTURE_LOG 0

namespace Ryno{
	struct GLTexture{
		U64 width;
		U64 height;
	};
	class TextureManager{
	public:
		static TextureManager* get_instance();
		Texture load_png(const std::string& name, Owner loc);
		Texture load_cube_map(const std::string& name, Owner loc);

		void reset();
		
	private:
		TextureManager::TextureManager();
		U8 read_file_to_buffer(const std::string& f_path, std::vector<U8>& buffer);
		std::vector<U32> temporary_textures;
		std::vector<U32> lifetime_textures;

	};
}