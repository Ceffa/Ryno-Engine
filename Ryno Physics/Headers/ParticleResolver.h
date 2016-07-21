#pragma once
#include "Core.h"
#include <vector>

namespace Ryno {

	class ParticleContact;

	//Resolves a list of conflicts
	class ParticleResolver {

	protected:
		
		U max_iterations;
		U current_iteration;


	public:
		ParticleResolver(U iterations) : max_iterations(iterations) {}
		void set_iterations(U iterations) { max_iterations = iterations; }
		void resolve_contacts(const std::vector<ParticleContact*>& contacts, F duration);

	};

}