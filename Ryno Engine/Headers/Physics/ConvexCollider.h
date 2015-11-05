#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"
#include "MeshManager.h"

//To be implemented by colliders that uses GJK
namespace Ryno{
	class ConvexCollider : public Collider {
		friend class GJK;
		
	public:
		
		ConvexCollider(I32 _coll_mesh,const glm::dvec3& _center, const glm::dvec3& _size);
		ConvexCollider::ConvexCollider(I32 _coll_mesh) :
			ConvexCollider(_coll_mesh, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)){}

		ConvexCollider(const ConvexCollider* c){ *this = *c; }
		
		MeshManager* mesh_manager;

		I32 collider_mesh;
		glm::dvec3 center;
		glm::dvec3 size;
		glm::dvec3 transformed_center;
		glm::dvec3 transformed_size;
	
		Collider* get_copy() override;

	protected:
		glm::dvec3 get_farthest_point(const glm::dvec3& dir) override;
		glm::dvec3 get_center() override;
		
		void adapt_to_transform(Transform* t) override;

	};
}