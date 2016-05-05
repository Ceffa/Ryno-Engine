#pragma once
#include "Global.h"
#include "Model.h"
#include "Transform.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"
#include <list>

namespace Ryno{

	class Emitter;
	class Script;
	class GameObject{
		friend class Scene;
	public:
		
		GameObject();
		~GameObject();

		GameObject(const GameObject& go);
		void copy(const GameObject& go);
		void reset_to_null();
		void addScript(Script* s);
		void removeScript(Script* s);
		template<class T>
		T* GetScript() {
			for (auto* s : scripts)
				if (T* v = dynamic_cast<T*>(s))
					return v;
			return nullptr;
		}
		//Status
		bool active = true;

		//Components
		Model* model;
		Transform transform;
		PointLight* point_light;
		DirectionalLight* dir_light;
		SpotLight* spot_light;
		Emitter* emitter;
		

		static std::list<GameObject*> game_objects;

	private:
		std::list<Script*> scripts;
		
	};


}
