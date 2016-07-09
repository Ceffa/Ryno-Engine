#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "Script.h"
#include "Transform.h"

namespace RynoEngine{



	std::list<GameObject*> GameObject::game_objects;

	GameObject::GameObject(){
		game_objects.push_back(this);
		transform.game_object = this;
	}

	
	GameObject::GameObject(const GameObject& go) 
	{
		copy(go);
		
	}


	void GameObject::copy(const GameObject& go)
	{
		active = go.active;

		transform.copy(go.transform);
		transform.game_object = this;
	
	 
		game_objects.push_back(this);

		scripts.clear();
		for (auto* s : go.scripts) {
			if (s->is_copyable())
				add_script(s->clone());
		}
		
	}

	GameObject::~GameObject() {

		for (auto* s : scripts) {
			delete s;
			s = nullptr;
		}
		game_objects.remove(this);
	}

	

}

