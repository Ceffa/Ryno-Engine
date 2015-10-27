#include "GameObject.h"
#include "Emitter.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>


namespace Ryno{

	Emitter::Emitter(const Emitter *e){ *this = *e; }

	Emitter::~Emitter(){ m_particles.clear(); }
	
	void Emitter::init(U32 nr_particles, F32 decay_rate, F32 number_per_frame, GameObject* go){

		m_max_particles = nr_particles;
		m_decay_rate = decay_rate;
		m_number_per_frame = number_per_frame;

		for (U32 i = 0; i < nr_particles; i++){
			Particle3D* p = new Particle3D(go);
			
			m_particles.push_back(p);
			p->active = false;
		}

		delete go;

	}


	void Emitter::update(F32 delta_time){

		lambda_emitter(this, delta_time);
		
		for (Particle3D* p : m_particles){
			if (p->active){
				lambda_particle(p, delta_time);
				p->lifetime += m_decay_rate * delta_time;
				if (p->lifetime >= 1.0f)
					p->active = false;
			}
		}
		}
	

	Particle3D* Emitter::new_particle(){
		Particle3D* ret = m_particles[0]; //override first particle, whatever
		for (Particle3D* p : m_particles)
			if (!p->active)
				ret = p;
				
		ret->active = true;
		ret->lifetime = 0;
		return ret; 
	}

	Particle3D::Particle3D(GameObject* go): GameObject(go)
	{
	}
	

}

