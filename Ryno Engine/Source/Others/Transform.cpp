#include "Transform.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	Transform::Transform(const Transform& t)
	{
		*this = t;
	}


	void Transform::generate_model_matrix() {

		rotation_matrix = glm::toMat4(rotation);

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
	
}