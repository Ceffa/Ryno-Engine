#pragma once
#include "Core.h"
#include "RigidBody.h"


namespace Ryno {

	class Contact {
		friend class ContactResolver;
	public:
		RigidBody* bodies[2];

		F restitution;				//Percentage of impulse
		F penetration;				//If positive, they are interpenetrating
		F friction;					//Friction
		V3 contact_point;			//Contact point in world coordinates
		V3 contact_normal;			//Normalized direction of the contact
		V3 bodies_movements[2];		//Holds the movement after resolving interpenetration

		void set_body_data(RigidBody* a, RigidBody* b, F _rest, F _fric);

	protected:
		void calculate_internals(F duration);
		void calculate_contact_basis();
		void calculate_desired_delta_velocity(F duration);
		void apply_velocity_change(V3 velocity_change[2], V3 rotation_change[2], F duration);
		void apply_position_change(V3 linear_change[2], V3 angular_change[2], F penetration);

		inline V3 calculate_frictionless_impulse(M3* inv_inertia_tensor);
		inline V3 calculate_friction_impulse(M3* inv_inertia_tensor);
		V3 calculate_local_velocity(U body_index, F duration);
		M3 contact_to_world;

		//Holds the required velocity to be resolved
		F desired_delta_velocity;
		//contact velocity
		V3 contact_velocity;

		//World position of each contact point relative to
		//the center of the body
		V3 relative_contact_position[2];

		void swap_bodies();

	};

	



}