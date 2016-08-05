#include "Contact.h"


namespace Ryno {

	

	void Contact::set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric)
	{
		bodies[0] = a;
		bodies[1] = b;
		friction = _fric;
		restitution = _rest;
	}

	
	void Contact::calculate_internals(F duration)
	{
		// Check if the first object is NULL, and swap if it is.
		if (!bodies[0]) swap_bodies();

		// Calculate an set of axis at the contact point.
		calculate_contact_basis();

		// Store the relative position of the contact relative to each body
		relative_contact_position[0] = contact_point - bodies[0]->get_position();
		if (bodies[1]) {
			relative_contact_position[1] = contact_point - bodies[1]->get_position();
		}

		// Find the relative velocity of the bodies at the contact point.
		contact_velocity = calculate_local_velocity(0, duration);
		if (bodies[1]) {
			contact_velocity -= calculate_local_velocity(1, duration);
		}

		// Calculate the desired change in velocity for resolution
		calculate_desired_delta_velocity(duration);
	}

	//Build a matrix that serves as basis for the contact.
	//The main axis is the contact normal, the other two are build with 
	//cross products
	void Contact::calculate_contact_basis()
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

	inline V3 Contact::calculate_frictionless_inpulse(M3* inv_inertia_tensor)
	{
		V3 impulse_contact;

		//The following 3 lines use formulas to get the unit linear velocity
		//due only to rotaton
		V3 delta_vel_world = cross(relative_contact_position[0], contact_normal);
		delta_vel_world = inv_inertia_tensor[0] * delta_vel_world;
		delta_vel_world = cross(delta_vel_world, relative_contact_position[0]);

		//Get the delta velocity relative to the normal.
		//This is the same (but faster) than this:
		//F delta_velocity = (glm::transpose(contact_to_world) * delta_vel_world).x;
		F unit_delta_velocity = dot(delta_vel_world, contact_normal);

		// Add the unit linear component of velocity change
		unit_delta_velocity += bodies[0]->get_inverse_mass();

		// Check if we need to the second body's data
		if (bodies[1])
		{
			V3 delta_vel_world = cross(relative_contact_position[0], contact_normal);
			delta_vel_world = inv_inertia_tensor[0] * delta_vel_world;
			delta_vel_world = cross(delta_vel_world, relative_contact_position[0]);

			F delta_velocity = dot(delta_vel_world, contact_normal);

			delta_velocity += bodies[0]->get_inverse_mass();
		}

		// Calculate the required size of the impulse
		// NB: the desired delta velocity is the total amount
		// of velocity we want to introduce. The unit delta velocity 
		// is a UNIT factor that represent how much velocity is produced
		return V3(desired_delta_velocity / unit_delta_velocity,0,0);
	}

	V3 Contact::calculate_local_velocity(U body_index, F duration)
	{
		RigidBody *this_body = bodies[body_index];

		// Work out the velocity of the contact point.
		V3 velocity = cross(this_body->rotation , relative_contact_position[body_index]);	//angular
		velocity += this_body->velocity;	//linear									

		// Turn the velocity into contact-coordinates.
		V3 contactVelocity = glm::transpose(contact_to_world) * velocity;

		return contactVelocity;
	}

	void Contact::calculate_desired_delta_velocity(F duration)
	{
		const static F velocity_limit = 0.25f;
		// Calculate the acceleration induced velocity accumulated this frame
		F velocity_from_acc = dot(bodies[0]->delta_acceleration,contact_normal) * duration;
		

		if (bodies[1])
			velocity_from_acc += dot(bodies[1]->delta_acceleration, contact_normal) * duration;

		// If the velocity is very slow, limit the restitution
		F this_restitution = restitution;
		if (abs(contact_velocity.x) < velocity_limit)
		{
			this_restitution = 0.0f;
		}

		// Combine the bounce velocity with the removed
		// acceleration velocity.
		desired_delta_velocity =
			-contact_velocity.x
			- this_restitution * (contact_velocity.x - velocity_from_acc);
	}

	void Contact::apply_velocity_change(V3 velocity_change[2], V3 rotation_change[2])
	{
		
		M3 inertia_tensor[2];
		inertia_tensor[0] = bodies[0]->inverse_inertia_tensor_world;
		if (bodies[1])
			inertia_tensor[1] = bodies[1]->inverse_inertia_tensor_world;

		// We will calculate the impulse for each contact axis
		V3 impulse_contact = calculate_frictionless_inpulse(inertia_tensor);

		// Convert impulse to world coordinates
		V3 impulse = contact_to_world * impulse_contact;

		// Split in the impulse into linear and rotational components
		V3 impulsive_torque = cross(relative_contact_position[0],impulse);
		rotation_change[0] = inertia_tensor[0] * impulsive_torque;
		velocity_change[0] = impulse *  bodies[0]->get_inverse_mass();

		// Apply the changes
		bodies[0]->velocity += velocity_change[0];
		bodies[0]->rotation += rotation_change[0];

		if (bodies[1])
		{
			impulse *= -1;
			V3 impulsive_torque = cross(relative_contact_position[1], impulse);
			rotation_change[1] = inertia_tensor[1] * impulsive_torque;
			velocity_change[1] = impulse *  bodies[1]->get_inverse_mass();

			// And apply them.
			bodies[1]->velocity += velocity_change[1];
			bodies[1]->rotation += rotation_change[1];
		}
	}


	void Contact::swap_bodies()
	{

		contact_normal *= -1;

		RigidBody *temp = bodies[0];
		bodies[0] = bodies[1];
		bodies[1] = temp;

	}


	void Contact::apply_position_change(V3 linear_change[2],V3 angular_change[2],F penetration)
	{
		const F angular_limit = 0.2f;
		F angular_move[2];
		F linear_move[2];

		F total_inertia = 0;
		F linear_inertia[2];
		F angular_inertia[2];

		// We need to work out the inertia of each object in the direction
		// of the contact normal, due to angular inertia only.
		for (U i = 0; i < 2; i++)
		if (bodies[i])
		{
			// Use the same procedure as for calculating frictionless
			// velocity change to work out the angular inertia.
			// 
			V3 angular_inertia_world = cross(relative_contact_position[i],contact_normal);
			angular_inertia_world =	bodies[i]->inverse_inertia_tensor_world * angular_inertia_world;
			angular_inertia_world =	cross(angular_inertia_world,relative_contact_position[i]);
			angular_inertia[i] = dot(angular_inertia_world,contact_normal);

			// The linear component is simply the inverse mass
			linear_inertia[i] = bodies[i]->get_inverse_mass();

			// Keep track of the total inertia from all components
			total_inertia += linear_inertia[i] + angular_inertia[i];
		}

		// Loop through again calculating and applying the changes
		for (U i = 0; i < 2; i++) if (bodies[i])
		{
			// The linear and angular movements required are in proportion to
			// the two inverse inertias.
			F sign = (i == 0) ? 1 : -1;
			angular_move[i] =
				sign * penetration * (angular_inertia[i] / total_inertia);
			linear_move[i] =
				sign * penetration * (linear_inertia[i] / total_inertia);

			// To avoid angular projections that are too great (when mass is large
			// but inertia tensor is small) limit the angular move.
			V3 projection = relative_contact_position[i] - contact_normal * dot(-relative_contact_position[1], contact_normal);


			//Big rotations could cause problems.
			//So we limit them and transfer part of the movement
			//to the linear  movement.
			//The factor is scaled by the size of the object,
			// which is approximated by the magnitude of the
			F max_magnitude = angular_limit * glm::length(projection);

			F total_move = angular_move[i] + linear_move[i];

			if (angular_move[i] < -max_magnitude)
				angular_move[i] = -max_magnitude;
			else if (angular_move[i] > max_magnitude)
				angular_move[i] = max_magnitude;

			linear_move[i] = total_move - angular_move[i];

			// We have the linear amount of movement required by turning
			// the rigid body (in angularMove[i]). We now need to
			// calculate the desired rotation to achieve that.
			if (angular_move[i] == 0)
			{
				// Easy case - no angular movement means no rotation.
				angular_change[i] = V3(0, 0, 0);
			}
			else
			{
				// Work out the direction we'd like to rotate in.
				V3 target_angular_direction = cross(relative_contact_position[i], contact_normal);

				// Work out the direction we'd need to rotate to achieve that
				angular_change[i] =
					bodies[i]->inverse_inertia_tensor_world * target_angular_direction *
					(angular_move[i] / angular_inertia[i]);
			}

			// Velocity change is easier - it is just the linear movement
			// along the contact normal.
			linear_change[i] = contact_normal * linear_move[i];

			// Now we can start to apply the values we've calculated.
			// Apply the linear movement
			bodies[i]->add_position(linear_change[i]);

			// And the change in orientation.
			bodies[i]->add_orientation(glm::quat(angular_change[i]));			
		}
	}

}