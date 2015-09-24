#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"

//To be implemented by colliders that uses GJK
namespace Ryno{
	class SphereCollider : public Collider {
		friend class GJK;
		
	public:
		SphereCollider();
		SphereCollider(const glm::vec3& _center, F32 _radius);
		SphereCollider(F32 x, F32 y, F32 z, F32 _radius)
			: SphereCollider(glm::vec3(x, y, z), _radius){}
		SphereCollider(const SphereCollider* s){ *this = *s; }
		
		Collider* get_copy() override;

		glm::vec3 center;
		F32 radius;

	protected:
		glm::vec3 get_farthest_point(const glm::vec3& dir) override;
		glm::vec3 get_center() override;
		
		Collider* adapt_to_transform(Transform* t) override;

	};
}