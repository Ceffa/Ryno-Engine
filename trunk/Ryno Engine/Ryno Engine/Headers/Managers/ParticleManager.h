#pragma once
#include "Emitter.h"
#include "GameObject.h" 
#include <functional>
#include <vector>
#include "Global.h"
#include "Structures.h"


namespace Ryno{
	
	class ParticleManager{
	public:
		static ParticleManager* get_instance();
		void init(){}
		void add_emitter(GameObject* emitter){ emitters.push_back(emitter); }
		void remove_emitter(GameObject* emitter){ emitters.remove(emitter); }
		void update(F32 delta);
	private:
		std::list <GameObject*> emitters;
		ParticleManager(){}
		~ParticleManager(){}
	};
}