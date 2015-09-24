#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"

//To be implemented by colliders that uses GJK
namespace Ryno{
	class AABBCollider : public Collider {
		friend class GJK;
		
	public:
		AABBCollider();
		AABBCollider(const glm::vec3& _center, const glm::vec3& _size);

		AABBCollider(const AABBCollider* s){ *this = *s; }
		
		Collider* get_copy() override;

		glm::vec3 center;
		glm::vec3 size;

	protected:
		glm::vec3 get_farthest_point(const glm::vec3& dir) override;
		glm::vec3 get_center() override;
		
		Collider* adapt_to_transform(Transform* t) override;

	};
}