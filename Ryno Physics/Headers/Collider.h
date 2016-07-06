#include "Types.h"
#include "glm/glm.hpp"

namespace Ryno {

	class Collider {
	public:
		glm::vec3 center;
	};

	class PointCollider : public Collider {
	
	};

	class SphereCollider : public Collider {
	public:
		F32 radius;
	};

	class AABBCollider : public Collider {
	public:
		glm::vec3 size;
	};

	class OBBCollider : public Collider {
	public:
		glm::vec3 size;
	};

	class CapsuleCollider : public Collider {
	public:
		F32 radius;
		F32 height;
	};

}

