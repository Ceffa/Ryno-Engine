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

		//Status
		bool active = true;

		//Components
		New<Model> model;
		New<Transform> transform;
		New<PointLight> point_light;
		New<DirectionalLight> dir_light;
		New<SpotLight> spot_light;
		New<Emitter> emitter;

		static std::list<GameObject*> game_objects;
		
	};


}

#endif