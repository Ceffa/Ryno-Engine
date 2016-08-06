#include "ForceRegister.h"

namespace Ryno {

	
	void ForceRegister::add(RigidBody* body,ForceGenerator* fg)
	{
		registrations.emplace_back(body, fg);
	}

	void ForceRegister::remove(RigidBody* body, ForceGenerator* fg)
	{
		auto r = registrations.begin();
		for (; r != registrations.end();r++)
			if (r->body == body && r->generator == fg)
				registrations.erase(r);
	}

	void ForceRegister::clear()
	{
		registrations.clear();
	}

	void ForceRegister::update_forces(F duration)
	{
		for (auto& r : registrations) {
			r.generator->update_force(r.body, duration);
		}
	}

}