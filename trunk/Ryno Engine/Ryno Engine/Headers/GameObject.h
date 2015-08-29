#pragma once
#include "Global.h"
#include "Model.h"

namespace Ryno{

	class GameObject{
	public:
		GameObject();
		~GameObject(){}

		Model model;
		glm::vec3 position;
		glm::vec3 scale;
		F32 yaw, pitch, roll;
		
		void rotate(F32 y, F32 p, F32 r);
		void generate_model_matrix();
	};

}