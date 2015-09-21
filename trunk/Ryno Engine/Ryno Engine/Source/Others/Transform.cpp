#include "Transform.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	Transform::Transform(const Transform *t)
	{
		*this = *t;
	}

	void Transform::generate_model_matrix(){

		model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, -position.z)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(glm::quat(glm::vec3(0, 0, rotation.z))*glm::quat(glm::vec3(rotation.y, 0, 0))*glm::quat(glm::vec3(0, rotation.x, 0))),
			//Scaling the rot-trans matrix
			scale);


	}


	void Transform::add_rotation(F32 y, F32 p, F32 r){

		rotation.y += p;
		rotation.x += y;
		rotation.z += r;

		if (rotation.y > M_PI_2 || rotation.y <0) rotation.y -= (F32)((I32)(rotation.y / M_PI_2))*M_PI_2;

		if (rotation.x > M_PI_2 || rotation.x <0) rotation.x -= (F32)((I32)(rotation.x / M_PI_2))*M_PI_2;

		if (rotation.z > M_PI_2 || rotation.z < 0) rotation.z -= (F32)((I32)(rotation.z / M_PI_2))*M_PI_2;
	}
	void Transform::set_rotation(F32 y, F32 p, F32 r){

		rotation.y = p;
		rotation.x = y;
		rotation.z = r;

		if (rotation.y > M_PI_2 || rotation.y <0) rotation.y -= (F32)((I32)(rotation.y / M_PI_2))*M_PI_2;

		if (rotation.x > M_PI_2 || rotation.x <0) rotation.x -= (F32)((I32)(rotation.x / M_PI_2))*M_PI_2;

		if (rotation.z > M_PI_2 || rotation.z < 0) rotation.z -= (F32)((I32)(rotation.z / M_PI_2))*M_PI_2;
	}
}