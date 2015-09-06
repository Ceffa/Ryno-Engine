#pragma once
#include "Global.h"
#include "Structures.h"
#include <GLM/glm.hpp>


namespace Ryno{

	class Sprite{
	public:
		Sprite();
		~Sprite(){}
		Sprite(const Sprite *s);

		void generate_model_matrix();

		glm::mat3 model_matrix;
		glm::vec2 position;
		glm::vec2 scale;
		ColorRGBA color;
		glm::vec2 tiling;
		F32 angle;
		U32 texture;


	};


}