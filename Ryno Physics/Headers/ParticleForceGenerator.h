#pragma once
#include "Core.h"



namespace RynoPhysics {

	class Particle;
	//Abstract class. Each derived class is a
	//generator of a different type of force.
	//A particle must be registered with a Generator
	//through the ParticleForceRegister class
	class ParticleForceGenerator {

	public: 
		
		virtual void update_force(Particle* p, F duration) = 0;

	};

}