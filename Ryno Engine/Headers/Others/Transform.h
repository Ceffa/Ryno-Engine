#pragma once
#include "Global.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <list>

namespace Ryno{

	class Transform{
	public:
		Transform(){}
		~Transform(){}
		Transform(const Transform& t);

		//Rotation
		glm::quat rotation;

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
		glm::mat4 hinerited_matrix;
		glm::mat4 rotation_matrix;
		
		void generate_model_matrix();
		void combine_model_matrices();

		void set_parent(Transform* t);
		void remove_parent();
		Transform* get_parent() { return parent; }

	private:
		//parent and children
		Transform* parent = nullptr;
		std::list<Transform*> children;
		U32 child_nr = 0;
		void add_child(Transform* child);
		void remove_child(Transform* child);
		Transform* get_child(U32 idx);



	};


}