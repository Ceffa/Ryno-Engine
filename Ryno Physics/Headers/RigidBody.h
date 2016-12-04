#pragma once
#include "Core.h"
#include "Component.h"


namespace Ryno {

	class CollisionPrimitive;
	class RigidBody : public Ryno::Component {
		friend class Contact;
	protected:
		F inverse_mass;
		V3 force_accumulator;
		V3 torque_accumulator;
		M3 inverse_inertia_tensor;

		V3 velocity;					//p*
		V3 rotation;					//o*

		//Handle sleep
		bool is_awake;
		bool can_sleep;
		F motion;

		static F sleep_epsilon;

	public: 
		RigidBody() : angular_damping(1), linear_damping(1), can_sleep(true), is_awake(true), motion(2*sleep_epsilon) {}
		bool is_unique() override { return true; }
		bool is_copyable() override { return true; }

		M3 inverse_inertia_tensor_world;

		V3 delta_acceleration;
										//p and o are inside transform
	
		V3 acceleration;				//p**, just for gravity
		F linear_damping = 0;
		F angular_damping = 0;

		std::vector<CollisionPrimitive*> primitives;


		void integrate(F duration);
		
		//Mass
		void set_inverted_mass(F _inverted_mass) { inverse_mass = _inverted_mass; }
		void set_mass(F _mass) { inverse_mass = 1.0/_mass; }
		F get_inverse_mass() { return inverse_mass; }
		F get_mass() { return 1.0/inverse_mass; }
		bool has_finite_mass();

		//Position
		void get_position(V3* p) const{ *p = game_object->transform.get_position(); }
		void set_position(const V3& p) const { game_object->transform.set_position(p); }
		void add_position(const V3& p)const { game_object->transform.add_position(p); }
		const V3& get_position()const {return game_object->transform.get_position(); }

		//Orientation
		void get_orientation(Q* o) const { *o = game_object->transform.get_rotation(); }
		void set_orientation(const Q& o) const { game_object->transform.set_rotation(o); }
		void add_orientation(const Q& o) const { game_object->transform.add_rotation(o); }
		const Q& get_orientation() const { return game_object->transform.get_rotation(); }

		//Force
		void add_force(const V3 &force);						//force only
		void add_force_world(const V3& force, const V3& point);	//may add torque
		void add_force_body(const V3& force, const V3& point);	//may add torque

		//Torque
		void add_torque(V3& torque);

		//Inertia tensor
		void set_inverted_inertia_tensor(const M3& i) { inverse_inertia_tensor = i; }
		void set_inertia_tensor(const M3& i) { set_inverted_inertia_tensor(glm::inverse(i)); }

		//Transform matrix
		const M4& get_transform_matrix() { return game_object->transform.pos_rot_matrix; }

		//Accumulators
		void clear_accumulators();			//Clear forces and torques on the body
		
		//Derived data
		void calculate_derived_data();		//Calculate and cache useful data
		void calculate_world_inverse_inertia_tensor();

		RigidBody* clone() override {
			return new RigidBody(*this);
		}
		RigidBody(const RigidBody& copy);
		~RigidBody() { for (auto p : primitives) delete p; }

		V3 get_world_point(const V3& point);

		template <class T>
		T* add_primitive(const glm::mat4& offset = glm::mat4())
		{
			T* t = new T(this,offset);
			primitives.push_back(t);
			return t;
		}

		template<class T>
		T* get_primitive() {
			for (auto* p : primitives)
				if (T* v = dynamic_cast<T*>(p))
					return v;
			return nullptr;
		}

		void set_awake(const bool awake = true);
		void set_can_sleep(const bool _can_sleep = true);
		bool get_awake() { return is_awake; }
		bool get_can_sleep() { return can_sleep; }

		void set_velocity(const V3& vel) { velocity = vel; set_awake(); }
		void add_velocity(const V3& vel) { set_velocity(vel + velocity); }
		V3& get_velocity() { return velocity; }

		void set_rotation(const V3& rot) { rotation = rot; set_awake(); }
		void add_rotation(const V3& rot) { set_rotation(rot + rotation); }
		V3& get_rotation() { return rotation; }
	
	};


}