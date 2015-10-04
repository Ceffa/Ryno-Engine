#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"

//To be implemented by colliders that uses GJK
namespace Ryno{
	class PointCollider : public Collider {
		friend class GJK;
		
	public:
		PointCollider(const glm::dvec3& _center);
		PointCollider() :
			PointCollider(glm::dvec3(0, 0, 0)){}

		PointCollider(const PointCollider* s){ *this = *s; }

		glm::dvec3 center;
		glm::dvec3 transformed_center;

		Collider* get_copy() override;

	protected:
		glm::dvec3 get_farthest_point(const glm::dvec3& dir) override;
		glm::dvec3 get_center() override;
		
		void adapt_to_transform(Transform* t) override;

	};
}