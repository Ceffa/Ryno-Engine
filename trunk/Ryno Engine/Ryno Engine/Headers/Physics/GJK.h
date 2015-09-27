#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"
#include "GameObject.h"
namespace Ryno{

	
	class GJK{
	public:
		GJK(){}

		static bool gjk_colliders(Collider* c1, Collider* c2);
		static bool gjk(GameObject* go1, GameObject* go2);

	private:
		static glm::vec3 support(Collider*, Collider*,glm::vec3& dir);
		static bool contains_origin( glm::vec3& dir);
		static bool triangle(glm::vec3& dir);
		static bool tetrahedron(glm::vec3& dir);
		static bool check_tetrahedron(const glm::vec3& ao, const glm::vec3& ab, const glm::vec3& ac, const glm::vec3& abc, glm::vec3& dir);

		static glm::vec3 a, b, c, d;	//four simplex points
		static F32 length; //current number of simplex points



	};
}