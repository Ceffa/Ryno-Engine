#pragma once
#include "Core.h"
#include "Broad.h"
#include "ContactResolver.h"
#include "ForceRegister.h"



namespace Ryno {

	class PhysicsWorld {
		
	public:
		static PhysicsWorld* get_instance();

		void set_up();
		void physics_step(F duration);
	private:
		PhysicsWorld() {}
		Broad broad;
		ContactResolver resolver;
		ForceRegister force_register;

		std::vector<RigidBody*> bodies;
	
	};

	



}