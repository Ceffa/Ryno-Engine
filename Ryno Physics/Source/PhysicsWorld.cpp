#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Narrow.h"



namespace Ryno {


	PhysicsWorld* PhysicsWorld::get_instance() {

		static PhysicsWorld instance;//only at the beginning
		return &instance;
	}


	


	void PhysicsWorld::physics_step(F duration)
	{
		//Add and initialize bodies
		broad.clear();
		bodies.clear();

		for (auto b : GameObject::game_objects) if (b->active) {
			RigidBody* r = b->get_script<RigidBody>();
			if (r) {
				r->calculate_derived_data();
				bodies.push_back(r);
				broad.insert(r);
			}
		}

		//Update forces
		force_register.update_forces(duration);

		//Integrate bodies
		for (auto b : bodies) {
			b->integrate(duration);
		}
		
		//Contact resolutions
		std::vector<PotentialContact> potential_contacts = broad.get_contacts();
		CollisionDetector::detect_all_contacts(potential_contacts);
		resolver.resolve_contacts(CollisionDetector::data, duration);
	
	}




	void PhysicsWorld::set_up()
	{
		CollisionDetector::set_up();
		resolver.set_up(100, 100, .01f, .01f);
	}

}
