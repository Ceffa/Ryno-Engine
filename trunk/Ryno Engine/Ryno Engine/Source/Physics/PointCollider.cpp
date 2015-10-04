#include "PointCollider.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
namespace Ryno{

	PointCollider::PointCollider(const glm::dvec3& _center)
	{
		center = _center;
	}
	
	Collider* PointCollider::get_copy()
	{
		return new PointCollider(this);
	}

	glm::dvec3 PointCollider::get_farthest_point(const glm::dvec3& dir)
	{
		return transformed_center;
	}

	glm::dvec3 PointCollider::get_center()
	{
		return center;
	}

	void PointCollider::adapt_to_transform(Transform* t)
	{
		transformed_center = center + glm::dvec3(t->position);
	}

}