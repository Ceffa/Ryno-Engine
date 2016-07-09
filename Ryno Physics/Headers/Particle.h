#pragma once
#include "Core.h"


namespace RynoPhysics {

	class Particle {

	protected:
		V3 position;
		V3 velocity;
		V3 acceleration;
		F damping = 0.999;
		
		F inverted_mass;

	public: 
		void set_inverted_mass(F _inverted_mass) { inverted_mass = _inverted_mass; }
		void set_mass_mass(F _mass) { inverted_mass = 1.0/_mass; }



	};

}