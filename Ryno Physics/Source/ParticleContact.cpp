#include "ParticleContact.h"


namespace RynoEngine {

	void ParticleContact::resolve(F duration)
	{
		resolve_velocity(duration);
		resolve_interpenetration(duration);
	}

	F ParticleContact::calculate_separating_velocity() const
	{
		//Return the dot between the relative velocity and the contact normal.
		V3 relative_velocity = particles[0]->get_velocity();
		if (particles[1]) relative_velocity -= particles[1]->get_velocity();
		
		return dot(relative_velocity, contact_normal);
	}

	void ParticleContact::resolve_velocity(F duration)
	{
		F separating_velocity = calculate_separating_velocity();

		//If the separating velocity is > 0, the objects are not colliding
		if (separating_velocity > 0)
			return;

		//Calculate total velocity for the inpulse.
		//It needs to also include the initial velocity (because it should be removed).
		//In case of a perfectly elastic collision, it is double tan the separating velocity
		F delta_velocity = -separating_velocity * (restitution + 1);

		//Calculate global (inverse) mass
		F total_inverse_mass = particles[0]->get_inverted_mass();
		if (particles[1]) total_inverse_mass += particles[1]->get_inverted_mass();

		//If both masses are infinite, return
		if (total_inverse_mass <= 0)
			return;

		//Calculate total inpulse (but not the one applied to each particle)
		F total_inpulse = delta_velocity / total_inverse_mass;
		V3 inpulse_with_direction = total_inpulse * contact_normal;

		//Add a portion of this inpulse to each particle (depending on mass).
		//Invert the sign in second particle
		particles[0]->add_velocity(inpulse_with_direction * particles[0]->get_inverted_mass());
		if (particles[1])
			particles[1]->add_velocity(-inpulse_with_direction * particles[1]->get_inverted_mass());

	}

	void ParticleContact::resolve_interpenetration(F duration)
	{

	}

}