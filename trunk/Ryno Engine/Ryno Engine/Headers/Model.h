#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"

namespace Ryno{

	
	
	class Model{
	public:
		Model();
		~Model(){}
		Model(const Model *m);
		void set_color(U8 r, U8 g, U8 b);
		void set_alpha(U8 f);
		void set_color_and_flatness(U8 r, U8 g, U8 b, U8 f);
			
		void set_tiling(U32 u, U32 v);
		void set_texture_normal(I32 t, I32 n);

		glm::mat4 model_matrix;
		U32 texture,normal_map;
		I32 mesh;
		glm::vec2 tiling;
		ColorRGBA color;  //Alpha is flatness
		bool cast_shadows = true;
		
		

		
	};
}