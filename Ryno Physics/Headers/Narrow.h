#pragma once
#include "Core.h"
#include "RigidBody.h"
#include "Contact.h"
#include "Broad.h"



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

	enum PrimitiveType {Point, HalfPlane, FullPlane, Box, Sphere, Primitive};

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
		virtual PrimitiveType get_type() const { return PrimitiveType::Primitive; }
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
		PrimitiveType get_type() const override { return PrimitiveType::Sphere; }

		F radius;
	};

	//Point primitive
	class CollisionPoint : public CollisionPrimitive {
	public:
		CollisionPoint(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionPoint(const CollisionPoint& copy) : CollisionPrimitive(copy) {}
		CollisionPrimitive* clone() override { return new CollisionPoint(*this); }
		PrimitiveType get_type() const override { return PrimitiveType::Point; }
	};

	//Plane primitive.
	class CollisionHalfPlane : public CollisionPrimitive {
	public:
		CollisionHalfPlane(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionHalfPlane(const CollisionHalfPlane& copy) : CollisionPrimitive(copy) { normal = copy.normal; offset = copy.offset; }
		CollisionHalfPlane* clone() override { return new CollisionHalfPlane(*this); }
		virtual PrimitiveType get_type() const { return PrimitiveType::HalfPlane; }

		V3 normal;
		F offset;
	};

	//Plane primitive.
	class CollisionFullPlane : public CollisionPrimitive {
	public:
		CollisionFullPlane(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionFullPlane(const CollisionFullPlane& copy) : CollisionPrimitive(copy) { normal = copy.normal; offset = copy.offset; }
		CollisionFullPlane* clone() override { return new CollisionFullPlane(*this); }
		virtual PrimitiveType get_type() const { return PrimitiveType::FullPlane; }

		V3 normal;
		F offset;
	};

	//Box primitive
	class CollisionBox : public CollisionPrimitive {
	public:
		CollisionBox(RigidBody* _body, const M4& _offset) : CollisionPrimitive(_body, _offset) {}
		CollisionBox(const CollisionBox& copy) : CollisionPrimitive(copy) { half_size = copy.half_size; }
		CollisionBox* clone() override { return new CollisionBox(*this); }
		virtual PrimitiveType get_type() const { return PrimitiveType::Box; }

		V3 half_size;
	};

	
	//static class that handles the different
	//types of collisions
	class CollisionDetector
	{
	public:

		static CollisionData data;

		static void set_up();

		static void detect_all_contacts(const std::vector<PotentialContact>& contacts);
		
		
		//Fallback
		static U detect(const CollisionPrimitive &one, const CollisionPrimitive &two) { return 0; }

		static U detect(const CollisionSphere &one,	const CollisionSphere &two);


		static U detect(const CollisionSphere &sphere,const CollisionHalfPlane &plane);
		//inverse
		static U detect(const CollisionHalfPlane &plane,const CollisionSphere &sphere) {return detect(sphere, plane);}

		static U detect(const CollisionSphere &sphere,const CollisionFullPlane &plane);
		//inverse
		static U detect(const CollisionFullPlane &plane,const CollisionSphere &sphere) {return detect(sphere, plane);}


		static U detect(const CollisionBox &box,const CollisionHalfPlane &plane);
		//inverse
		static U detect(const CollisionHalfPlane &plane, const CollisionBox &box) { return detect(box, plane); }

		static U detect(const CollisionBox &one,const CollisionBox &two);


		static U detect(const CollisionBox &box,const CollisionSphere &sphere);
		//Inverse 
		static U detect(const CollisionSphere &sphere,const CollisionBox &box) {return detect(box, sphere);}


		static U detect_two_unknown(const CollisionPrimitive &one, const CollisionPrimitive &two)
		{
			if (const auto a = dynamic_cast<const CollisionSphere*>(&one))
				return detect_one_unknown(*a, two);
			else if (const auto a = dynamic_cast<const CollisionBox*>(&one))
				return detect_one_unknown(*a, two);
			else if (const auto a = dynamic_cast<const CollisionFullPlane*>(&one))
				return detect_one_unknown(*a, two);
			else if (const auto a = dynamic_cast<const CollisionHalfPlane*>(&one))
				return detect_one_unknown(*a, two);
			else if (const auto a = dynamic_cast<const CollisionPoint*>(&one))
				return detect_one_unknown(*a, two);
		}


		template<class T>
		static U detect_one_unknown(const T& known, const CollisionPrimitive& unknown) {
			
			if (const auto a = dynamic_cast<const CollisionSphere*>(&unknown))
				return detect(known,*a);
			else if (const auto a = dynamic_cast<const CollisionBox*>(&unknown))
				return detect(known, *a);
			else if (const auto a = dynamic_cast<const CollisionFullPlane*>(&unknown))
				return detect(known, *a);
			else if (const auto a = dynamic_cast<const CollisionHalfPlane*>(&unknown))
				return detect(known, *a);
			else if (const auto a = dynamic_cast<const CollisionPoint*>(&unknown))
				return detect(known, *a);
		}
	};

	

}