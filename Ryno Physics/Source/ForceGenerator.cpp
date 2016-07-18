#include "ForceGenerator.h"
#include "RigidBody.h"

namespace RynoEngine {

	

	void Gravity::update_force(RigidBody* body, F duration)
	{
		if (!body->has_finite_mass())
			return;

		V3 f = acceleration / body->get_inverse_mass();
		body->add_force(f);
	}

	void Drag::update_force(RigidBody* body, F duration)
	{
		//Get velocity
		V3 force = body->velocity;

		//Get magnitude
		F drag_coeff = glm::length(force);

		//Normalize velocity
		force /= drag_coeff;

		//Calculate drag coefficient
		drag_coeff = k1 * drag_coeff + k2 * drag_coeff * drag_coeff;

		//Multiply force by drag coefficient
		force *= -drag_coeff;

		body->add_force(force);		

	}

	void Spring::update_force(RigidBody* body, F duration)
	{
		V3 world_point = body->get_world_point(attach_point_1);
		V3 force = world_point - other->get_world_point(attach_point_2);
		
		//First, cache the magnitude
		F power = glm::length(force);

		//If bungee and compressed, then do nothing
		if (bungee && power < rest_length)
			return;

		//use magnitude to normalize the force
		force /= power;

		//Calculate final power
		power = k * (power - rest_length);

		//Multiply it by normalized force vector
		force *= -power;

		body->add_force_world(force, world_point);
	}

	void AnchoredSpring::update_force(RigidBody* body, F duration)
	{
		V3 world_point = body->get_world_point(attach_point);
		V3 force = world_point - origin;
		
		//First, cache the magnitude
		F power = glm::length(force);

		//If bungee and compressed, then do nothing
		if (bungee && power < rest_length)
			return;

		//use magnitude to normalize the force
		force /= power;

		//Calculate final power
		power = k * (rest_length - power);

		//Multiply it by normalized force vector
		force *= power;

		body->add_force_world(force,world_point);
	}

	void Buoyancy::update_force(RigidBody* body, F duration)
	{
		F32 pos = body->get_position().y;

		//If outside -> no force
		if (pos >= level + max_depth)
			return;

		V3 force = glm::vec3(0, 0, 0);
		//Max force if fully submerged
		force.y = liquid_density * volume;

		//Reduce force by a factor if not fully submerged
		if (pos > level - max_depth) 
			force.y *= (pos - level - max_depth) / (-2 * max_depth);

		body->add_force(force);
	}

}