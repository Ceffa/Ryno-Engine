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

	void RigidBody::add_torque(V3& torque)
	{
		torque_accumulator += torque;
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

	void RigidBody::add_force(const V3 &force)
	{
		force_accumulator += force;
		is_awake = true;
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

	void RigidBody::add_force_world(const V3& force, const V3& point)
	{
		V3 dis = point - get_position();
		
		force_accumulator += force;
		torque_accumulator += cross(dis, force);

		is_awake = true;
	}

	V3 RigidBody::get_world_point(const V3& point)
	{
		return V3(M4(get_transform_matrix()) * V4(point,1));
	}

	void RigidBody::add_force_body(const V3& force, const V3& point)
	{
		add_force_world(force, get_world_point(point));
	}

}