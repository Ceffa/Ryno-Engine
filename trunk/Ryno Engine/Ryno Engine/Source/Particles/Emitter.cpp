#include "GameObject.h"
#include "Emitter.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>


namespace Ryno{

	Emitter::Emitter(const Emitter *e){ *this = *e; }

	Emitter::~Emitter(){ m_particles.clear(); }
	
	void Emitter::init(U32 nr_particles, F32 decay_rate, F32 number_per_frame, GameObject* go,
		std::function<void(Particle3D*, F32)> update_func){

		lambda_particle = update_func;
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

	void Emitter::add_particle(const glm::vec3& position, const glm::vec3& dir, F32 speed, const glm::vec3& size, const ColorRGBA color){
		Particle3D* p = find_free_particle();

		p->active = true;
		p->transform->position = position;
		p->direction = dir;
		p->speed = speed;
		p->model->color = color;
		p->lifetime = 0;
		p->transform->set_scale(size);

	}

	void Emitter::update(F32 delta_time){


		for (U32 i = 0; i < m_number_per_frame; i++){
			F32 yaw = (rand() % 360) * DEG_TO_RAD;
			F32 pitch = (rand() % 360) * DEG_TO_RAD;
			glm::vec3 dir = normalize(glm::vec3(sin(yaw), -sin(pitch), cos(yaw)));
			add_particle(game_object->transform->position, dir, .5f, glm::vec3(50.0f), ColorRGBA(255, 255, 255, 255));
		}

		for (Particle3D* p : m_particles){
			if (p->active){
				lambda_particle(p, delta_time);
				p->lifetime += m_decay_rate * delta_time;
				if (p->lifetime >= 1.0f)
					p->active = false;
			}
		}
		}
	

	Particle3D* Emitter::find_free_particle(){

		for (Particle3D* p : m_particles)
			if (!p->active)
				return p;

		return m_particles[0]; //override first particle, whatever
	}

	Particle3D::Particle3D(GameObject* go): GameObject(go)
	{
	}
	

}

