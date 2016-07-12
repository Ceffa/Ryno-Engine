#pragma once
#include "Core.h"
#include "Particle.h"


namespace RynoEngine {

	class ParticleContact  {

	public:
		Particle* particles[2];

		F restitution;		//Percentage of impulse
		F penetration;		//If positive, they are interpenetrating
		V3 contact_normal;	//Normalized direction of the contact

	protected:
		//Resolve collision (by resolving velocity and interpenetration)
		void resolve(F duration);	
		F calculate_separating_velocity() const;

	private:
		void resolve_velocity(F duration);
		void resolve_interpenetration(F duration);

	};

}