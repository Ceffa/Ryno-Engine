#pragma once
#include "Global.h"
#include "Model.h"
#include "Structures.h"
#include "GameObject.h"
#include "RynoMath.h"
#include <vector>
#include <functional>
#include "GenericMap.h"
#include "StackAllocator.h"
#include "Pool.h"

namespace Ryno{
	

	class Particle3D : public GameObject {

	public:

		Particle3D(GameObject* go) : GameObject(go){  }
		Particle3D() : GameObject(){  }
		~Particle3D(){}

		glm::vec3 direction;
		F32 acceleration;
		F32 speed;
		ColorRGBA color;
		F32 lifetime;
		F32 decay_rate = .0001f; 
		GenericMap save_map;		

	

	};


class Emitter{

	friend class Particle3D;

	
	public:
		~Emitter();

		Emitter(GameObject* go);
		Emitter(const Emitter *e, GameObject* go);
		
		void init(U32 nr_particles);
		Particle3D* new_particle();
		void update(F32 delta_time);
		void remove_particle(Particle3D* p);
		void disable();


		GameObject* game_object;
		GenericMap save_map;

		std::function<void(Emitter*, Particle3D*)> lambda_creation;
		std::function<void(Emitter*)> lambda_spawn;
		std::function<void(Emitter*,Particle3D*, F32)> lambda_particle_update;

		F32 m_max_particles = 0;
		F32 m_elapsed_time;
	private:
		New<Pool> pool;
		std::vector <New<Particle3D>> m_particles;
		std::list <Particle3D*> m_pool;
	};


}
