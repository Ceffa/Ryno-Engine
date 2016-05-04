#include "AABBCollider.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
namespace Ryno{

	AABBCollider::AABBCollider(const glm::dvec3& _center, const glm::dvec3& _size)
	{
		center = _center;
		size = _size;
	}




	Collider* AABBCollider::get_copy()
	{
		return new AABBCollider(this);
	}

	glm::dvec3 AABBCollider::get_farthest_point(const glm::dvec3& dir)
	{
		F64 x, y, z;
		x = dir.x >= 0 ? 1 : -1;
		y = dir.y >= 0 ? 1 : -1;
		z = dir.z >= 0 ? 1 : -1;

		return transformed_center + transformed_size * glm::dvec3(glm::dvec3(x, y, z));

	}

	glm::dvec3 AABBCollider::get_center()
	{
		return transformed_center;
	}

	void AABBCollider::adapt_to_transform(Transform* t)
	{	
		transformed_center = center + glm::dvec3(t->get_position());
		transformed_size = size * glm::dvec3(t->get_scale());
	}

}