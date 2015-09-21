#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		transform = new Transform();
		game_objects.push_back(this);
	}
	GameObject::GameObject(const GameObject *go)
	{
		*this = *go;
		
		transform = new Transform(go->transform);

		if (go->model)
			model = new Model(go->model);
		if (go->point_light)
			point_light = new PointLight(go->point_light);
		if (go->spot_light)
			spot_light = new SpotLight(go->spot_light);
		if (go->dir_light)
			dir_light = new DirectionalLight(go->dir_light);

		game_objects.push_back(this);
	}
	GameObject::~GameObject(){
		game_objects.remove(this);
	}

	

}

