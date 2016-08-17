#include "RigidBody.h"
#include "Narrow.h"
#include <iostream>

namespace Ryno {

	void RigidBody::integrate(F duration)
	{
		if (!has_finite_mass() || !is_awake) return;

		//Calculate accelerations (using mass and tensor respectively)
		delta_acceleration = acceleration + force_accumulator * inverse_mass;
		
		V3 angular_acceleration = inverse_inertia_tensor * torque_accumulator;

		F current_motion;
		if (can_sleep) {
			current_motion = glm::length2(velocity) + glm::length2(rotation);
		}
		//Calculate velocities
		velocity += delta_acceleration * duration;								//Increment velocity
		velocity *= pow(linear_damping, duration);								//Damp velocity by factor d^t

		rotation += angular_acceleration * duration;
		rotation *= pow(angular_damping, duration);

		//Calculate positions
		game_object->transform.add_position(velocity * duration + .5f * acceleration * duration * duration);
		
		game_object->transform.add_rotation(rotation * duration + .5f * angular_acceleration * duration * duration);
		
		calculate_derived_data();
		
		//Reset force and torque accumuators
		clear_accumulators();

		static F base_bias = 0.3;
		if (can_sleep) {
			
			F bias = pow(base_bias, duration);
			motion = bias*motion + (1 - bias)*current_motion;
			Log::println(motion);

			if (motion < sleep_epsilon) set_awake(false);
			else if (motion > 10 * sleep_epsilon) motion = 10 * sleep_epsilon;
		}
	}

	bool RigidBody::has_finite_mass()
	{
		return inverse_mass > 0.0;
	}

	void RigidBody::add_torque(V3& torque)
	{
		torque_accumulator += torque;
		set_awake();
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
		set_awake();
	}	

	void RigidBody::calculate_derived_data()
	{
		game_object->transform.generate_physics_matrix();

		calculate_world_inverse_inertia_tensor();
		for (auto p : primitives)
		{
			p->calculate_transform();
		}
	}

	void RigidBody::calculate_world_inverse_inertia_tensor()
	{
		inverse_inertia_tensor_world = M3(get_transform_matrix()) * inverse_inertia_tensor;
	}

	void RigidBody::add_force_world(const V3& force, const V3& point)
	{
		V3 dis = point - get_position();
		
		force_accumulator += force;
		torque_accumulator += cross(dis, force);

		set_awake();
	}

	V3 RigidBody::get_world_point(const V3& point)
	{
		return V3(M4(get_transform_matrix()) * V4(point,1));
	}

	void RigidBody::add_force_body(const V3& force, const V3& point)
	{
		add_force_world(force, get_world_point(point));
	}

	RigidBody::RigidBody(const RigidBody& copy)
	{
		*this = copy;
		primitives.resize(copy.primitives.size());
		for (U i = 0; i < primitives.size(); i++){
			
			primitives[i] = primitives[i]->clone();
			primitives[i]->body = this;
		}
	}

	void RigidBody::set_awake(const bool awake)
	{
		if (awake) {
			motion = sleep_epsilon*2.0;//avoid sleep again soon
			is_awake = true;
		}
		else {
			is_awake = false;
			velocity = V3(0);
			rotation = V3(0);
		}
		Log::println(awake ? "yes" : "no");

	}

	void RigidBody::set_can_sleep(const bool _can_sleep)
	{
		can_sleep = _can_sleep;

		if (!can_sleep && !is_awake) set_awake();
	}

	F RigidBody::sleep_epsilon = 1;

}