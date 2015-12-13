#include "GameObject.h"
#include "Emitter.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "ParticleManager.h"

namespace Ryno{



	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		game_objects.push_back(this);
	}
	GameObject::GameObject(const GameObject* go) 
	{
		*this = *go;

		if (*go->transform)
			transform.copy(go->transform);
		if (*go->model)
			model.copy(go->model);
		if (*go->point_light)
			point_light.copy(go->point_light);
		if (*go->spot_light)
			spot_light.copy(go->spot_light);
		if (*go->dir_light)
			dir_light.copy(go->dir_light);
		if (*go->emitter)
			emitter.copy(go->emitter);
		
		game_objects.push_back(this);


	}
	GameObject::~GameObject(){
		game_objects.remove(this);
	}

	

}

