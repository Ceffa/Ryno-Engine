#include "SphereCollider.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
namespace Ryno{

	SphereCollider::SphereCollider(const glm::vec3& _center, F32 _radius)
	{
		center = _center;
		radius = _radius;
	}

	SphereCollider::SphereCollider()
	{
		center = glm::vec3(0, 0, 0);
		radius = 1;
	}

	Collider* SphereCollider::get_copy()
	{
		return new SphereCollider(this);
	}

	glm::vec3 SphereCollider::get_farthest_point(const glm::vec3& dir)
	{
		return center + normalize(dir) *radius;
	}

	glm::vec3 SphereCollider::get_center()
	{
		return center;
	}

	Collider* SphereCollider::adapt_to_transform(Transform* t)
	{
		SphereCollider* s = new SphereCollider();
		s->center = center + t->position;
		s->radius = radius * t->scale.x;	//meh
		return s;
	}

}