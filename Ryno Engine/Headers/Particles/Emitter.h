#pragma once
#include "Global.h"
#include "Model.h"
#include "Structures.h"
#include "GameObject.h"
#include "RynoMath.h"
#include <vector>
#include <functional>
#include "GenericMap.h"
#include "Script.h" 

namespace RynoEngine{
	

	class Particle3D : public GameObject {

	public:

		Particle3D(const Particle3D& go) {}
		Particle3D() {}
		~Particle3D() {}

		glm::vec3 direction;
		F32 acceleration;
		F32 speed;
		ColorRGBA color;
		F32 lifetime;
		F32 decay_rate = .0001f; 
		GenericMap save_map;		

	

	};


class Emitter : public Script{


	friend class Particle3D;

	
	public:
		~Emitter();

		Emitter() {}
		Emitter(const Emitter& e);

		bool is_unique() override { return false; }
		bool is_copyable() override { return true; }
		Emitter* clone() override { return new Emitter(*this); }
		
		void init(U32 nr_particles);
		Particle3D* new_particle();
		void update(F32 delta_time);
		void remove_particle(Particle3D* p);
		void disable();


		GenericMap save_map;

		std::function<void(Emitter*, Particle3D*)> lambda_creation;
		std::function<void(Emitter*)> lambda_spawn;
		std::function<void(Emitter*,Particle3D*, F32)> lambda_particle_update;

		F32 m_elapsed_time;
private:
		std::vector <Particle3D> m_particles;
		std::list <Particle3D*> m_pool;
	};


}
