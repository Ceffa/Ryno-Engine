#pragma once
#include "Core.h"


namespace RynoEngine {

	class ParticleContact;

	//Resolves a list of conflicts
	class ParticleResolver {

	protected:
		
		U max_iterations;
		U current_iteration;

		void resolve_contacts(ParticleContact* contacts, U contacts_number, F duration);

	public:
		ParticleResolver(U iterations) : max_iterations(iterations) {}
		void set_iterations(U iterations) { max_iterations = iterations; }

	};

}