#pragma once
#include "Core.h"
#include "ParticleForceGenerator.h"
#include <vector>;

namespace RynoPhysics {

	class Particle;

	class ParticleForceRegister {

	protected:
		struct RegisterEntry {
			RegisterEntry(Particle*p, ParticleForceGenerator*fg) : particle(p), generator(fg) {}
			Particle* particle;
			ParticleForceGenerator* generator;
		};
		
		typedef std::vector<RegisterEntry> Registry;
		Registry registrations;

	public: 
		void add(Particle* p, ParticleForceGenerator* fg);
		void remove(Particle* p, ParticleForceGenerator* fg);
		void clear();
		void update_forces(F duration);

		



	};

}