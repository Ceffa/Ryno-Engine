#pragma once
#include <GLM/glm.hpp>
#include "Transform.h"



namespace Ryno{
	//To be implemented by colliders that uses GJK
	class Collider {
		friend class GJK;
	public:
		Collider(){}

		virtual Collider* get_copy() = 0;

	protected:
		virtual glm::dvec3 get_farthest_point(const glm::dvec3& dir) = 0;
		virtual glm::dvec3 get_center()=0;
		virtual void adapt_to_transform(Transform* t) =0;
	};
}