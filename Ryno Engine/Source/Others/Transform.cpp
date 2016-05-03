#include "Transform.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	Transform::Transform(const Transform& t)
	{
		*this = t;
	}

	void Transform::generate_model_matrix(){

		rotation_matrix = glm::toMat4(glm::quat(glm::vec3(0, 0, rotation.z))*glm::quat(glm::vec3(rotation.x, 0, 0))*glm::quat(glm::vec3(0, rotation.y, 0)));

		model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, -position.z)) *
			//Rotation matrix built from three quaternions
			rotation_matrix,
			//Scaling the rot-trans matrix
			scale);


	}


	void Transform::combine_model_matrices()
	{
		Transform* curr_parent = parent;
		hinerited_matrix = glm::mat4();
		while (curr_parent != nullptr) {
			hinerited_matrix = parent->model_matrix * hinerited_matrix;
			curr_parent = curr_parent->parent;
		}
	}

	void Transform::set_parent(Transform* t)
	{
		parent = t;
		t->add_child(this);
	}

	void Transform::remove_parent()
	{
		parent->remove_child(this);
		parent = nullptr;
	}

	void Transform::add_child(Transform* child)
	{
		children.push_back(child);
		child_nr++;
	}

	void Transform::remove_child(Transform* child)
	{
		children.remove(child);
		child_nr--;
	}

	Transform* Transform::get_child(U32 idx)
	{
		if (idx > children.size()) {
			std::list<Transform*>::iterator it = std::next(children.begin(), idx);
			return *it;
		}
		return nullptr;
	}

	void Transform::add_rotation(F32 p, F32 y, F32 r) {

		rotation.y += y* DEG_TO_RAD;
		rotation.x += p* DEG_TO_RAD;
		rotation.z += r* DEG_TO_RAD;

		if (rotation.y > M_PI_2 || rotation.y <0) rotation.y -= (F32)((I32)(rotation.y / M_PI_2))*M_PI_2;

		if (rotation.x > M_PI_2 || rotation.x <0) rotation.x -= (F32)((I32)(rotation.x / M_PI_2))*M_PI_2;

		if (rotation.z > M_PI_2 || rotation.z < 0) rotation.z -= (F32)((I32)(rotation.z / M_PI_2))*M_PI_2;
	}
	void Transform::set_rotation(F32 p, F32 y, F32 r){

		rotation.y = y * DEG_TO_RAD;
		rotation.x = p * DEG_TO_RAD;
		rotation.z = r * DEG_TO_RAD;

		if (rotation.y > M_PI_2 || rotation.y <0) rotation.y -= (F32)((I32)(rotation.y / M_PI_2))*M_PI_2;

		if (rotation.x > M_PI_2 || rotation.x <0) rotation.x -= (F32)((I32)(rotation.x / M_PI_2))*M_PI_2;

		if (rotation.z > M_PI_2 || rotation.z < 0) rotation.z -= (F32)((I32)(rotation.z / M_PI_2))*M_PI_2;
	}
}