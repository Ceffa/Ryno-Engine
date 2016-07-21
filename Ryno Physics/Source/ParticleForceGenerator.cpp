#include "ParticleForceGenerator.h"
#include "Particle.h"

namespace Ryno {

	

	void ParticleGravity::update_force(Particle* particle, F duration)
	{
		if (!particle->has_finite_mass())
			return;

		V3 f = acceleration / particle->get_inverse_mass();
		particle->add_force(f);
	}

	void ParticleDrag::update_force(Particle* particle, F duration)
	{
		//Get velocity
		V3 force = particle->velocity;

		//Get magnitude
		F drag_coeff = glm::length(force);

		//Normalize velocity
		force /= drag_coeff;

		//Calculate drag coefficient
		drag_coeff = k1 * drag_coeff + k2 * drag_coeff * drag_coeff;

		//Multiply force by drag coefficient
		force *= -drag_coeff;

		particle->add_force(force);		

	}

	void ParticleSpring::update_force(Particle* particle, F duration)
	{
		V3 force;
		particle->get_position(&force);
		force -= other->get_position();

		//First, cache the magnitude
		F power = glm::length(force);

		//If bungee and compressed, then do nothing
		if (bungee && power < rest_length)
			return;

		//use magnitude to normalize the force
		force /= power;

		//Calculate final power
		power = k * (power - rest_length);

		//Multiply it by normalized force vector
		force *= -power;

		particle->add_force(force);
	}

	void ParticleAnchoredSpring::update_force(Particle* particle, F duration)
	{
		V3 force;
		particle->get_position(&force);
		force -= origin;

		//First, cache the magnitude
		F power = glm::length(force);

		//If bungee and compressed, then do nothing
		if (bungee && power < rest_length)
			return;

		//use magnitude to normalize the force
		force /= power;

		//Calculate final power
		power = k * (rest_length - power);

		//Multiply it by normalized force vector
		force *= power;

		particle->add_force(force);
	}

	void ParticleBuoyancy::update_force(Particle* particle, F duration)
	{
		F32 pos = particle->get_position().y;

		//If outside -> no force
		if (pos >= level + max_depth)
			return;

		V3 force = glm::vec3(0, 0, 0);
		//Max force if fully submerged
		force.y = liquid_density * volume;

		//Reduce force by a factor if not fully submerged
		if (pos > level - max_depth) 
			force.y *= (pos - level - max_depth) / (-2 * max_depth);

		particle->add_force(force);
	}

}