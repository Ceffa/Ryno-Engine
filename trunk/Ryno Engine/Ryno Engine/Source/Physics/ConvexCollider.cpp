#include "ConvexCollider.h"
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>
#include "Log.h"
namespace Ryno{

	ConvexCollider::ConvexCollider(I32 _coll_mesh,const glm::dvec3& _center, const glm::dvec3& _size)
	{
		mesh_manager = MeshManager::get_instance();
		collider_mesh = _coll_mesh;
		center = _center;
		size = _size;
	}



	Collider* ConvexCollider::get_copy()
	{
		return new ConvexCollider(this);
	}

	glm::dvec3 ConvexCollider::get_farthest_point(const glm::dvec3& dir)
	{
		U32 support_point = 0;
		glm::vec3 support_direction = dir;
		std::vector<glm::vec3>* vertices = &mesh_manager->get_collider_mesh(collider_mesh)->vertices;
		I32 size = mesh_manager->get_collider_mesh(collider_mesh)->size;
		F64 dot_result = dot((*vertices)[0], support_direction);
	
		for (U32 cnt = 1; cnt < size; cnt++)
		{
			F64 temp_dot = dot((*vertices)[cnt], support_direction);
			if (temp_dot > dot_result)
			{
				support_point = cnt;
				dot_result = temp_dot;
			}
		}
		

		Log::println(to_string(glm::dvec3((*vertices)[support_point])));
		return transformed_center + transformed_size * glm::dvec3((*vertices)[support_point]);

	}

	glm::dvec3 ConvexCollider::get_center()
	{
		return transformed_center;
	}

	void ConvexCollider::adapt_to_transform(Transform* t)
	{
		transformed_center = center + glm::dvec3(t->position);
		transformed_size = size * glm::dvec3(t->scale);
	}

}