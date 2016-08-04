#include "ContactResolver.h"


namespace Ryno {

	void ContactResolver::resolve_contacts(CollisionData& coll_data, F duration)
	{
		Contact* contacts = coll_data.contacts;
		U num_contacts = coll_data.max_contacts - coll_data.remaining_contacts;

		if (num_contacts == 0)
			return;

		prepare_contacts(contacts, num_contacts, duration);
		adjust_positions(contacts, num_contacts, duration);
		adjust_velocities(contacts, num_contacts, duration);

	}


	void ContactResolver::prepare_contacts(Contact* contacts, U num_contacts, F duration)
	{
		for (U i = 0; i < num_contacts; i++) {
			(contacts + i)->calculate_internals(duration);
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

	}

}