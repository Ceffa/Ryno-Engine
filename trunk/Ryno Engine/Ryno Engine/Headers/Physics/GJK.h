#pragma once
#include <GLM/glm.hpp>
#include "Global.h"
#include "Collider.h"
#include "GameObject.h"
namespace Ryno{

	struct Simplex{
		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 c;
		F32 length = 0;
		void add(const glm::vec3& p){
			length++;
			c = b;
			b = a;
			a = p;
		}
		void remove_a(){
			length--;
			a = b;
			b = c;
			c = glm::vec3(0);
		}
		void remove_b(){
			length--;
			b = c;
			c = glm::vec3(0);
		}
		void remove_c(){
			length--;
			c = glm::vec3(0);
		}
		glm::vec3 get_last(){
			return a;
		}

		void wipe(){
			a = glm::vec3(0, 0, 0);
			b = a; 
			c = a; 
			length = 0;
		}
	};

	class GJK{
	public:
		GJK(){}

		static bool gjk_colliders(Collider* c1, Collider* c2);
		static bool gjk(GameObject* go1, GameObject* go2);

	private:
		static Simplex simplex;
		static glm::vec3 dir;
		static glm::vec3 support(Collider* c1, Collider* c2);
		static bool is_origin_included();

	};
}