#include "Particle.h"
#define NDEBUG
namespace RynoPhysics {

	void Particle::integrate(F duration)
	{
		if (inverse_mass <= 0.0) return;		//Check invalid mass

		assert(duration > 0.0);					//Assert valid duration

		V3 resulting_acceleration = acceleration += force_accumulator * inverse_mass;
		position += velocity * duration;		//Increment position
		velocity += acceleration * duration;	//Increment velocity
		velocity *= pow(damping, duration);		//Damp velocity by factor d^t
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