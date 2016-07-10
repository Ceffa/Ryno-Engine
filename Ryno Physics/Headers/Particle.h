#pragma once
#include "Core.h"


namespace RynoPhysics {

	class Particle {

	protected:
		V3 position;
		V3 velocity;
		V3 acceleration;
		F damping = 0.999;
		
		F inverse_mass;
		V3 force_accumulator;

		void integrate(F duration);

	public: 
		//Inverted mass (and mass) getters and setters
		void set_inverted_mass(F _inverted_mass) { inverse_mass = _inverted_mass; }
		void set_mass_mass(F _mass) { inverse_mass = 1.0/_mass; }
		F get_inverted_mass() { return inverse_mass; }
		F get_mass() { return 1.0/inverse_mass; }
		bool has_finite_mass();

		void add_force(V3& force);
		void add_scaled_force(V3& direction, F intensity);
		void clear_accumulator();

	};

}