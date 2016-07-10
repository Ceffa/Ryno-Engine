#include "ParticleGravity.h"
#include "Particle.h"

namespace RynoPhysics {

	

	void ParticleGravity::update_force(Particle* particle, F duration)
	{
		if (!particle->has_finite_mass())
			return;

		V3 f = acceleration * particle->get_mass();
		particle->add_force(f);
	}

}