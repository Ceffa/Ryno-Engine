#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureManager.h"
#include "Structures.h"

namespace Ryno{

	

	
	class Model{
	public:

		glm::mat4 model_matrix;
		U32 texture;
		I32 mesh;
	};
}