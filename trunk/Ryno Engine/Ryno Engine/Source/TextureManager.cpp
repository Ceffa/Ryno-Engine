#include "TextureManager.h"
#include "PNGLoader.h"
#include "Log.h"
#include <iostream>

namespace Ryno{


	TextureManager::TextureManager(){}

	TextureManager* TextureManager::get_instance(){

		static TextureManager instance;//only at the beginning
		return &instance;
	}

	


	U32 TextureManager::loadPNG(const std::string& name){
		std::string path = "Resources/Textures/2D/" + name + ".png";
		GLTexture texture = {};


		std::vector<U8> out;
		U64 width, height;
		std::vector<U8> in;
		if (!read_file_to_buffer(path,in)){
			Log::FatalError("Failed to load file to buffer.");
		}

		

		I32 error_code = PNGLoader::decodePNG(out,width, height, in.data(), in.size());
		if (error_code != 0){
			Log::FatalError("Decoding png failed: Errorcode: " + error_code);
		}
		
		texture.width = width;
		texture.height = height;
		U32 texture_id;
		glGenTextures(1, &(texture_id));
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);


		return texture_id;

	}

	U32 TextureManager::loadCubeMap(const std::string& name){

		U32 texture_id;
		glGenTextures(1, &(texture_id));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

		static std::string pieces[6]{"pos_x", "neg_x", "pos_y", "neg_y", "pos_z", "neg_z"};
		
		for (U8 i = 0; i < 6; i++){
			std::string path = "Resources/Textures/3D/" + name + "/"+name+"_"+pieces[i]+".png";
			GLTexture texture = {};


			std::vector<U8> out;
			U64 width, height;
			std::vector<U8> in;
			if (!read_file_to_buffer(path, in)){
				Log::FatalError("Failed to load file to buffer.");
			}



			I32 error_code = PNGLoader::decodePNG(out, width, height, in.data(), in.size());
			if (error_code != 0){
				Log::FatalError("Decoding png failed: Errorcode: " + error_code);
			}

			texture.width = width;
			texture.height = height;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return texture_id;

	}


	U8 TextureManager::read_file_to_buffer(const std::string& f_path, std::vector<U8>& buffer){

		std::ifstream file(f_path.c_str(), std::ios::binary); //read binary, everything
		if (file.fail()){
			perror(f_path.c_str());
			return 0;
		}

		//calculate file size
		file.seekg(0, std::ios::end); //seek to the end
		I32 size = (I32)file.tellg(); //determine 
		file.seekg(0, std::ios::beg);
		size -= (I32)file.tellg(); //remove eventual headers still present

		buffer.resize(size);
		file.read((C*)&(buffer[0]), size);//get char address at the beginning of vector
		file.close();

		
		return 1;
	}
}