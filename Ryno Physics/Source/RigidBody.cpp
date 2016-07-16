#include "RigidBody.h"
#include <iostream>

namespace RynoEngine {

	void RigidBody::integrate(F duration)
	{

	
	}

	bool RigidBody::has_finite_mass()
	{
		return inverse_mass > 0.0;
	}

	void RigidBody::add_force(V3& force)
	{
		force_accumulator += force;
	}
	void RigidBody::add_torque(V3& torque)
	{
		torque_accumulator += torque;
	}

	void RigidBody::add_scaled_force(V3& direction, F intensity)
	{
		add_force(direction*intensity);
	}

	void RigidBody::clear_accumulators()
	{
		force_accumulator.x = 0;
		force_accumulator.y = 0;
		force_accumulator.z = 0;
		torque_accumulator.x = 0;
		torque_accumulator.y = 0;
		torque_accumulator.z = 0;
	}

	void RigidBody::add_scaled_torque(V3& direction, F intensity)
	{
		add_torque(direction*intensity);
	}

	void RigidBody::calculate_derived_data()
	{
		//I should call here the generation of the transform matrix,
		//but maybe I can reuse the transform one.
		get_world_inverse_inertia_tensor();
	}

	void RigidBody::get_world_inverse_inertia_tensor()
	{
		inverse_inertia_tensor_world = M3(get_transform_matrix()) * inverse_inertia_tensor;
	}

}