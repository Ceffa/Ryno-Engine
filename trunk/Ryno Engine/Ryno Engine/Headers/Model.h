#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureManager.h"
#include "Structures.h"

namespace Ryno{

	

	
	class Model{
	public:

		void set_color(U8 r, U8 g, U8 b, U8 a);
		glm::mat4 model_matrix;
		U32 texture;
		I32 mesh;
		ColorRGBA color;
		//glm::vec4 color;

		
	};
}