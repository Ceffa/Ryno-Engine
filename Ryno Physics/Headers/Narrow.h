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
	public:
		Primitive(RigidBody* _body, const M4& _offset) : body(_body), offset(_offset) {}
		RigidBody* body;
		M4 offset;
	};

	struct PrimitiveSet {
	public:
		PrimitiveSet() {}
		std::vector<Primitive> primitives;
	};

	struct CollisionData {
		Contact* contacts;
		I contacts_left;
	};

}