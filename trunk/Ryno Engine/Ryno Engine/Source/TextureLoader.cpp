#include "TextureLoader.h"
#include "PNGLoader.h"
#include "Log.h"

namespace Ryno{
	GLTexture TextureLoader::loadPNG(const std::string& name){
		std::string path = "Resources/Textures/" + name + ".png";
		GLTexture texture = {};

		std::vector<U8> out;
		U64 width, height;
		std::vector<U8> in;
		if (!read_file_to_buffer(path, in))
			Log::FatalError("Failed to load file to buffer.");

		I32 error_code = PNGLoader::decodePNG(out,width, height, &(in[0]), in.size());
		if (error_code != 0){
			Log::FatalError("Decoding png failed: Errorcode: " + error_code);
		}
		
		texture.width = width;
		texture.height = height;

		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;

	}

	U8 TextureLoader::read_file_to_buffer(const std::string& f_path, std::vector<U8>& buffer){

		std::ifstream file(f_path.c_str(), std::ios::binary); //read binary, everything
		if (file.fail()){
			perror(f_path.c_str());
			return 0;
		}

		//calculate file size
		file.seekg(0, std::ios::end); //seek to the end
		uint32_t size = file.tellg(); //determine 
		file.seekg(0, std::ios::beg);
		size -= file.tellg(); //remove eventual headers still present

		buffer.resize(size);
		file.read((char*)&(buffer[0]), size);//get char address at the beginning of vector
		file.close();
		return 1;
	}
}