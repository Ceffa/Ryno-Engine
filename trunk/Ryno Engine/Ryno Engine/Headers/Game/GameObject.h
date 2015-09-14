#pragma once
#include "Global.h"
#include "Model.h"
#include <list>

namespace Ryno{

	class GameObject{
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject *go);

		void rotate(F32 y, F32 p, F32 r);
		void generate_model_matrix();

		//Gameobject data
		Model* model;
		glm::vec3 position;
		glm::vec3 scale;
		F32 yaw, pitch, roll;

		//Static gameobjects list
		static std::list<GameObject*> game_objects;
	

		
	};


}