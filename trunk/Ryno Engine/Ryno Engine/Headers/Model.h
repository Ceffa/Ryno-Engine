#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureManager.h"
#include "Structures.h"

namespace Ryno{

	
	struct UV{
		void set(U32 _u, U32 _v){
			u = _u;
			v = _v;
		}
		glm::vec2 get_vec_2(){
			return glm::vec2(u, v);
		}
		U32 u;
		U32 v;
	};
	
	class Model{
	public:
		Model();
		~Model(){}

		void set_color(U8 r, U8 g, U8 b, U8 a);
		void set_tiling(U32 u, U32 v);
		void set_mesh_texture_normal(I32 m, I32 t, I32 n);
		glm::mat4 model_matrix;
		U32 texture,normal_map;
		I32 mesh;
		UV tiling;
		ColorRGBA color;
		

		
	};
}