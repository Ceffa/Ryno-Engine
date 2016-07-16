#pragma once
#include "Core.h"
#include "Script.h"


namespace RynoEngine {

	class RigidBody : public RynoEngine::Script {

	protected:
		F inverse_mass;
		V3 force_accumulator;
		V3 torque_accumulator;

	public: 

		V3 delta_acceleration;
										//p and o are inside transform
		V3 velocity;					//p*
		V3 rotation;					//o*
		V3 acceleration;				//p**, just for gravity
		F linear_damping = .8f;
		F angular_damping = .8f;



		void integrate(F duration);
		
		//Inverted mass (and mass) getters and setters
		void set_inverted_mass(F _inverted_mass) { inverse_mass = _inverted_mass; }
		void set_mass(F _mass) { inverse_mass = 1.0/_mass; }
		F get_inverse_mass() { return inverse_mass; }
		F get_mass() { return 1.0/inverse_mass; }

		bool has_finite_mass();

		//Position
		void get_position(V3* p) { *p = game_object->transform.get_position(); }
		void set_position(const V3& p) { game_object->transform.set_position(p); }
		void add_position(const V3& p) { game_object->transform.add_position(p); }
		const V3 get_position() { return game_object->transform.get_position(); }

		//Orientation
		void get_orientation(Q* o) { *o = game_object->transform.get_rotation(); }
		void set_orientation(const Q& o) { game_object->transform.set_rotation(o); }
		void add_orientation(const Q& o) { game_object->transform.add_rotation(o); }
		const Q get_orientation() { return game_object->transform.get_rotation(); }

		//Force
		void add_force(V3& force);
		void add_scaled_force(V3& direction, F intensity);

		//Torque
		void add_torque(V3& torque);
		void add_scaled_torque(V3& direction, F intensity);

		void clear_accumulators();			//Clear forces and torques on the body
		void calculate_derived_data();		//Calculate and cache useful data

		RigidBody* clone() override {
			return new RigidBody(*this);
		}

	};

}