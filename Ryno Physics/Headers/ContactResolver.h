#pragma once
#include "Core.h"
#include "Contact.h"
#include "Narrow.h"


namespace Ryno {

	class ContactResolver {
	public:
		void resolve_contacts(CollisionData& coll_data, F duration);
	protected:
		void prepare_contacts(Contact* contacts, U num_contacts, F duration);
		void adjust_positions(Contact* contacts, U num_contacts, F duration);
		void adjust_velocities(Contact* contacts, U num_contacts, F duration);

	private:
		U position_iterations, position_iterations_used;
		U velocity_iterations, velocity_iterations_used;

		F position_epsilon; //Under this value, ignore interpenetrarion
		F velocity_epsilon; //Under this value, ignore velocity adjustment

	

	};

}