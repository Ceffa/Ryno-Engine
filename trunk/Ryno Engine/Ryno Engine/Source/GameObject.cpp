#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>

namespace Ryno{

	void GameObject::generate_model_matrix(){
		//scale
		model.model_matrix = glm::scale(glm::mat4(1.0), scale);
		//rotate
		model.model_matrix = glm::rotate(model.model_matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		model.model_matrix = glm::rotate(model.model_matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		model.model_matrix = glm::rotate(model.model_matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));
		//translate
		model.model_matrix = glm::translate(model.model_matrix, glm::vec3(position.x,position.y,-position.z));
	}

}

