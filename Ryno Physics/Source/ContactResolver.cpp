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

	void ContactResolver::adjust_positions(Contact* contacts, U num_contacts, F duration)
	{

	}

	void ContactResolver::adjust_velocities(Contact* contacts, U num_contacts, F duration)
	{

	}

}