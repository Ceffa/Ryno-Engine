#include "Contact.h"


namespace Ryno {

	

	void Contact::set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric)
	{
		bodies[0] = a;
		bodies[1] = b;
		friction = 10;
		restitution = 0;
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

	inline V3 Contact::calculate_frictionless_impulse(M3* inv_inertia_tensor)
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

	inline V3 Contact::calculate_friction_impulse(M3* inv_inertia_tensor)
	{

		F inverse_mass = bodies[0]->get_inverse_mass();
		auto& o = relative_contact_position[0];

		//Usually you gt a torque from an impulse by a cross product with the relative distance.
		//If we build a M3 from the relative distance in this way, we have a matrix
		//that goes from impulse to torque
		M3 impulse_to_torque = M3(
			0, -o.z, o.y,
			o.z, 0, -o.x,
			-o.y, o.x, 0
			);
		M3 delta_vel_world = impulse_to_torque;
		delta_vel_world *= inv_inertia_tensor[0];	//Rotation generated by unit torque
		delta_vel_world *= impulse_to_torque;		//Same as crossing the relative contact pos. Gives linear vel generated by torque only
		delta_vel_world *= -1;						//M1 x M2 = - M2 x M1, thus the minus


		// Check if we need to the second body's data
		if (bodies[1])
		{
			auto& o = relative_contact_position[1];
			impulse_to_torque = M3(
				0, -o.z, o.y,
				o.z, 0, -o.x,
				-o.y, o.x, 0
				);

			M3 delta_vel_world_2 = impulse_to_torque;
			delta_vel_world_2 *= inv_inertia_tensor[1];	//Rotation generated by unit torque
			delta_vel_world_2 *= impulse_to_torque;		//Same as crossing the relative contact pos. Gives linear vel generated by torque only
			delta_vel_world_2 *= -1;						//M1 x M2 = - M2 x M1, thus the minus

			//combine velocities and masses
			delta_vel_world += delta_vel_world_2;
			inverse_mass += bodies[1]->get_inverse_mass();
		}

		//change base of matrix (B x M x B^-1) to get velocity in contact coords
		M3 delta_velocity = transpose(contact_to_world) * delta_vel_world * contact_to_world;
		//Add the linear velocity. Need to build a matrix like this:
		//m,0,0
		//0,m,0
		//0,0,m
		delta_velocity[0].x += inverse_mass;
		delta_velocity[1].y += inverse_mass;
		delta_velocity[2].z += inverse_mass;


		//From: 
		//unit velocity generated by impulse
		//To:
		//Impulse requires to generate unit velocity
		M3 impulse_matrix = inverse(delta_velocity);

		//Velocities to kill. The x element is the same as the frictionless impulse
		//and must be completely null out
		V3 vel_kill(desired_delta_velocity, -contact_velocity.y, -contact_velocity.z);

		//Get impulse necessary to kill the velocities
		V3 impulse_contact = impulse_matrix * vel_kill;

		//Get only the planar impulse
		F planar_impulse = sqrt(impulse_contact.y*impulse_contact.y + impulse_contact.z * impulse_contact.z);

			std::cout << glm::to_string(impulse_contact) << std::endl;

		//The X of the impulse is the one along the normal.
		//This impulse, multiplied by friction, is the amount to overcome.
		//If the planar is greater, than there is enough force for dynamic friction
		if (planar_impulse > impulse_contact.x * friction) {
			impulse_contact.y /= planar_impulse;
			impulse_contact.z /= planar_impulse;

			//Recalculate delta_velocity (and store it in impulse_contact.X just for caching)
			impulse_contact.x =
				delta_velocity[0].x +
				delta_velocity[1].x * impulse_contact.y * friction +
				delta_velocity[2].x * impulse_contact.z * friction;
			impulse_contact.x = desired_delta_velocity / impulse_contact.x;
			impulse_contact.y *= friction * impulse_contact.x;
			impulse_contact.z *= friction * impulse_contact.x;

		}

		return impulse_contact;

	}

	V3 Contact::calculate_local_velocity(U body_index, F duration)
	{
		RigidBody *this_body = bodies[body_index];

		// Work out the velocity of the contact point.
		V3 velocity = cross(this_body->rotation , relative_contact_position[body_index]);	//angular
		velocity += this_body->velocity;	//linear									

		// Turn the velocity into contact-coordinates.
		V3 contact_velocity = glm::transpose(contact_to_world) * velocity;

		V3 acc_velocity = this_body->delta_acceleration * duration;

		// Calculate the velocity in contact-coordinates.
		acc_velocity = glm::transpose(contact_to_world) * acc_velocity;

		// We ignore any component of acceleration in the contact normal
		// direction, we are only interested in planar acceleration
		acc_velocity.x = 0;

		// Add the planar velocities - if there's enough friction they will
		// be removed during velocity resolution
		contact_velocity += acc_velocity;

		return contact_velocity;
	}

	void Contact::calculate_desired_delta_velocity(F duration)
	{
		const static F velocity_limit = 0.25f;

		V3 scaled_normal = contact_normal*duration;
		// Calculate the acceleration induced velocity accumulated this frame
		F velocity_from_acc = dot(bodies[0]->delta_acceleration, scaled_normal);
		

		if (bodies[1])
			velocity_from_acc -= dot(bodies[1]->delta_acceleration, scaled_normal);

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
		V3 impulse_contact = ((friction == 0) ? calculate_frictionless_impulse(inertia_tensor) : calculate_friction_impulse(inertia_tensor));

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
			impulsive_torque = cross(impulse, relative_contact_position[1]);
			rotation_change[1] = inertia_tensor[1] * impulsive_torque;
			velocity_change[1] = impulse *  -bodies[1]->get_inverse_mass();

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