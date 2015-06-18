#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureLoader.h"
#include "Structures.h"

namespace Ryno{

	

	
	class Model{
	public:

		glm::mat4 model_matrix;
		GLTexture texture;
		I32 mesh;
	};
}