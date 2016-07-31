#pragma once
#include "Core.h"
#include "RigidBody.h"
#include "Contact.h"



namespace Ryno {

	//The primitive entity.
	//It is made by a rigidbody, and an offset,
	//because a single game object could have more than one primitve
	//with a different offset from the center
	struct Primitive {
		friend class RigidBody;
	public:
		Primitive(RigidBody* _body, const M4& _offset = glm::mat4()) : body(_body), offset(_offset) {}
		Primitive(const Primitive& copy) { offset = copy.offset; }
		virtual Primitive* clone() { return new Primitive(*this); }
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
	class CollisionSphere : public Primitive {
	public: 
		CollisionSphere(RigidBody* _body, const M4& _offset) : Primitive(_body, _offset) {}
		CollisionSphere(const CollisionSphere& copy) : Primitive(copy) { radius = copy.radius; }
		CollisionSphere* clone() override { return new CollisionSphere(*this); }


		F radius;
	};

	//Plane primitive.
	//It ignores the rigidbody because the planes
	//are usually static
	class CollisionPlane : public Primitive {
	public:
		CollisionPlane(RigidBody* _body, const M4& _offset) : Primitive(_body, _offset) {}
		CollisionPlane(const CollisionPlane& copy) : Primitive(copy) { normal = copy.normal; offset = copy.offset; }
		CollisionPlane* clone() override { return new CollisionPlane(*this); }

		V3 normal;
		F offset;
	};

	//Box primitive
	class CollisionBox : public Primitive {
	public:
		CollisionBox(RigidBody* _body, const M4& _offset) : Primitive(_body, _offset) {}
		CollisionBox(const CollisionBox& copy) : Primitive(copy) { half_size = copy.half_size; }
		CollisionBox* clone() override { return new CollisionBox(*this); }

		V3 half_size;
	};

	//Struct used by the contact generators to store contacts.
	struct CollisionData {
		
		void setup(F _max_contacts);
		Contact* contacts;
		I remaining_contacts;
		CollisionData& operator++() {
			contacts++; remaining_contacts--; return *this;
		}

		void clear();

		//deleted to avoid calling this
		CollisionData operator++(int i) = delete;
	private:
		I max_contacts;

	};

	//static class that handles the different
	//types of collisions
	class CollisionDetector
	{
	public:

		static U sphere_and_sphere(
			const CollisionSphere &one,
			const CollisionSphere &two,
			CollisionData &data
			);

		//Half spaces are infinitely thick on one side
		static U sphere_and_half_space(
			const CollisionSphere &sphere,
			const CollisionPlane &plane,
			CollisionData &data
			);

		//True planes are thin, and if the object goes 
		//over them, the normal of the contact must be 
		//multiplied by -1
		static U sphere_and_true_plane(
			const CollisionSphere &sphere,
			const CollisionPlane &plane,
			CollisionData &data
			);


		/**
		* Does a collision test on a collision box and a plane representing
		* a half-space (i.e. the normal of the plane
		* points out of the half-space).
		*/
		static U box_and_half_space(
			const CollisionBox &box,
			const CollisionPlane &plane,
			CollisionData &data
			);

		static U box_and_box(
			const CollisionBox &one,
			const CollisionBox &two,
			CollisionData &data
			);

		static U box_and_point(
			const CollisionBox &box,
			const V3 &point,
			CollisionData &data
			);

		static U box_and_sphere(
			const CollisionBox &box,
			const CollisionSphere &sphere,
			CollisionData &data
			);
	};

	class IntersectionTest
	{
	public:

		
		static bool box_and_half_space(
			const CollisionBox &box,
			const CollisionPlane &plane);

	};

}