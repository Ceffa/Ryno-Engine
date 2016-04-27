#ifndef go_def
#define go_def
#pragma once
#include "Global.h"
#include "Model.h"
#include "Transform.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"
#include "SphereCollider.h"
#include <list>

namespace Ryno{

	class Emitter;
	class GameObject{

	public:
		
		GameObject();
		~GameObject();

		GameObject(const GameObject& go);
		void copy(const GameObject& go);
		void reset_to_null();
		//Status
		bool active = true;

		//Components
		Model* model;
		Transform* transform;
		PointLight* point_light;
		DirectionalLight* dir_light;
		SpotLight* spot_light;
		Emitter* emitter;

		static std::list<GameObject*> game_objects;
		
	};


}

#endif