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
		Particle3D(){}
		Particle3D(GameObject* go);
		~Particle3D(){} 
		glm::vec3 direction;
		F32 acceleration;
		F32 speed;
		ColorRGBA color;
		F32 lifetime;
		F32 decay_rate = .0001f; 
		Emitter* e;
		GenericMap save_map;		

	};


class Emitter{

	friend class Particle3D;

	
	public:
		Emitter(){}
		Emitter(const Emitter *e);
		~Emitter();
		void init(U32 nr_particles, GameObject* go);
		Particle3D* new_particle();
		void update(F32 delta_time);

		GameObject* game_object;

		std::function<void(Particle3D*, F32)> lambda_particle = [](Particle3D* p, F32 delta_time)
		{
			p->speed += p->acceleration * delta_time;
			p->transform->set_position(p->direction * p->speed * delta_time + p->transform->position);
		};
		std::function<void(Emitter*, F32)> lambda_emitter = [](Emitter* e, F32 delta_time){
			F32 yaw = (rand() % 360) * DEG_TO_RAD;
			F32 pitch = (rand() % 360) * DEG_TO_RAD;
			glm::vec3 dir = normalize(glm::vec3(sin(yaw), -sin(pitch), cos(yaw)));
			Particle3D* p = e->new_particle();
		};
		F32 m_max_particles;
		F32 m_elapsed_time;
		F32 m_emission_rate;
	private:
		
		std::vector <Particle3D*> m_particles;
		std::list <Particle3D*> m_pool;
	};


}
#endif 