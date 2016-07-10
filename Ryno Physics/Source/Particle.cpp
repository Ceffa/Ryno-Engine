#include "Particle.h"
#include <iostream>

#define NDEBUG
namespace RynoEngine {

	void Particle::integrate(F duration)
	{

		if (inverse_mass <= 0.0) return;								//Check invalid mass
		
		//Get acceleration from every force
		V3 new_acc = acceleration + force_accumulator * inverse_mass;

		game_object->transform.add_position(velocity * duration);		//Increment position
		velocity += new_acc * duration;									//Increment velocity
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