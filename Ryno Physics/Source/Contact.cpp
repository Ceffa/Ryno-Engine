#include "Contact.h"


namespace Ryno {

	void Contact::set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric)
	{
		bodies[0] = a;
		bodies[1] = b;
		friction = _fric;
		restitution = _rest;
	}

	

}