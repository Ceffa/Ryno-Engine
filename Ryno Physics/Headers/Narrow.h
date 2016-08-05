#pragma once
#include "Core.h"
#include "RigidBody.h"
#include "Contact.h"



namespace Ryno {

	//Struct used by the contact generators to store contacts.
	struct CollisionData {
		void setup(F _max_contacts);
		Contact* contacts;
		I remaining_contacts;
		I max_contacts;
		CollisionData& operator++() {
			contacts++; remaining_contacts--; return *this;
		}

		void clear();

		//deleted to avoid calling this
		CollisionData operator++(int i) = delete;

	};

	//The primitive entity.
	//It is made by a rigidbody, and an offset,
	//because a single game object could have more than one primitve
	//with a different offset from the center
	struct CollisionPrimitive {
		friend class RigidBody;
	public:
		CollisionPrimitive(RigidBody* _body, const M4& _offset = glm::mat4()) : body(_body), offset(_offset) {}
		CollisionPrimitive(const CollisionPrimitive& copy) { offset = copy.offset; }
		virtual CollisionPrimitive* clone() { return new CollisionPrimitive(*this); }
		virtual const CollisionPrimitive& get_derived_primitive() const {	return *this;}
		RigidBody* body;
		M4 offset;			//Offset from rigidbody
		M4 transform;		//Calculated every frame from transform and offset

		//Extract position from the transform of the primitive
		V3 get_axis(U axis) const;
		V3 get_position() const { return get_axis(3); };
	protected:
		void calculate_transform();
	};


	//Sphere primitive
	class CollisionSphere : public CollisionPrimitive {
	public: 
		CollisionSphere(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionSphere(const CollisionSphere& copy) : CollisionPrimitive(copy) { radius = copy.radius; }
		CollisionSphere* clone() override { return new CollisionSphere(*this); }
		virtual const CollisionSphere& get_derived_primitive() const override {
			return *(CollisionSphere*)(this);
		};

		F radius;
	};

	//Plane primitive.
	//It ignores the rigidbody because the planes
	//are usually static
	class CollisionPlane : public CollisionPrimitive {
	public:
		CollisionPlane(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionPlane(const CollisionPlane& copy) : CollisionPrimitive(copy) { normal = copy.normal; offset = copy.offset; }
		CollisionPlane* clone() override { return new CollisionPlane(*this); }
		virtual const CollisionPlane& get_derived_primitive() const override {
			return *(CollisionPlane*)(this);
		};
		V3 normal;
		F offset;
	};

	//Box primitive
	class CollisionBox : public CollisionPrimitive {
	public:
		CollisionBox(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionBox(const CollisionBox& copy) : CollisionPrimitive(copy) { half_size = copy.half_size; }
		CollisionBox* clone() override { return new CollisionBox(*this); }
		virtual const CollisionBox& get_derived_primitive() const override {
			return *(CollisionBox*)(this);
		};
		V3 half_size;
	};

	
	//static class that handles the different
	//types of collisions
	class CollisionDetector
	{
	public:

		static U collide(
			const CollisionPrimitive &one,
			const CollisionPrimitive &two,
			CollisionData &data);

		static U collide(
			const CollisionSphere &one,
			const CollisionSphere &two,
			CollisionData &data
			);


		static U collide(
			const CollisionSphere &sphere,
			const CollisionPlane &plane,
			CollisionData &data
			);
		//inverse
		static U collide(
			const CollisionPlane &plane,
			const CollisionSphere &sphere,
			CollisionData &data
			) {	return collide(sphere, plane, data);}

		static U collide(
			const CollisionBox &box,
			const CollisionPlane &plane,
			CollisionData &data
			);
		//inverse
		static U collide(
			const CollisionPlane &plane,
			const CollisionBox &box,
			CollisionData &data
			) {	return collide(box, plane, data);}

		static U collide(
			const CollisionBox &one,
			const CollisionBox &two,
			CollisionData &data
			);


		static U collide(
			const CollisionBox &box,
			const CollisionSphere &sphere,
			CollisionData &data
			);
		//Inverse 
		static U collide(
			const CollisionSphere &sphere,
			const CollisionBox &box,
			CollisionData &data
			) {	return collide(box, sphere, data);}


	};

	

}