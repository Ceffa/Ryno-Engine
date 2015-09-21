#pragma once
#include "Global.h"
#include "Model.h"
#include "Transform.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"
#include <list>

namespace Ryno{

	class GameObject{
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject *go);

		//Components
		Model* model;
		Transform* transform;
		PointLight* point_light = nullptr;
		DirectionalLight* dir_light = nullptr;
		SpotLight* spot_light = nullptr;
		
		//Static GameObjects List
		static std::list<GameObject*> game_objects;
	

		
	};


}