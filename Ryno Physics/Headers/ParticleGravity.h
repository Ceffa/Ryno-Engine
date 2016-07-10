#pragma once
#include "Core.h"
#include "ParticleForceGenerator.h"


namespace RynoPhysics {

	class ParticleGravity : public ParticleForceGenerator {

	protected:
		V3 acceleration;
	public:
		ParticleGravity(const V3& gravity) : acceleration(gravity) {}
		void update_force(Particle* p, F duration) override;
	};

}