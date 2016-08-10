#include "ContactResolver.h"


namespace Ryno {

	void ContactResolver::set_up(U _pos_iterations, U _vel_iterations, F _pos_epsilon, F _vel_epsilon)
	{
		position_iterations = _pos_iterations;
		velocity_iterations = _vel_iterations;
		position_iterations_used = 0;
		velocity_iterations_used = 0;
		position_epsilon = _pos_epsilon;
		velocity_epsilon = _vel_epsilon;
	}

	void ContactResolver::resolve_contacts(CollisionData& coll_data, F duration)
	{
		//Get contact pointer from data. It needs to be calculated because data
		//always points to the last element
		U num_contacts = coll_data.max_contacts - coll_data.remaining_contacts;
		Contact* contacts = coll_data.contacts - num_contacts;

		if (num_contacts == 0)
			return;
		prepare_contacts(contacts, num_contacts, duration);
		adjust_positions(contacts, num_contacts, duration);
		adjust_velocities(contacts, num_contacts, duration);

	}


	void ContactResolver::prepare_contacts(Contact* contacts, U num_contacts, F duration)
	{
		for (U i = 0; i < num_contacts; i++) {
			contacts[i].calculate_internals(duration);
		}
	}

	void ContactResolver::adjust_positions(Contact *contacts,U num_contacts,F duration)
	{
		U i, index;
		V3 linear_change[2], angular_change[2];
		F max;
		V3 delta_position;

		// iteratively resolve interpenetrations in order of severity.
		position_iterations_used = 0;
		while (position_iterations_used < position_iterations)
		{
			
			// Find biggest penetration
			max = position_epsilon;
			index = num_contacts;
			for (i = 0; i<num_contacts; i++)
			{
				if (contacts[i].penetration > max)
				{
					max = contacts[i].penetration;
					index = i;
				}
			}


			if (index == num_contacts) break;	//no contact found


			// Resolve the penetration.
			contacts[index].apply_position_change(
				linear_change,
				angular_change,
				max);

			// Again this action may have changed the penetration of other
			// bodies, so we update contacts.
			for (i = 0; i < num_contacts; i++)
			{
				// Check each body in the contact
				for (U b = 0; b < 2; b++) if (contacts[i].bodies[b])
				{
					// Check for a match with each body in the newly
					// resolved contact
					for (U d = 0; d < 2; d++)
					{
						if (contacts[i].bodies[b] == contacts[index].bodies[d])
						{
							delta_position = linear_change[d] + cross(angular_change[d],contacts[i].relative_contact_position[b]);

							// The sign of the change is positive if we're
							// dealing with the second body in a contact
							// and negative otherwise (because we're
							// subtracting the resolution)..
							contacts[i].penetration += dot(delta_position,contacts[i].contact_normal)* (b ? 1 : -1);
						}
					}
				}
			}
			position_iterations_used++;
		}
	}

	void ContactResolver::adjust_velocities(Contact* contacts, U num_contacts, F duration)
	{
		V3 velocity_change[2], rotation_change[2];
		V3 delta_vel;

		// iteratively handle impacts in order of severity.
		velocity_iterations_used = 0;
		while (velocity_iterations_used < velocity_iterations)
		{
			// Find contact with maximum magnitude of probable velocity change.
			F max = velocity_epsilon;
			U index = num_contacts;
			for (U i = 0; i < num_contacts; i++)
			{
				if (contacts[i].desired_delta_velocity > max)
				{
					max = contacts[i].desired_delta_velocity;
					index = i;
				}
			}
			if (index == num_contacts) break;

		
			// Do the resolution on the contact that came out top.
			contacts[index].apply_velocity_change(velocity_change, rotation_change, duration);

			// With the change in velocity of the two bodies, the update of
			// contact velocities means that some of the relative closing
			// velocities need recomputing.
			for (U i = 0; i < num_contacts; i++)
			{
				// Check each body in the contact
				for (U b = 0; b < 2; b++) if (contacts[i].bodies[b])
				{
					// Check for a match with each body in the newly
					// resolved contact
					for (U d = 0; d < 2; d++)
					{
						if (contacts[i].bodies[b] == contacts[index].bodies[d])
						{
							delta_vel = velocity_change[d] + cross(rotation_change[d], contacts[i].relative_contact_position[b]);

							// The sign of the change is negative if we're dealing
							// with the second body in a contact.
							contacts[i].contact_velocity += (b ? -1.0f : 1.0f) * (glm::transpose(contacts[i].contact_to_world) * delta_vel);
							contacts[i].calculate_desired_delta_velocity(duration);
						}
					}
				}
			}
			velocity_iterations_used++;
		}
	}

}