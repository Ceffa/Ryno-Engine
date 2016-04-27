#pragma once
#include "Global.h"
#include <GLM/glm.hpp>

namespace Ryno{

	class Transform{
	public:
		Transform(){}
		~Transform(){}
		Transform(const Transform& t);

		//Rotation
		glm::vec3 rotation;

		void set_rotation(F32 y, F32 p, F32 r);
		void add_rotation(F32 y, F32 p, F32 r);
		void set_rotation(glm::vec3 rot){ set_rotation(rot.x, rot.y, rot.z); }
		void add_rotation(glm::vec3 rot){ add_rotation(rot.x, rot.y, rot.z); }

		//Position
		glm::vec3 position;

		void set_position(F32 x, F32 y, F32 z){ position = glm::vec3(x, y, z); }
		void add_position(F32 x, F32 y, F32 z){ position += glm::vec3(x, y, z); }
		void set_position(glm::vec3 pos){ set_position(pos.x, pos.y, pos.z); }
		void add_position(glm::vec3 pos){ add_position(pos.x, pos.y, pos.z); }

		//Scale
		glm::vec3 scale;

		void set_scale(F32 x, F32 y, F32 z){ scale = glm::vec3(x, y, z); }
		void add_scale(F32 x, F32 y, F32 z){ scale += glm::vec3(x, y, z); }
		void set_scale(glm::vec3 sc){ set_scale(sc.x, sc.y, sc.z); }
		void add_scale(glm::vec3 sc){ add_scale(sc.x, sc.y, sc.z); }

		//Model matrix
		glm::mat4 model_matrix;
		glm::mat4 rotation_matrix;
		

		void generate_model_matrix();

	};


}