#pragma once
#include "Core.h"
#include "Contact.h"


namespace Ryno {

	struct PotentialContact {
	public:
		PotentialContact(RigidBody* a, RigidBody* b) {
			bodies[0] = a;
			bodies[1] = b;
		}
		RigidBody* bodies[2];
	};

	class Broad {

	private:
		std::vector<RigidBody*> bodies;
		std::vector<PotentialContact> contacts;
	public:
		Broad() {}

		void insert(RigidBody* b) { bodies.push_back(b); }

		void clear() { contacts.clear(); bodies.clear(); }

		std::vector<PotentialContact> get_contacts() {
			for (U i = 0; i < bodies.size(); i++) {
				for (U j = i+1; j < bodies.size(); j++) {
					contacts.emplace_back(bodies[i], bodies[j]);
				}
			}
			return contacts;
		}

	};

}