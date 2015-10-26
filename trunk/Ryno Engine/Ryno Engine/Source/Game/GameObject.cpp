#include "GameObject.h"
#include "Emitter.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Ryno{

	void GameObject::set_emitter(Emitter* _emitter)
	{
		emitter = _emitter; 
		emitter->game_object = this;
	}

	Emitter* GameObject::get_emitter()
	{
		return emitter;
	}


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
		if (go->collider)
			collider = go->collider->get_copy();
		if (go->point_light)
			point_light = new PointLight(go->point_light);
		if (go->spot_light)
			spot_light = new SpotLight(go->spot_light);
		if (go->dir_light)
			dir_light = new DirectionalLight(go->dir_light);
		if (go->emitter)
			set_emitter(go->emitter);

		game_objects.push_back(this);
	}
	GameObject::~GameObject(){
		game_objects.remove(this);
	}

	

}

