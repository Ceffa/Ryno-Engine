#pragma once
#include "Types.h"
#include "glm/glm.hpp"
#include "Script.h"

namespace Ryno {
	
	class Collider : public Script {
	public:
		
		bool is_unique() override { return false; }
		bool is_copyable() override { return true; }
			
		glm::vec3 center;
	};

	class PointCollider : public Collider {
	public:
		PointCollider* clone() override { return new PointCollider(*this); }
	};

	class SphereCollider : public Collider {
	public:
		SphereCollider* clone() override { return new SphereCollider(*this); }

		F32 radius;
	};

	class AABBCollider : public Collider {
	public:
		AABBCollider* clone() override { return new AABBCollider(*this); }

		glm::vec3 size;
	};

	class OBBCollider : public Collider {
	public:
		OBBCollider* clone() override { return new OBBCollider(*this); }

		glm::vec3 size;
	};

	class CapsuleCollider : public Collider {
	public:
		CapsuleCollider* clone() override { return new CapsuleCollider(*this); }

		F32 radius;
		F32 height;
	};

}

