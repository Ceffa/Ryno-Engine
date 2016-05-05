#include "GameObject.h"
#include "Emitter.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "ParticleManager.h"
#include "Script.h"
#include "Transform.h"

namespace Ryno{



	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		game_objects.push_back(this);
		reset_to_null();
		transform.game_object = this;
	}

	void GameObject::reset_to_null() {

		model = nullptr;
		point_light = nullptr;
		dir_light = nullptr;
		spot_light = nullptr;
		emitter = nullptr;
	}
	GameObject::GameObject(const GameObject& go) 
	{
		copy(go);
		
	}


	void GameObject::copy(const GameObject& go)
	{
		active = go.active;

		reset_to_null();

		transform.copy(go.transform);
		transform.game_object = this;
		if (go.model)
			model = new Model(*go.model);
		if (go.point_light)
			point_light = new PointLight(*go.point_light);
		if (go.spot_light)
			spot_light = new SpotLight(*go.spot_light);
		if (go.dir_light)
			dir_light = new DirectionalLight(*go.dir_light);
		if (go.emitter)
			emitter = new Emitter(*go.emitter, this);

		game_objects.push_back(this);
		
	}

	GameObject::~GameObject() {
		game_objects.remove(this);
		
		if (model)
			delete model;
		if (point_light)
			delete point_light;
		if (spot_light)
			delete spot_light;
		if (dir_light)
			delete dir_light;
		if (emitter)
			delete emitter;
		reset_to_null();
	}

	void GameObject::addScript(Script* s) {
		s->gameObject = this;
		scripts.emplace_back(s);
		s->start();
	}

	

	void GameObject::removeScript(Script* s)
	{
		scripts.remove(s);
		s->gameObject = nullptr;
	}

}

