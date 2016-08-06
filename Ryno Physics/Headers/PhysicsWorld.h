#pragma once
#include "Core.h"
#include "Broad.h"
#include "ContactResolver.h"
#include "ForceRegister.h"



namespace Ryno {

	class PhysicsWorld {
		
	public:
		static PhysicsWorld* get_instance();
		static bool first;
		void set_up();
		void reset() { force_register.clear(); first = true; }
		void add_force(RigidBody* body, ForceGenerator* generator) {
			force_register.add(body, generator);
		}
		void remove_force(RigidBody* body, ForceGenerator* generator) {
			force_register.remove(body, generator);
		}
		void physics_step(F duration);
	private:
		PhysicsWorld() {}
		Broad broad;
		ContactResolver resolver;
		ForceRegister force_register;

		std::vector<RigidBody*> bodies;
	
	};

	



}