#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "Component.h"
#include "Transform.h"
#include <iostream>

namespace Ryno{



	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		game_objects.push_back(this);
		transform.game_object = this;
	}

	
	GameObject::GameObject(const GameObject& go) 
	{
		game_objects.push_back(&copy(go));
	}


	GameObject& GameObject::copy(const GameObject& go)
	{
		active = go.active;

		transform.copy(go.transform);
		transform.game_object = this;
	
		components.clear();
		for (auto s : go.components) {
			if (s->is_copyable())
				add_component(s->clone());
		}
		return *this;
		
	}

	GameObject::~GameObject() {

		for (auto* s : components) {
			delete s;
			s = nullptr;
		}
		game_objects.remove(this);
	}

	

}

