#pragma once
#include "Core.h"
#include "Script.h"
#include "GameObject.h"


namespace RynoEngine {

	class Particle : public RynoEngine::Script {

	protected:
		V3 velocity;
		V3 acceleration;

		F damping = .7f;
		
		F inverse_mass;
		V3 force_accumulator;


	public: 

		V3 frame_acceleration;
		void integrate(F duration);
		
		//Inverted mass (and mass) getters and setters
		void set_inverted_mass(F _inverted_mass) { inverse_mass = _inverted_mass; }
		void set_mass(F _mass) { inverse_mass = 1.0/_mass; }
		F get_inverse_mass() { return inverse_mass; }
		F get_mass() { return 1.0/inverse_mass; }

		bool has_finite_mass();

		void get_velocity(V3* v) { *v = velocity; }
		void get_position(V3* p) { *p = game_object->transform.get_position(); }
		void get_acceleration(V3* a) { *a = acceleration; }
		void set_velocity(const V3& v) { velocity = v; }
		void set_acceleration(const V3& a) { acceleration = a; }
		void add_velocity(const V3& v) { velocity += v; }
		void set_position(const V3& p) { game_object->transform.set_position(p); }
		void add_position(const V3& p) { game_object->transform.add_position(p); }
		
		const V3 get_acceleration() { return acceleration; }
		const V3 get_velocity() { return velocity; }
		const V3 get_position() { return game_object->transform.get_position(); }



		void add_force(V3& force);
		void add_scaled_force(V3& direction, F intensity);
		void clear_accumulator();

		Particle* clone() override {
			return new Particle(*this);
		}

	};

}