#include "GJK.h"
#include "Log.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>



namespace Ryno{

	glm::dvec3 GJK::a = glm::dvec3(0);
	glm::dvec3 GJK::b = a, GJK::c = a, GJK::d = a;
	F32 GJK::length = 0;


	glm::dvec3 double_cross(const glm::dvec3& v1, const glm::dvec3& v2){
		return glm::cross(glm::cross(v1, v2), v1);
	}

	bool GJK::gjk(GameObject* go1, GameObject* go2){
		go1->collider->adapt_to_transform(*go1->transform);
		go2->collider->adapt_to_transform(*go2->transform);
		
		bool b = gjk_colliders(*go1->collider, *go2->collider);

		return b;
	}

	bool GJK::gjk_colliders(Collider* c1, Collider* c2)
	{

		Log::println("");

		a = glm::dvec3(0);
		b = glm::dvec3(0);
		c = glm::dvec3(0);
		d = glm::dvec3(0);

		//Random initial dir
		glm::dvec3 dir = glm::dvec3(normalize(c2->get_center() - c1->get_center()));
		



		//Get support in the initial direction
		c = support(c1, c2, dir);
		//Invert the direction
		dir = -c;

		//Get the new support in the new direction
		b = support(c1,c2, dir);

		//Se il dot tra OB e DIR e' minore di zero si ritorna, 
		//dato che l'origine e' oltre e irraggiungibile
		if (dot(b, dir) < 0) return false;
		
		//Get new direction on the same plane as BC and perpendicular to B
		dir = double_cross(c - b, -b);

		//Update the length of the simplex to 2
		length = 2; //begin with 2 points in simplex

		//Loop till 50 to avoid infinite loop. 
		//It is just a precaution, it should never loop infinitely
		I32 steps = 0;

		//Main Loop
		while (steps < 50)
		{
			//Get new point a
			a = support(c1, c2, dir);

			//Return false if a is not past the origin
			if (dot(a, dir) < 0) return false;
			//else return true if the current simplex holds the origin
			else if (contains_origin(dir)) return true;
				
			steps++;
			if (steps == 50)
				std::cout << "Warning: GJK loop" << std::endl;
		}

		return false;
	}

	bool GJK::contains_origin(glm::dvec3& dir)
	{

		if (length == 2)
		{
			return triangle(dir);	//always returns false actually
		}
		else if (length == 3)
		{
			return tetrahedron(dir);
		}

		return false;
	}


	bool GJK::triangle(glm::dvec3& dir)
	{
		//Get the three edges of triangles
		glm::dvec3 ao = -a;
		glm::dvec3 ab = b - a;
		glm::dvec3 ac = c - a;
		//Get normal to triangle surface
		glm::dvec3 abc = glm::cross(ab, ac);

		//Point a cannot be behind the BC segment.
		//But in can be in three different locations after BC: in the triangle, at its left or right.
		//Let's test this:

		//Get the normal of AB.
		//Then check the dot with the origin.
		//If > 0, since the normal is pointing outside the triangle, the origin is too, 
		//and thus it must be outside the triangle.
		glm::dvec3 ab_abc = glm::cross(ab, abc);

		if (dot(ab_abc,ao) > 0)
		{
			//If origin is outside, I remove the point C,
			//I go back to a line and set the normal as the new direction
			c = b;
			b = a;

			//Calculate the dir.
			//I cannot use ab_abc instead because, while similar, ab_abc does not point
			//Towards the origin, is just a ormal to the AB edge
			dir = double_cross(ab, ao);

			//The direction changed, the tetrahedron cannot be built
			return false;
		}

		//Then we need to check the two other cases.
		//Is it inside or outside AC?
		//Same as before, I won't comment this
		glm::dvec3 abc_ac = glm::cross(abc, ac);


		if (dot(abc_ac,ao) > 0)
		{
			//Remove B this time
			b = a;

			dir = double_cross(ac, ao);
			return false;
		}

		//If the code gets here, the origin is inside the triangle.
		//The tetrahedron can be built, but the direction is required.
		//It is a simple matter of calculating the dot product between
		//the normal to the surface of the triangle and the origin
		if (dot(abc,ao) > 0)
		{
			//base of tetrahedron
			d = c;
			c = b;
			b = a;

			//new direction
			dir = abc;
		}
		else
		{
			//upside down tetrahedron.
			//Reverse winding order
			d = b;
			//c = c; implied
			b = a;
			dir = -abc;
		}

		length = 3;



		return false;
	}
	
	

	bool GJK::tetrahedron(glm::dvec3& dir)
	{
		//Now we have the final triangle.
		//Here we get the edges
		glm::dvec3 ao = -a;
		glm::dvec3 ab = b - a;
		glm::dvec3 ac = c - a;

		//get triangle normal
		glm::dvec3 abc = glm::cross(ab, ac);

		//CASE 1: Origin in front of the triangle ABC
		if (dot(abc,ao) > 0)
		{
			check_tetrahedron(ao, ab, ac, abc, dir);
		}

		//CASE 2: Build ACD triangle and do the same
		glm::dvec3 ad = d - a;

		//build acd triangle
		glm::dvec3 acd = glm::cross(ac, ad);

		//same direaction with ao
		if (dot(acd,ao) > 0)
		{
			//in front of triangle ACD
			b = c;
			c = d;
			ab = ac;
			ac = ad;
			abc = acd;

			check_tetrahedron(ao, ab, ac, abc, dir);
		}

		//CASE 3: Same thing with ADB triangle
		//build adb triangle
		glm::dvec3 adb = glm::cross(ad, ab);

		//same direaction with ao
		if (dot(adb,ao) > 0)
		{
			//in front of triangle ADB
			c = b;
			b = d;

			ac = ab;
			ab = ad;

			abc = adb;
			check_tetrahedron(ao, ab, ac, abc, dir);
		}
		/*Log::println(to_string(d));
		Log::println(to_string(c));
		Log::println(to_string(b));
		Log::println(to_string(a));*/
		//origin in tetrahedron
		return true;

	}
	
	bool GJK::check_tetrahedron(const glm::dvec3& ao, const glm::dvec3& ab, const glm::dvec3& ac, const glm::dvec3& abc, glm::dvec3& dir)
	{

		//almost the same like triangle checks
		glm::dvec3 ab_abc = glm::cross(ab, abc);

		//If outside the AB edge of the triangle
		if (dot(ab_abc,ao) > 0)
		{
			c = b;
			b = a;

			//dir is not ab_abc because it's not pointing towards the origin (it's just a normal)
			//ABxA0xAB direction we are looking for
			dir = double_cross(ab, ao);

			//build new triangle
			// d will be lost
			length = 2;

			return false;
		}

		//Case 2
		glm::dvec3 acp = glm::cross(abc, ac);

		//If outside the AC edge
		if (dot(acp,ao) > 0)
		{
			b = a;

			//dir is not abc_ac because it's not pointing towards the origin
			//ACxA0xAC direction we are looking for
			dir = double_cross(ac, ao);

			//build new triangle
			//d will be lost
			length = 2;

			return false;
		}

		//Case 3 does not need to be checked because implicitely impossible
		//So the point must be inside the tethraedron with base:
		d = c;
		c = b;
		b = a;

		dir = abc;

		length = 3;

		//The next iteration will check it and find that the point is inside it.

		return false;
	}
	glm::dvec3 GJK::support(Collider* c1, Collider* c2, glm::dvec3& dir)
	{
		return c1->get_farthest_point(dir) - c2->get_farthest_point(-dir);
	}
}
