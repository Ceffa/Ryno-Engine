#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		model = new Model();
		game_objects.push_back(this);
	}
	GameObject::GameObject(const GameObject *go)
	{
		*this = *go;
		model = new Model(go->model);
		game_objects.push_back(this);
	}
	GameObject::~GameObject(){
		game_objects.remove(this);
	}

	void GameObject::generate_model_matrix(){
	
		model->model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, -position.z)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(glm::quat(glm::vec3(0, 0, roll))*glm::quat(glm::vec3(pitch, 0, 0))*glm::quat(glm::vec3(0, yaw, 0))),
			//Scaling the rot-trans matrix
			scale);
		

	}


	void GameObject::rotate(F32 y, F32 p, F32 r){
		
		pitch += p;
		yaw += y;
		roll += r;

		if (pitch > M_PI_2 || pitch <0) pitch -= (F32)((I32)(pitch / M_PI_2))*M_PI_2;
		
		if (yaw > M_PI_2 || yaw <0) yaw -= (F32)((I32)(yaw / M_PI_2))*M_PI_2;
		
		if (roll > M_PI_2 || roll <0) roll -= (F32)((I32)(roll / M_PI_2))*M_PI_2;
	}

}

