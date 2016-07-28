#pragma once
#include "Core.h"
#include "RigidBody.h"


namespace Ryno {

	class Contact {
	public:
		RigidBody* particles[2];

		F restitution;				//Percentage of impulse
		F penetration;				//If positive, they are interpenetrating
		V3 contact_point;			//Contact point in world coordinates
		V3 contact_normal;			//Normalized direction of the contact
		V3 bodies_movements[2];	//Holds the movement after resolving interpenetration

	protected:
		//Resolve collision (by resolving velocity and interpenetration)
		void resolve(F duration);
		F calculate_separating_velocity() const;

	private:
		void resolve_velocity(F duration);
		void resolve_interpenetration(F duration);

	};

}