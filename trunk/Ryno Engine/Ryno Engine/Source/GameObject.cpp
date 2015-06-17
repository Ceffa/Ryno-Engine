#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>

namespace Ryno{

	void GameObject::generate_model_matrix(){
		//scale
		model_matrix = glm::scale(glm::mat4(1.0), scale);
		//rotate
		model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));
		//translate
		model_matrix = glm::translate(model_matrix, position);
	}

}

