#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"

//To be implemented by colliders that uses GJK
namespace Ryno{
	class SphereCollider : public Collider {
		friend class GJK;
		
	public:
		SphereCollider(const glm::dvec3& _center, F32 _radius);
		SphereCollider(F32 x, F32 y, F32 z, F32 _radius)
			: SphereCollider(glm::dvec3(x, y, z), _radius){}
		SphereCollider() 
			: SphereCollider(0,0,0,1){}

		SphereCollider(const SphereCollider* s){ *this = *s; }


		Collider* get_copy() override;

		

		glm::dvec3 center;
		glm::dvec3 transformed_center;
		F32 radius;
		F32 transformed_radius;

	protected:
		glm::dvec3 get_farthest_point(const glm::dvec3& dir) override;
		glm::dvec3 get_center() override;
		
		void adapt_to_transform(Transform* t) override;

	};
}