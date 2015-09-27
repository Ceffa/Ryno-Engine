#include "AABBCollider.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
namespace Ryno{

	AABBCollider::AABBCollider(const glm::vec3& _center, const glm::vec3& _size)
	{
		center = _center;
		size = _size;
	}

	AABBCollider::AABBCollider()
	{
		center = glm::vec3(0, 0, 0);
		size = glm::vec3(1,1,1);
	}

	Collider* AABBCollider::get_copy()
	{
		return new AABBCollider(this);
	}

	glm::vec3 AABBCollider::get_farthest_point(const glm::vec3& dir)
	{
		F32 x, y, z;
		if (dir.x >=0) x = size.x; else x = -size.x;
		if (dir.y >= 0) y = size.y; else y = -size.y;
		if (dir.z >=0) z = size.z; else z = -size.z;

		return center + glm::vec3(x, y, z);

	}

	glm::vec3 AABBCollider::get_center()
	{
		return center;
	}

	Collider* AABBCollider::adapt_to_transform(Transform* t)
	{
		AABBCollider* s = new AABBCollider();
		s->center = center + t->position;
		s->size = t->scale;	
		return s;
	}

}