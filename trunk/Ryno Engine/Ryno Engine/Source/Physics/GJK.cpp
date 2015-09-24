#include "GJK.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>



namespace Ryno{

	Simplex GJK::simplex;
	glm::vec3 GJK::dir;


	glm::vec3 triple_product(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c){

		glm::vec3 _a = normalize(a);
		glm::vec3 _b = normalize(b);
		glm::vec3 _c = normalize(c);
		return b*(dot(_c, _a)) - a*(dot(_c, _b));
	}

	bool GJK::gjk(GameObject* go1, GameObject* go2){
		Collider* s1 = go1->collider->adapt_to_transform(go1->transform);
		Collider* s2 = go2->collider->adapt_to_transform(go2->transform);
		
		bool b = gjk_colliders(s1, s2);
		delete s1;
		delete s2;
		return b;
	}

	bool GJK::gjk_colliders(Collider* c1, Collider* c2)
	{
		simplex.wipe();
		
		//Direction is initially the difference between the two centers
		dir = c2->get_center() - c1->get_center();
		//First simplex is the line from the support in d and -d.
		simplex.add(support(c1, c2));

		//Update d correctly to add the next point in the loop
		dir = -dir;

		// start looping
		while (true) {
			std::cout << to_string(dir) << std::endl;
			// add a new point to the simplex
			simplex.add(support(c1, c2));
			// make sure that the last point we added actually passed the origin
			if (dot(simplex.get_last(), dir) <= 0) {

				// if the point added last was not past the origin in the direction of d
				// then the Minkowski Sum cannot possibly contain the origin since
				// the last point added is on the edge of the Minkowski Difference
				return false;
			}
			else {
				// otherwise we need to determine if the origin is in
				// the current simplex
				if (is_origin_included()){
					// if it does then we know there is a collision
					return true;
				}
			}
		}
		return false;
	}

	bool GJK::is_origin_included(){

		glm::vec3 a, ao, b, c, ab, ac, bc;

		// get the last point added to the simplex
		a = simplex.get_last();
		// compute AO (same thing as -A)
		ao = -a;

		if (simplex.length == 3) {

			// then its the triangle case
			// get b and c
			b = simplex.b;
			c = simplex.c;

			// compute the edges
			ab = b - a;
			ac = c - a;
			// compute the normals
			glm::vec3 ab_perp = triple_product(ac, ab, ab);
			glm::vec3 ac_perp = triple_product(ab, ac, ac);
			// is the origin in R4
			if (dot(ab_perp, ao) > 0) {
				// remove point c
				simplex.remove_c();

				// set the new direction to abPerp
				dir = ab_perp;

			}
			else {
				if (dot(ac_perp, ao) > 0) {
					// remove point b
					simplex.remove_b();
					// set the new direction to acPerp
					dir = ac_perp;
				}
				else{
					// otherwise we know its in R5 so we can return true
					//this doesn't work in 3d sadly
					return true;
				}
			}
		}
		else {
			// then its the line segment case
			b = simplex.b;
			// compute AB
			ab = b - a;
		

			// get the perp to AB in the direction of the origin
			glm::vec3 ab_perp = triple_product(ab, ao, ab);
			// set the direction to abPerp
			dir = ab_perp;

		}
		return false;
		}
	
	

	glm::vec3 GJK::support(Collider* c1, Collider* c2)
	{
		
		return c1->get_farthest_point(dir) - c2->get_farthest_point(-dir);
	}
}
