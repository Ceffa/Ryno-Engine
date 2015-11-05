#include "SphereCollider.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
namespace Ryno{

	SphereCollider::SphereCollider(const glm::dvec3& _center, F32 _radius)
	{
		center = _center;
		radius = _radius;
	}

		
	Collider* SphereCollider::get_copy()
	{
		return new SphereCollider(this);
	}

	glm::dvec3 SphereCollider::get_farthest_point(const glm::dvec3& dir)
	{
		return transformed_center + normalize(dir) * (F64)transformed_radius;
	}

	glm::dvec3 SphereCollider::get_center()
	{
		return transformed_center;
	}

	void SphereCollider::adapt_to_transform(Transform* t)
	{
		
		transformed_center = center + glm::dvec3(t->position);
		transformed_radius = radius * t->scale.x;	//meh
	
	}

}