#include "ParticleForceGenerator.h"
#include "Particle.h"

namespace RynoPhysics {

	

	void ParticleGravity::update_force(Particle* particle, F duration)
	{
		if (!particle->has_finite_mass())
			return;

		V3 f = acceleration * particle->get_mass();
		particle->add_force(f);
	}

	void ParticleDrag::update_force(Particle* particle, F duration)
	{
		//Get velocity
		V3 force(particle->get_velocity());

		//Get magnitude
		F drag_coeff = force.length();

		//Normalize velocity
		force /= drag_coeff;

		//Calculate drag coefficient
		drag_coeff = k1 * drag_coeff + k2 * drag_coeff * drag_coeff;

		//Multiply force by drag coefficient
		force *= -drag_coeff;

		particle->add_force(force);		

	}

}