#include "Particle.h"
#include <iostream>

namespace RynoEngine {

	void Particle::integrate(F duration)
	{

		if (inverse_mass <= 0.0) return;	
		
		delta_position = velocity * duration;
		delta_acceleration = acceleration + force_accumulator * inverse_mass;
		delta_velocity = delta_acceleration * duration;
																		
		game_object->transform.add_position(delta_position);
		

		velocity += delta_velocity;										//Increment velocity
		velocity *= pow(damping, duration);								//Damp velocity by factor d^t

		clear_accumulator();
	}

	bool Particle::has_finite_mass()
	{
		return inverse_mass > 0.0;
	}

	void Particle::add_force(V3& force)
	{
		force_accumulator += force;
	}

	void Particle::add_scaled_force(V3& direction, F intensity)
	{
		add_force(direction*intensity);
	}

	void Particle::clear_accumulator()
	{
		force_accumulator.x = 0;
		force_accumulator.y = 0;
		force_accumulator.z = 0;
	}

}