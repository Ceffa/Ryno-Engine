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
		static glm::dvec3 support(Collider*, Collider*,glm::dvec3& dir);
		static bool contains_origin( glm::dvec3& dir);
		static bool triangle(glm::dvec3& dir);
		static bool tetrahedron(glm::dvec3& dir);
		static bool check_tetrahedron(const glm::dvec3& ao, const glm::dvec3& ab, const glm::dvec3& ac, const glm::dvec3& abc, glm::dvec3& dir);

		static glm::dvec3 a, b, c, d;	//four simplex points
		static F32 length; //current number of simplex points



	};
}