#ifndef emit_def
#define emit_def
#include "Global.h"
#include "Model.h"
#include "Structures.h"
#include "GameObject.h"
#include "RynoMath.h"
#include <vector>
#include <functional>
#include "GenericMap.h"

namespace Ryno{
	

	class Particle3D : public GameObject {

	public:
		static void* operator new(size_t size, MemoryLocation mem_loc){
			return Allocator::alloc(size, mem_loc);
		}
		Particle3D();
		Particle3D(GameObject* go, MemoryLocation mem_loc);
		glm::vec3 direction;
		F32 acceleration;
		F32 speed;
		ColorRGBA color;
		F32 lifetime;
		F32 decay_rate = .0001f; 
		GenericMap save_map;		

	private:
		~Particle3D();

	};


class Emitter{

	friend class Particle3D;

	
	public:
		static void* operator new(size_t size, MemoryLocation mem_loc){
			return Allocator::alloc(size, mem_loc);
		}
		Emitter(){}
		Emitter(const Emitter *e);
		
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

		F32 m_max_particles;
		F32 m_elapsed_time;
		F32 m_emission_rate;
	private:
		~Emitter();
		std::vector <Particle3D*> m_particles;
		std::list <Particle3D*> m_pool;
	};


}
#endif 