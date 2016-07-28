#pragma once
#include "Core.h"
#include "RigidBody.h"


namespace Ryno {

	class Contact {
	public:
		RigidBody* bodies[2];

		F restitution;				//Percentage of impulse
		F penetration;				//If positive, they are interpenetrating
		F friction;					//Friction
		V3 contact_point;			//Contact point in world coordinates
		V3 contact_normal;			//Normalized direction of the contact
		V3 bodies_movements[2];		//Holds the movement after resolving interpenetration

		void set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric);


	};

}