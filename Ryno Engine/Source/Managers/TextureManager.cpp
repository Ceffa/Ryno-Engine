#include "TextureManager.h"
#include <string>
#include <iostream>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "ImageLoader.h"
namespace RynoEngine{


	TextureManager::TextureManager(){}

	TextureManager* TextureManager::get_instance(){

		static TextureManager instance;//only at the beginning
		return &instance;
	}

	


	Texture TextureManager::load_png(const std::string& name, Owner loc){
		
		static const std::string middle_path = "Resources/Textures/2D/";

		std::string path = BASE_PATHS[loc] + middle_path + name;

		if (cached_textures.find(path) != cached_textures.end()) {
			return cached_textures[path];
		}


	
		int width, height, comp;
		
		
		unsigned char* image = stbi_load(path.c_str(), &width, &height, &comp, STBI_rgb_alpha);
		
		if (!image){
			std::cout << "Png not found: " << path.c_str() << std::endl;
		}
		
		Texture t;
		t.width = width;
		t.height = height;
		glGenTextures(1, &(t.id));
		glBindTexture(GL_TEXTURE_2D, t.id);
	
		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (loc == GAME)
			temporary_textures.push_back(t.id);
		else
			lifetime_textures.push_back(t.id);


		cached_textures[path] = t;
		stbi_image_free(image);
		return t;

	}

	RynoEngine::Texture TextureManager::load_cube_map(const std::string& name, const std::string& ext, Owner loc){

		static const std::string middle_path = "Resources/Textures/3D/";
		static const std::string pieces[6]{"rt", "lf", "up", "dn", "ft", "bk"};
		std::string base_path = BASE_PATHS[loc] + middle_path + name;
		if (cached_textures.find(base_path) != cached_textures.end()) {
			return cached_textures[base_path];
		}
		Texture t;
		
		glGenTextures(1, &(t.id));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, t.id);
		

		int width, height, comp;

		for (U8 i = 0; i < 6; i++){
			std::string path = base_path + "/"+name+"_"+pieces[i]+ext;



			unsigned char* image = stbi_load(path.c_str(), &width, &height, &comp, STBI_rgb_alpha);
			if (!image){
				std::cout << "Decoding png failed." << std::endl;
			}

			
			/*if(comp == 3)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			else*/
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			
			stbi_image_free(image);

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		t.width = width;
		t.height = height;
		cached_textures[base_path] = t;

		return t;

	}


	void TextureManager::reset()
	{
#if TEXTURE_LOG
		std::cout << "Texture Manager: number of textures: " << temporary_textures.size() << std::endl;
#endif
		glDeleteTextures(temporary_textures.size(), temporary_textures.data());
		temporary_textures.resize(0);
		cached_textures.clear();
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