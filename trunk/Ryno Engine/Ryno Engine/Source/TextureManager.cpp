#include "TextureManager.h"
#include "lodepng.h"
#include "Log.h"
#include <iostream>
namespace Ryno{


	TextureManager::TextureManager(){}

	TextureManager* TextureManager::get_instance(){

		static TextureManager instance;//only at the beginning
		return &instance;
	}

	


	Texture TextureManager::loadPNG(const std::string& name, LocationOfResource loc){
		
		static const std::string middle_path = "Resources/Textures/2D/";

		std::string path = BASE_PATHS[loc] + middle_path + name + ".png";


		std::vector<unsigned char> out;
		unsigned width, height;
		
		

		I32 error_code = lodepng::decode(out, width, height, path.c_str());
		if (error_code != 0){
			Log::FatalError("Decoding png failed: Errorcode: " + error_code);
		}
		
		Texture t;
		t.width = width;
		t.height = height;
		glGenTextures(1, &(t.id));
		glBindTexture(GL_TEXTURE_2D, t.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);


		return t;

	}

	Texture TextureManager::loadCubeMap(const std::string& name, LocationOfResource loc){

		Texture t;
		
		glGenTextures(1, &(t.id));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, t.id);
		static const std::string middle_path = "Resources/Textures/3D/";

		static const std::string pieces[6]{"pos_x", "neg_x", "pos_y", "neg_y", "pos_z", "neg_z"};
		
		unsigned width, height;

		for (U8 i = 0; i < 6; i++){
			std::string path = BASE_PATHS[loc] + middle_path + name + "/"+name+"_"+pieces[i]+".png";


			std::vector<unsigned char> out;
			



			I32 error_code = lodepng::decode(out, width, height, path.c_str());
			if (error_code != 0){
				Log::FatalError("Decoding png failed: Errorcode: " + error_code);
			}

			

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		t.width = width;
		t.height = height;
		return t;

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