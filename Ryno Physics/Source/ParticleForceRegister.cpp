#include "ParticleForceRegister.h"

namespace RynoPhysics {

	
	void ParticleForceRegister::add(Particle* p, ParticleForceGenerator* fg)
	{
		registrations.emplace_back(p, fg);
	}

	void ParticleForceRegister::remove(Particle* p, ParticleForceGenerator* fg)
	{
		auto r = registrations.begin();
		for (; r != registrations.end();r++)
			if (r->particle == p && r->generator == fg)
				registrations.erase(r);
	}

	void ParticleForceRegister::clear()
	{
		registrations.clear();
	}

	void ParticleForceRegister::update_forces(F duration)
	{
		for (auto& r : registrations) {
			r.generator->update_force(r.particle, duration);
		}
	}

}