#pragma once
#include "Global.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <list>


namespace RynoEngine{

	class GameObject;
	class Transform{
	public:
		Transform(){}
		~Transform(){}
		Transform(const Transform& t);
		void copy(const Transform& t);

		//Position
		void set_position(glm::vec3 pos) { position = pos; }
		void add_position(glm::vec3 pos) { set_position(pos + position); }
		void set_position(F32 x, F32 y, F32 z) { set_position(glm::vec3(x, y, z)); }
		void add_position(F32 x, F32 y, F32 z){ add_position(glm::vec3(x, y, z)); }

		const glm::vec3& get_position() { return position; }
	
		//Scale
		void set_scale(glm::vec3 sc) { scale = sc; }
		void add_scale(glm::vec3 sc) { set_scale(sc + scale); }
		void set_scale(F32 x, F32 y, F32 z) { set_scale(glm::vec3(x, y, z)); }
		void add_scale(F32 x, F32 y, F32 z){ add_scale(glm::vec3(x, y, z)); }

		const glm::vec3& get_scale() { return scale; }

		//Rotation
		void set_rotation(glm::quat q) { rotation = q; }
		void add_rotation(glm::quat q) { set_rotation(q * rotation); }
		void set_rotation(glm::vec3 v) { set_rotation(glm::quat(v * (F32)ryno_math::DegToRad)); }
		void add_rotation(glm::vec3 v) { add_rotation(glm::quat(v* (F32)ryno_math::DegToRad)); }
		void set_rotation(F32 x, F32 y, F32 z) { set_rotation(glm::vec3(x, y, z)); }
		void add_rotation(F32 x, F32 y, F32 z) { add_rotation(glm::vec3(x, y, z)); }

		const glm::quat& get_rotation() { return rotation; }

		//Model matrix
		glm::mat4 model_matrix;
		glm::mat4 hinerited_matrix;
		glm::mat4 rotation_matrix;
		
		void generate_model_matrix();
		void combine_model_matrices();

		void set_parent(Transform* t);
		void remove_parent();
		Transform* get_parent() { return parent; }

		std::list<Transform*> children;
		GameObject* game_object = nullptr;
	private:

		//Rotation
		glm::quat rotation;
		//Position
		glm::vec3 position;
		//Scale
		glm::vec3 scale;

		//parent and children
		Transform* parent = nullptr;
		
		U32 child_nr = 0;
		void add_child(Transform* child);
		void remove_child(Transform* child);
		Transform* get_child(U32 idx);



	};


}