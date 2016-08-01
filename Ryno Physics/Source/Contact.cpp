#include "Contact.h"


namespace Ryno {

	void Contact::set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric)
	{
		bodies[0] = a;
		bodies[1] = b;
		friction = _fric;
		restitution = _rest;
	}

	
	//Build a matrix that serves as basis for the contact.
	//The main axis is the contact normal, the other two are build with 
	//cross products
	void Contact::calculate_normal_basis()
	{
		//The two other axis
		V3 contact_tangents[2];

		//We start by randomly picking one of the two other axis. 
		//For example the X world axis (1,0,0). 
		//We have to be careful though: if it is almost in the same direction as the normal
		//the result will be bad. So we pick X or Y depending on how close they are to the normal.
		if (abs(contact_normal.x) > abs(contact_normal.y))
		{
			// Scaling factor to ensure the results are normalised
			const F s = 1.0f / sqrt(contact_normal.z*contact_normal.z +
				contact_normal.x*contact_normal.x);

			//We cross the normal with the picked X world axis.
			//This is a cross product, but I avoid glm because 
			//this way is faster (and it will be used a lot).
			contact_tangents[0].x = contact_normal.z*s;
			contact_tangents[0].y = 0;
			contact_tangents[0].z = -contact_normal.x*s;

			//Then we must modify the randomly picked X axis.
			//By crossing the two already created axes
			contact_tangents[1].x = contact_normal.y*contact_tangents[0].x;
			contact_tangents[1].y = contact_normal.z*contact_tangents[0].x -
				contact_normal.x*contact_tangents[0].z;
			contact_tangents[1].z = -contact_normal.y*contact_tangents[0].x;
		}
		else
		{
			//Same thing but by picking Y
			const F s = 1.0f / sqrt(contact_normal.z*contact_normal.z +
				contact_normal.y*contact_normal.y);

			contact_tangents[0].x = 0;
			contact_tangents[0].y = -contact_normal.z*s;
			contact_tangents[0].z = contact_normal.y*s;

			// The new Y-axis is at right angles to the new X- and Z- axes
			contact_tangents[1].x = contact_normal.y*contact_tangents[0].z -
				contact_normal.z*contact_tangents[0].y;
			contact_tangents[1].y = -contact_normal.x*contact_tangents[0].z;
			contact_tangents[1].z = contact_normal.x*contact_tangents[0].y;
		}

		// Make a matrix from the three vectors.
		contact_to_world[0] = contact_normal;
		contact_to_world[1] = contact_tangents[0];
		contact_to_world[2] = contact_tangents[1];
		
	}

}