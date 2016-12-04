#include "GameObject.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include "ParticleManager.h"


namespace Ryno{

	Emitter::~Emitter()
	{
		ParticleManager::get_instance()->remove_emitter(this);
	}

	Emitter::Emitter(const Emitter& e){
		//Instead of copying it, create it anew with the values 
		//taken by the old emitter.
		//To know how the particles is build, the first particle
		//in the old pool is casted to GameObject
		//and passed to the init method.
		save_map = e.save_map;
		lambda_spawn = e.lambda_spawn;
		lambda_creation = e.lambda_creation;
		lambda_particle_update = e.lambda_particle_update;
		m_elapsed_time = e.m_elapsed_time;
	}
	
	
	void Emitter::init(U32 nr_particles){
		ParticleManager::get_instance()->add_emitter(this);

		m_particles.resize(nr_particles);

		for (U32 i = 0; i < nr_particles; i++) {
			lambda_creation(this, &m_particles[i]);
			m_particles[i].active = false;
			m_pool.push_back(&m_particles[i]);
		}
	}


	void Emitter::update(F32 delta_time){
		if (!game_object->active) return;
		lambda_spawn(this);
		m_elapsed_time += delta_time;
		
		for (auto& p : m_particles){
		
			if (p.active){
				lambda_particle_update(this,&p, delta_time);
				p.lifetime += p.decay_rate * delta_time;
				if (p.lifetime >= 1.0f){
					remove_particle(&p);
				}
			}
		}
	}
	
	

	void Emitter::remove_particle(Particle3D* p){
		
		//Disable emitter component on the particle, in case it has one
		Emitter* e = p->get_component<Emitter>();
		if (e) e->disable();
		m_pool.push_back(p);
		p->active = false;
		
	}
	void Emitter::disable(){
		for (auto& p : m_particles){
			if (p.active) remove_particle(&p);
		}
	}
	Particle3D* Emitter::new_particle(){
		Particle3D* ret;
		if (m_pool.empty()) ret = &m_particles[0]; //override first particle, whatever
		else 
		{
			ret = m_pool.back();
			m_pool.pop_back();
		}
		ret->active = true;
		ret->lifetime = 0;
		ret->save_map.clear();
		return ret; 
	}

}

