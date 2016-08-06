#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Narrow.h"



namespace Ryno {


	PhysicsWorld* PhysicsWorld::get_instance() {

		static PhysicsWorld instance;//only at the beginning
		return &instance;
	}
	bool PhysicsWorld::first = true;
	void PhysicsWorld::physics_step(F duration)
	{
		//Add and initialize bodies
		broad.clear();
		bodies.clear();

		for (auto go : GameObject::game_objects) if (go->active) {
			RigidBody* r = go->get_script<RigidBody>();
			if (r) {
				bodies.push_back(r);
				broad.insert(r);
				r->calculate_derived_data();
			}
		}

		//Update forces
		force_register.update_forces(duration);

		//Integrate bodies
		for (auto go : bodies)
			go->integrate(duration);

		std::vector<PotentialContact> potential_contacts = broad.get_contacts();
		CollisionDetector::detect_all_contacts(potential_contacts);
		//std::cout << CollisionDetector::data.max_contacts - CollisionDetector::data.remaining_contacts << std::endl;
		
		if (first) {
			first = false;
		}
		else {

			resolver.resolve_contacts(CollisionDetector::data, duration);
		}

	}




	void PhysicsWorld::set_up()
	{
		CollisionDetector::set_up();
		resolver.set_up(100, 100, .01f, .01f);
	}

}
